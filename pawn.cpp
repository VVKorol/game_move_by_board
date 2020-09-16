#include "pawn.h"

std::vector<Position> Pawn::VariantsOfStep(const Position& pos) {
    std::vector<Position> res;
    if(pos.first > 1)
        res.push_back(Position(pos.first-1, pos.second));
    if(pos.second > 1)
        res.push_back(Position(pos.first, pos.second-1));
    if(pos.first < MaxValue.first)
        res.push_back(Position(pos.first + 1, pos.second));
    if(pos.second < MaxValue.second)
        res.push_back(Position(pos.first, pos.second+1));
    return res;
}

bool Pawn::permissibleStep(const Position& p1, const Position& p2) {
    int def = dist(p1, p2);
    return def == 1;
    //return ((def.first == 0 && (def.second == -1 || def.second == 1 )) ||
    //    (def.second == 0 && (def.first == -1 || def.first == 1 )));
}
