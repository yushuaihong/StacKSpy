#include "Kernel/EventBus.h"

namespace StacKSpy { namespace Core { namespace Kernel {

    void EventBus::Subscribe(const std::string& eventName, EventHandler handler) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_handlers[eventName] = handler;
    }

    void EventBus::SubscribeHandler(const std::string& eventName, IEventHandler* handler) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_eventHandlers[eventName].push_back(handler);
    }

    void EventBus::Unsubscribe(const std::string& eventName) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_handlers.erase(eventName);
        m_eventHandlers.erase(eventName);
    }

    void EventBus::Publish(const std::string& eventName, const EventData& data) {
        EventHandler handler;
        std::vector<IEventHandler*> handlers;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_handlers.find(eventName);
            if (it != m_handlers.end()) handler = it->second;
            auto it2 = m_eventHandlers.find(eventName);
            if (it2 != m_eventHandlers.end()) handlers = it2->second;
        }
        if (handler) handler(data);
        for (auto* h : handlers) {
            if (h) h->OnEvent(eventName, data);
        }
    }

}}}
