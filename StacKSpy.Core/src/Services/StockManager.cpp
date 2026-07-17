#include "Services/StockManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

namespace StacKSpy { namespace Core { namespace Services {

    // 内部辅助：不带锁的 Contains 检查（调用者已持锁）
    bool StockManager::ContainsLocked(const std::string& code) const {
        return std::find(m_stockPool.begin(), m_stockPool.end(), code) != m_stockPool.end();
    }

    bool StockManager::AddStock(const std::string& code) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (ContainsLocked(code)) return false;
        m_stockPool.push_back(code);
        return true;
    }

    bool StockManager::RemoveStock(const std::string& code) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = std::find(m_stockPool.begin(), m_stockPool.end(), code);
        if (it == m_stockPool.end()) return false;
        m_stockPool.erase(it);
        return true;
    }

    std::vector<std::string> StockManager::GetStockPool() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_stockPool;
    }

    bool StockManager::ContainsStock(const std::string& code) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return ContainsLocked(code);
    }

    void StockManager::SaveStockPool() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream file(m_configPath);
        if (!file.is_open()) return;
        file << "[";
        for (size_t i = 0; i < m_stockPool.size(); ++i) {
            if (i > 0) file << ",";
            file << "\"" << m_stockPool[i] << "\"";
        }
        file << "]";
    }

    void StockManager::LoadStockPool() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stockPool.clear();
        std::ifstream file(m_configPath);
        if (!file.is_open()) return;

        // 读取整个文件内容
        std::stringstream buf;
        buf << file.rdbuf();
        std::string content = buf.str();

        // 简单 JSON 数组解析：["code1","code2",...]
        size_t pos = 0;
        while ((pos = content.find('"', pos)) != std::string::npos) {
            pos++; // 跳过开头的 "
            size_t end = content.find('"', pos);
            if (end == std::string::npos) break;
            std::string code = content.substr(pos, end - pos);
            if (!code.empty()) {
                m_stockPool.push_back(code);
            }
            pos = end + 1;
        }
    }

    void StockManager::SetConfigPath(const std::string& path) {
        m_configPath = path;
    }

}}}
