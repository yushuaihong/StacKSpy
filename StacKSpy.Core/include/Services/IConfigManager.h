#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Services {

    class IConfigManager {
    public:
        virtual ~IConfigManager() = default;
        virtual std::string GetString(const std::string& key, const std::string& defaultValue = "") = 0;
        virtual int GetInt(const std::string& key, int defaultValue = 0) = 0;
        virtual double GetDouble(const std::string& key, double defaultValue = 0.0) = 0;
        virtual bool GetBool(const std::string& key, bool defaultValue = false) = 0;
        virtual void SetString(const std::string& key, const std::string& value) = 0;
        virtual void SetInt(const std::string& key, int value) = 0;
        virtual void SetDouble(const std::string& key, double value) = 0;
        virtual void SetBool(const std::string& key, bool value) = 0;
        virtual void Save() = 0;
        virtual void Load() = 0;
    };

}}}
