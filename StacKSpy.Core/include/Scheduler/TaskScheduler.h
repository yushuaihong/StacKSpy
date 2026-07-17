#pragma once
#include "Scheduler/ITaskScheduler.h"
#include <thread>
#include <atomic>
#include <unordered_map>
#include <mutex>

namespace StacKSpy { namespace Core { namespace Scheduler {

    struct ScheduledTask {
        std::string Id;
        TaskCallback Callback;
        std::chrono::seconds Interval{0};
        int DailyHour = -1;
        int DailyMinute = -1;
        std::atomic<bool> Running{false};
        std::thread Thread;
    };

    class TaskScheduler : public ITaskScheduler {
    public:
        TaskScheduler();
        ~TaskScheduler();

        void ScheduleInterval(const std::string& taskId, TaskCallback callback,
                              std::chrono::seconds interval) override;
        void ScheduleDaily(const std::string& taskId, TaskCallback callback,
                           int hour, int minute) override;
        void Cancel(const std::string& taskId) override;
        void Start() override;
        void Stop() override;

    private:
        std::unordered_map<std::string, std::shared_ptr<ScheduledTask>> m_tasks;
        std::mutex m_mutex;
        std::atomic<bool> m_running{false};
    };

}}}
