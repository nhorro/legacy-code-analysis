#include "Manager.hpp"

#include "instrumentation.hpp"

int main() {
    printf(">>> main started\n");
    Manager mgr;
    mgr.start();

    mgr.submitJob({2.0f, 3.0f, CalcJob::Op::Add});
    mgr.submitJob({10.0f, 5.0f, CalcJob::Op::Sub});
    mgr.submitJob({3.0f, 4.0f, CalcJob::Op::Mul});
    mgr.submitJob({20.0f, 4.0f, CalcJob::Op::Div});

    std::this_thread::sleep_for(std::chrono::seconds(1));
    mgr.stop();
    return 0;
}
