#pragma once
#include <string>
#include "Version.h"

namespace StacKSpy { namespace Core { namespace Plugins {

    class IPlugin {
    public:
        virtual ~IPlugin() = default;
        virtual std::string GetName() const = 0;
        virtual std::string GetVersion() const { return STACKSPY_VERSION_STRING; }
        virtual void Initialize() = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void Destroy() = 0;
    };

}}}
