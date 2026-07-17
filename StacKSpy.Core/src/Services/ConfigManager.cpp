#include "Services/ConfigManager.h"
#include <fstream>

namespace StacKSpy { namespace Core { namespace Services {

    std::string ConfigManager::GetString(const std::string& key, const std::string& defaultValue) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_data.find(key);
        return it != m_data.end() ? it->second : defaultValue;
    }

    int ConfigManager::GetInt(const std::string& key, int defaultValue) {
        auto val = GetString(key, "");
        return val.empty() ? defaultValue : std::stoi(val);
    }

    double ConfigManager::GetDouble(const std::string& key, double defaultValue) {
        auto val = GetString(key, "");
        return val.empty() ? defaultValue : std::stod(val);
    }

    bool ConfigManager::GetBool(const std::string& key, bool defaultValue) {
        auto val = GetString(key, "");
        return val.empty() ? defaultValue : (val == "true" || val == "1");
    }

    void ConfigManager::SetString(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data[key] = value;
    }

    void ConfigManager::SetInt(const std::string& key, int value) { SetString(key, std::to_string(value)); }
    void ConfigManager::SetDouble(const std::string& key, double value) { SetString(key, std::to_string(value)); }
    void ConfigManager::SetBool(const std::string& key, bool value) { SetString(key, value ? "true" : "false"); }

    void ConfigManager::Save() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream file(m_filePath);
        file << "{";
        bool first = true;
        for (const auto& [key, value] : m_data) {
            if (!first) file << ",";
            file << "\"" << key << "\":\"" << value << "\"";
            first = false;
        }
        file << "}";
    }

    void ConfigManager::Load() {
        std::lock_guard<std::mutex> lock(m_mutex);
        // 设置默认配置
        m_data["PriceRefreshInterval"] = "60";
        m_data["KDJRefreshTime"] = "17:00";
        m_data["AlertThresholds"] = "5,0,5";
        m_data["AutoStart"] = "true";
        m_data["WindowMode"] = "main";
        // TODO: 从 config.json 文件加载并覆盖默认值
    }

}}}
