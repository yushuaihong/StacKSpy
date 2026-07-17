#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Plugins {

    class IPlugin {
    public:
        virtual ~IPlugin() = default;
        virtual std::string GetName() const = 0;
        virtual std::string GetVersion() const = 0;
        virtual void Initialize() = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void Destroy() = 0;
    };

}}}
