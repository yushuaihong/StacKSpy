#pragma once
#include <functional>
#include <string>
#include <chrono>

namespace StacKSpy { namespace Core { namespace Scheduler {

    using TaskCallback = std::function<void()>;

    class ITaskScheduler {
    public:
        virtual ~ITaskScheduler() = default;
        virtual void ScheduleInterval(const std::string& taskId, TaskCallback callback,
                                      std::chrono::seconds interval) = 0;
        virtual void ScheduleDaily(const std::string& taskId, TaskCallback callback,
                                   int hour, int minute) = 0;
        virtual void Cancel(const std::string& taskId) = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

}}}
