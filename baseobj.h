#pragma once
#include "meta.h"
#include <vector>

class BaseObj {
protected:
    std::vector<Position> virtual VariantsOfStep(const Position&) = 0;
    bool virtual permissibleStep(const Position& p1, const Position& p2)  = 0;
    BaseObj() {}

public:
    bool Step(const Position& from, const Position& to);
};
