#include "Services/StockPriceService.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <winhttp.h>
#include <vector>

#pragma comment(lib, "winhttp.lib")

namespace StacKSpy { namespace Core { namespace Services {

    // ===== 辅助：GBK 字节转 UTF-8 字符串 =====
    static std::string GbkBytesToUtf8(const char* gbkData, int gbkLen) {
        // GBK → UTF-16
        int wlen = MultiByteToWideChar(936, 0, gbkData, gbkLen, nullptr, 0);
        if (wlen <= 0) return {};
        std::vector<wchar_t> wbuf(wlen);
        MultiByteToWideChar(936, 0, gbkData, gbkLen, wbuf.data(), wlen);

        // UTF-16 → UTF-8
        int ulen = WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), wlen, nullptr, 0, nullptr, nullptr);
        if (ulen <= 0) return {};
        std::string result(ulen, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wbuf.data(), wlen, result.data(), ulen, nullptr, nullptr);
        // 移除可能的 null 终止符
        if (!result.empty() && result.back() == '\0') result.pop_back();
        return result;
    }

    // ===== 辅助：UTF-8 字符串转 UTF-16 宽字符串 =====
    static std::wstring Utf8ToWide(const std::string& str) {
        if (str.empty()) return {};
        int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        if (len <= 0) return {};
        std::wstring result(len - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, result.data(), len);
        return result;
    }

    // ===== 辅助：股票代码 → 市场前缀（sh/sz） =====
    static std::string GetMarketPrefix(const std::string& code) {
        if (code.empty()) return "sh";
        // 6开头=上海, 9开头=上海B股, 5开头=上海基金
        if (code[0] == '6' || code[0] == '9' || code[0] == '5')
            return "sh";
        // 0/3开头=深圳, 1/2开头=深圳基金/债券
        return "sz";
    }

    // ===== 辅助：按分隔符拆分字符串 =====
    static std::vector<std::string> SplitString(const std::string& str, char delim) {
        std::vector<std::string> tokens;
        std::istringstream stream(str);
        std::string token;
        while (std::getline(stream, token, delim))
            tokens.push_back(token);
        return tokens;
    }

    // ===== 辅助：HTTP GET 请求（腾讯行情 API）=====
    // 腾讯接口: http://qt.gtimg.cn/q=sh600519,sz000858
    // 返回 GBK 编码，字段以 ~ 分隔
    static std::string HttpGetTencent(const std::string& url) {
        std::wstring wHost = Utf8ToWide("qt.gtimg.cn");
        std::wstring wPath = Utf8ToWide(url);

        HINTERNET hSession = WinHttpOpen(L"StacKSpy/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) return {};

        HINTERNET hConnect = WinHttpConnect(hSession, wHost.c_str(), INTERNET_DEFAULT_HTTP_PORT, 0);
        if (!hConnect) { WinHttpCloseHandle(hSession); return {}; }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wPath.c_str(),
            nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
        if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return {}; }

        // 设置超时: 连接5秒, 发送5秒, 接收5秒
        WinHttpSetTimeouts(hRequest, 5000, 5000, 5000, 5000);

        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
            WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
            return {};
        }

        if (!WinHttpReceiveResponse(hRequest, nullptr)) {
            WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
            return {};
        }

        // 检查 HTTP 状态码
        DWORD statusCode = 0;
        DWORD size = sizeof(statusCode);
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &size, WINHTTP_NO_HEADER_INDEX);
        if (statusCode != 200) {
            WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
            return {};
        }

        // 读取响应体
        std::string rawBody;
        DWORD bytesRead = 0;
        char buffer[4096];
        while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            rawBody.append(buffer, bytesRead);
            bytesRead = 0;
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        // GBK → UTF-8
        return GbkBytesToUtf8(rawBody.c_str(), static_cast<int>(rawBody.size()));
    }

    // ===== 核心：批量获取所有股票实时价格 =====
    std::vector<Models::StockPrice> StockPriceService::FetchAllPrices(const std::vector<std::string>& stockCodes) {
        std::vector<Models::StockPrice> results;
        if (stockCodes.empty()) return results;

        // 构建批量请求 URL: /q=sh600519,sz000858,sz300750
        std::string url = "/q=";
        for (size_t i = 0; i < stockCodes.size(); ++i) {
            if (i > 0) url += ",";
            url += GetMarketPrefix(stockCodes[i]) + stockCodes[i];
        }

        std::string responseBody = HttpGetTencent(url);
        if (responseBody.empty()) {
            // 请求失败，返回空数据（不崩溃）
            return results;
        }

        // 获取当前时间作为兜底
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        struct tm tmBuf = {};
        localtime_s(&tmBuf, &timeT);
        char timeBuf[32];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tmBuf);
        std::string currentTime = timeBuf;

        // 解析每一行: v_shXXXXXX="field0~field1~field2~..."
        std::istringstream lines(responseBody);
        std::string line;
        while (std::getline(lines, line)) {
            // 提取引号内的数据: "field0~field1~..."
            auto qStart = line.find('"');
            auto qEnd = line.rfind('"');
            if (qStart == std::string::npos || qEnd == std::string::npos || qEnd <= qStart + 1)
                continue;

            std::string data = line.substr(qStart + 1, qEnd - qStart - 1);
            auto fields = SplitString(data, '~');

            // 腾讯行情 API 字段索引（~分隔）:
            // [1]  股票名称       [2]  股票代码
            // [3]  当前价格       [4]  昨收价
            // [5]  今开价         [6]  成交量(手)
            // [31] 涨跌额         [32] 涨跌幅(%)
            // [38] 换手率(%)      [30] 日期时间
            if (fields.size() < 40) continue;

            Models::StockPrice price;
            try {
                price.StockName      = fields[1];
                price.StockCode      = fields[2];
                price.Price          = std::stod(fields[3]);
                price.ChangePercent  = (fields.size() > 32) ? std::stod(fields[32]) : 0.0;
                price.TurnoverRate   = (fields.size() > 38) ? std::stod(fields[38]) : 0.0;
                price.UpdateTime     = (fields.size() > 30 && !fields[30].empty())
                                       ? fields[30] : currentTime;
            } catch (...) {
                // 解析异常，跳过该条数据
                continue;
            }

            results.push_back(price);
        }

        return results;
    }

    // ===== 查询单只股票名称 =====
    std::string StockPriceService::QueryStockName(const std::string& code) {
        std::string url = "/q=" + GetMarketPrefix(code) + code;
        std::string responseBody = HttpGetTencent(url);

        if (responseBody.empty()) return code;

        // 解析单条数据
        auto qStart = responseBody.find('"');
        auto qEnd = responseBody.rfind('"');
        if (qStart == std::string::npos || qEnd == std::string::npos || qEnd <= qStart + 1)
            return code;

        auto fields = SplitString(responseBody.substr(qStart + 1, qEnd - qStart - 1), '~');
        if (fields.size() < 2 || fields[1].empty()) return code;

        return fields[1];
    }

}}}
