#pragma once

struct CalcJob {
    float a, b;
    enum class Op { Add, Sub, Mul, Div } op;
    float result = 0.0f;
};
