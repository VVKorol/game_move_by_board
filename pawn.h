#pragma once

#include <map>
#include "baseobj.h"

class Pawn : public BaseObj {
public:
    Pawn() {}
    std::vector<Position> VariantsOfStep(const Position& pos) override;
    bool permissibleStep(const Position& p1, const Position& p2) override;
};

