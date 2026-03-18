#pragma once
#include <vector>

struct BallSave
{
    float x, y;
    int   r, g, b, a;
};

struct BilliardSave
{
    std::vector<BallSave> balls;
    BallSave              cueBall;
    bool                  hasCueBall;
    int                   score;
};