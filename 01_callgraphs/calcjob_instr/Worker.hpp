#pragma once
#include "CalcJob.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>

class Worker {
public:
    using Callback = std::function<void(const CalcJob&)>;

    Worker() : stop(false) {}

    void start() {
        workerThread = std::thread([this]() { this->run(); });
    }

    void stopWorker() {
        {
            std::unique_lock lock(mtx);
            stop = true;
            cv.notify_all();
        }
        workerThread.join();
    }

    void enqueueJob(const CalcJob& job) {
        std::unique_lock lock(mtx);
        queue.push(job);
        cv.notify_one();
    }

    void onJobDone(Callback cb) {
        jobDoneCallback = cb;
    }

private:
    void run() {
        while (true) {
            CalcJob job;
            {
                std::unique_lock lock(mtx);
                cv.wait(lock, [this]() { return !queue.empty() || stop; });
                if (stop && queue.empty()) break;
                job = queue.front(); queue.pop();
            }
            process(job);
            if (jobDoneCallback) {
                jobDoneCallback(job);
            }
        }
    }

    void process(CalcJob& job) {
        switch (job.op) {
            case CalcJob::Op::Add: job.result = job.a + job.b; break;
            case CalcJob::Op::Sub: job.result = job.a - job.b; break;
            case CalcJob::Op::Mul: job.result = job.a * job.b; break;
            case CalcJob::Op::Div: job.result = job.b != 0 ? job.a / job.b : 0; break;
        }
    }

    std::queue<CalcJob> queue;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread workerThread;
    bool stop;

    Callback jobDoneCallback;
};
