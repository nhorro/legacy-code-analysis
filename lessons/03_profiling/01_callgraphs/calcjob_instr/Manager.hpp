#pragma once
#include "Worker.hpp"
#include "Reporter.hpp"

class Manager {
public:
    Manager() {
        worker.onJobDone([this](const CalcJob& job) {
            reporter.report(job);
        });
    }

    void start() {
        worker.start();
    }

    void stop() {
        worker.stopWorker();
    }

    void submitJob(const CalcJob& job) {
        worker.enqueueJob(job);
    }

private:
    Worker worker;
    Reporter reporter;
};
