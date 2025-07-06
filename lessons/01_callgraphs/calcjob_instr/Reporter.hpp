#pragma once
#include "CalcJob.hpp"
#include <iostream>

class Reporter {
public:
    void report(const CalcJob& job) {
        std::cout << "Job done: " << job.a << " "
                  << opToStr(job.op) << " " << job.b
                  << " = " << job.result << std::endl;
    }

private:
    const char* opToStr(CalcJob::Op op) {
        switch(op) {
            case CalcJob::Op::Add: return "+";
            case CalcJob::Op::Sub: return "-";
            case CalcJob::Op::Mul: return "*";
            case CalcJob::Op::Div: return "/";
        }
        return "?";
    }
};
