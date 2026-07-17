#pragma once
#include "Services/IConfigManager.h"
#include <unordered_map>
#include <mutex>

namespace StacKSpy { namespace Core { namespace Services {

    class ConfigManager : public IConfigManager {
    public:
        std::string GetString(const std::string& key, const std::string& defaultValue = "") override;
        int GetInt(const std::string& key, int defaultValue = 0) override;
        double GetDouble(const std::string& key, double defaultValue = 0.0) override;
        bool GetBool(const std::string& key, bool defaultValue = false) override;
        void SetString(const std::string& key, const std::string& value) override;
        void SetInt(const std::string& key, int value) override;
        void SetDouble(const std::string& key, double value) override;
        void SetBool(const std::string& key, bool value) override;
        void Save() override;
        void Load() override;

    private:
        std::unordered_map<std::string, std::string> m_data;
        std::mutex m_mutex;
        std::string m_filePath = "config.json";
    };

}}}
