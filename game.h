#pragma once
#include <algorithm>
#include <sstream>

#include "pawn.h"

// #include <unordered_map>

// для разных фигур в игре заменить Sides на Sides + enumType или ссылкой на объект.
template <typename Obj>
class Game {
    Obj   actualObj;
    Sides actualSide;
    std::map<Position, Sides> cells; 
    std::map<Sides, std::vector<Position>> cellsEnd;
    std::map<Sides, std::map<Position, Position>> targets;


public:
    std::map<Position, Sides>& Cells() {return cells;}
    Game(const std::map<Sides, std::vector<Position>>& startPosition, const std::map<Sides, std::vector<Position>>& finalPosition);
    bool CheckFinal(const Sides& actualSide);
    bool DoStep(const Position& from, const Position& to);
    bool ChooseObj(const Position& from);
    std::pair<Position, Position> AIBestStep();
    std::pair<Position, Position> AIBestStep(const Sides& side);
    std::pair<Position, Position> AIRandomStep(const Sides& side);
};

template class Game<Pawn>;

