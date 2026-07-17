#pragma once
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

namespace StacKSpy { namespace Core { namespace Kernel {

    // 事件数据
    using EventData = std::unordered_map<std::string, std::string>;
    using EventHandler = std::function<void(const EventData&)>;

    // 事件处理器接口（纯虚函数，C++/CLI 可通过原生类实现）
    class IEventHandler {
    public:
        virtual ~IEventHandler() = default;
        virtual void OnEvent(const std::string& eventName, const EventData& data) = 0;
    };

    class IEventBus {
    public:
        virtual ~IEventBus() = default;
        // 原有：std::function 订阅（Core 内部使用）
        virtual void Subscribe(const std::string& eventName, EventHandler handler) = 0;
        // 新增：接口指针订阅（Bridge 层使用，避开 std::function 调用约定问题）
        virtual void SubscribeHandler(const std::string& eventName, IEventHandler* handler) = 0;
        virtual void Unsubscribe(const std::string& eventName) = 0;
        virtual void Publish(const std::string& eventName, const EventData& data = {}) = 0;
    };

}}}
