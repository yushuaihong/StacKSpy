#include "Services/StockManager.h"
#include "Services/IStockPriceService.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

namespace StacKSpy { namespace Core { namespace Services {

    StockManager::StockManager(IStockPriceService* priceService)
        : m_priceService(priceService) {}

    void StockManager::SetPriceService(IStockPriceService* priceService) {
        m_priceService = priceService;
    }

    // 内部辅助：不带锁的 Contains 检查（调用者已持锁）
    bool StockManager::ContainsLocked(const std::string& code) const {
        return std::find_if(m_stockPool.begin(), m_stockPool.end(),
            [&code](const Models::StockInfo& item) {
                return item.Code == code;
            }) != m_stockPool.end();
    }

    bool StockManager::AddStock(const std::string& code) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (ContainsLocked(code)) return false;

        // 尝试从价格服务查询股票名称
        std::string name;
        if (m_priceService) {
            name = m_priceService->QueryStockName(code);
        }
        if (name.empty()) {
            name = "";  // 查询失败时以""代码作为名称
        }

        m_stockPool.emplace_back(code, name);
        return true;
    }

    bool StockManager::RemoveStock(const std::string& code) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = std::find_if(m_stockPool.begin(), m_stockPool.end(),
            [&code](const Models::StockInfo& item) {
                return item.Code == code;
            });
        if (it == m_stockPool.end()) return false;
        m_stockPool.erase(it);
        return true;
    }

    std::vector<Models::StockInfo> StockManager::GetStockPool() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_stockPool;
    }

    std::string StockManager::GetStockName(const std::string& code) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const Models::StockInfo& stock : m_stockPool) 
        {
            if (stock.Code == code) 
            {
                return stock.Name;
            }
        }
        return "";
    }

    bool StockManager::ContainsStock(const std::string& code) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return ContainsLocked(code);
    }

    // 序列化格式：[{"code":"sh600519","name":"贵州茅台"},...]
    void StockManager::SaveStockPool() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream file(m_configPath);
        if (!file.is_open()) return;

        file << "[";
        for (size_t i = 0; i < m_stockPool.size(); ++i) {
            if (i > 0) file << ",";
            file << "{\"code\":\"" << m_stockPool[i].Code
                 << "\",\"name\":\"" << m_stockPool[i].Name << "\"}";
        }
        file << "]";
    }

    // 兼容两种格式：
    //   新格式：[{"code":"sh600519","name":"贵州茅台"},...]
    //   旧格式：["sh600519","sz000858",...]
    void StockManager::LoadStockPool() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stockPool.clear();

        std::ifstream file(m_configPath);
        if (!file.is_open()) return;

        std::stringstream buf;
        buf << file.rdbuf();
        std::string content = buf.str();

        if (content.find('{') != std::string::npos) {
            // 新格式：解析 {"code":"...","name":"..."} 对象
            size_t pos = 0;
            while ((pos = content.find("\"code\"", pos)) != std::string::npos) {
                // 提取 code 值
                pos = content.find('"', pos + 6); // 跳过 "code"
                if (pos == std::string::npos) break;
                pos++; // 跳过开头 "
                size_t end = content.find('"', pos);
                if (end == std::string::npos) break;
                std::string code = content.substr(pos, end - pos);
                pos = end + 1;

                // 提取 name 值
                std::string name;
                size_t namePos = content.find("\"name\"", pos);
                if (namePos != std::string::npos && namePos < content.find('}', pos)) {
                    size_t nq = content.find('"', namePos + 6);
                    if (nq != std::string::npos) {
                        nq++;
                        size_t ne = content.find('"', nq);
                        if (ne != std::string::npos) {
                            name = content.substr(nq, ne - nq);
                        }
                    }
                }

                if (!code.empty()) {
                    m_stockPool.emplace_back(code, name.empty() ? code : name);
                }
            }
        } else {
            // 旧格式：["code1","code2",...]
            size_t pos = 0;
            while ((pos = content.find('"', pos)) != std::string::npos) {
                pos++;
                size_t end = content.find('"', pos);
                if (end == std::string::npos) break;
                std::string code = content.substr(pos, end - pos);
                if (!code.empty()) {
                    m_stockPool.emplace_back(code, code);  // 旧数据无名称，先用代码占位
                }
                pos = end + 1;
            }
        }
    }

    void StockManager::SetConfigPath(const std::string& path) {
        m_configPath = path;
    }

}}}
