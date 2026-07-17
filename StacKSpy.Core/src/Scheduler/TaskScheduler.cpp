#include "Scheduler/TaskScheduler.h"
#include <chrono>

namespace StacKSpy { namespace Core { namespace Scheduler {

    TaskScheduler::TaskScheduler() {}
    TaskScheduler::~TaskScheduler() { Stop(); }

    void TaskScheduler::ScheduleInterval(const std::string& taskId, TaskCallback callback,
                                          std::chrono::seconds interval) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto task = std::make_shared<ScheduledTask>();
        task->Id = taskId;
        task->Callback = callback;
        task->Interval = interval;
        task->Running = true;
        task->Thread = std::thread([task]() {
            while (task->Running) {
                std::this_thread::sleep_for(task->Interval);
                if (task->Running && task->Callback) task->Callback();
            }
        });
        m_tasks[taskId] = task;
    }

    void TaskScheduler::ScheduleDaily(const std::string& taskId, TaskCallback callback,
                                       int hour, int minute) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto task = std::make_shared<ScheduledTask>();
        task->Id = taskId;
        task->Callback = callback;
        task->DailyHour = hour;
        task->DailyMinute = minute;
        task->Running = true;
        task->Thread = std::thread([task]() {
            while (task->Running) {
                auto now = std::chrono::system_clock::now();
                auto time = std::chrono::system_clock::to_time_t(now);
                struct tm tmBuf = {};
                localtime_s(&tmBuf, &time);
                if (tmBuf.tm_hour == task->DailyHour && tmBuf.tm_min == task->DailyMinute && tmBuf.tm_sec == 0) {
                    if (task->Callback) task->Callback();
                    std::this_thread::sleep_for(std::chrono::seconds(61));
                } else {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
        });
        m_tasks[taskId] = task;
    }

    void TaskScheduler::Cancel(const std::string& taskId) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_tasks.find(taskId);
        if (it != m_tasks.end()) {
            it->second->Running = false;
            if (it->second->Thread.joinable()) it->second->Thread.join();
            m_tasks.erase(it);
        }
    }

    void TaskScheduler::Start() { m_running = true; }

    void TaskScheduler::Stop() {
        m_running = false;
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& [id, task] : m_tasks) {
            task->Running = false;
            if (task->Thread.joinable()) task->Thread.join();
        }
        m_tasks.clear();
    }

}}}
