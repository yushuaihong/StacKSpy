#pragma once
#include "Kernel/IEventBus.h"
#include <unordered_map>
#include <mutex>

namespace StacKSpy { namespace Core { namespace Kernel {

    class EventBus : public IEventBus {
    public:
        void Subscribe(const std::string& eventName, EventHandler handler) override;
        void SubscribeHandler(const std::string& eventName, IEventHandler* handler) override;
        void Unsubscribe(const std::string& eventName) override;
        void Publish(const std::string& eventName, const EventData& data = {}) override;

    private:
        std::unordered_map<std::string, EventHandler> m_handlers;
        std::unordered_map<std::string, std::vector<IEventHandler*>> m_eventHandlers;
        std::mutex m_mutex;
    };

}}}
