#include "game.h"

template <typename T>
Game<T>::Game(
                const std::map<Sides, std::vector<Position>>& startPosition,
                const std::map<Sides, std::vector<Position>>& finalPosition
        ) :
    actualSide(Sides::white),
    actualWin(Sides::none) {

    // поля которые необходимо заполнить, чтобы выиграть
    for (auto& sidesPositons : finalPosition) {
        for(auto& position : sidesPositons.second) {
            if (std::find(cellsEnd[sidesPositons.first].begin(), cellsEnd[sidesPositons.first].end(), position) != cellsEnd[sidesPositons.first].end())
                throw "Two final cell in one cell";
            cellsEnd[sidesPositons.first].push_back(position);
        }
        if (cellsEnd[sidesPositons.first].empty())
            throw "Empty final position";
    }

    // начальная расстановка + привзяка какой объект куда идет (для AI)
    for (auto& sidesPositons : startPosition) {
        auto endSellIt = cellsEnd[sidesPositons.first].begin();
        if (needDebug) std::cout <<  "endSellIt" << *endSellIt<<std::endl;
        auto lastEndSellIt = cellsEnd[sidesPositons.first].end();
        lastEndSellIt--;
        for(auto& position : sidesPositons.second) {
            if (cells.find(position) != cells.end())
                throw "Two obj in one cell";
            cells[position] = sidesPositons.first;

            targets[sidesPositons.first][*endSellIt] = position;
            if (needDebug) std::cout <<"Add cells" << position <<"->"<<cells[position]<< ":" 
                      <<*endSellIt  << "->" << targets[sidesPositons.first].at(*endSellIt)    <<std::endl;
            if (lastEndSellIt != endSellIt) {
                endSellIt++;
            }
        }
    }
}

template <typename T>
bool Game<T>::CheckFinal(const Sides& actualSide) {
    for(auto& cell : cellsEnd[actualSide]) {
        auto obj = cells.find(cell);
        if (obj == cells.end())
            return false;
        if(obj->second != actualSide)
            return false;
    }
    return true;
}

template <typename T>
bool Game<T>::CheckFinal() {
    if (CheckFinal(actualSide)) {
        actualWin = actualSide;
        return true;
    }
    return false;
}

template <typename T>
bool Game<T>::ChooseObj(const Position& from) {
    auto cellFrom = cells.find(from);

    if (cellFrom == cells.end()) {
        if (needDebug) std::cout << "ChooseObj fail. cellFrom is not find (end)" << std::endl;
        return false;
    }
    if (cellFrom->second != actualSide) {
        if (needDebug) std::cout << "ChooseObj fail actualSide(" << actualSide << ")!= obj Side("<<cellFrom->second << ")" << std::endl;
        return false;
    }
    if (needDebug) std::cout << "Choose obj:" << from <<std::endl;
    return true;
}

template <typename T>
bool Game<T>::DoStep(const Position& from, const Position& to) {

    auto cellFrom = cells.find(from);
    auto cellTo = cells.find(to);
    if (needDebug) std::cout << "DoStep " << from <<"->" << to <<std::endl;

    if (cellFrom == cells.end()) {
        if (needDebug) std::cout << "DoStep fail cellFrom is not Find" << std::endl;
        return false;
    }
    if ( cellFrom->second != actualSide) {
        if (needDebug) std::cout << "DoStep fail cellFrom is Empty" << std::endl;
        return false;
    }
    if (cellTo != cells.end() && cellTo->second != Sides::none) {
        if (needDebug) std::cout << "DoStep fail cellTo notEmpty" << std::endl;
        return false;
    }
    if (!actualObj.Step(from, to)) {
        if (needDebug) std::cout << "DoStep fail by step" << std::endl;
        return false;
    }
    if (needDebug) std::cout << "DoStep ok" << std::endl;

    auto it = std::find_if(targets[actualSide].begin(), targets[actualSide].end(),
                    [from](const std::pair<Position, Position>& mo) {return mo.second == from; });


    if (it == targets[actualSide].end())
        if (needDebug) std::cout<<"ERROR! target is not found!";
    else
        it->second = to;

    cells[to] = actualSide;
    cellFrom->second = Sides::none;
    CheckFinal();
    actualSide++;
    
    return true;
}



template <typename T>
std::pair<Position, Position> Game<T>::AIBestStep() {
    return AIBestStep(actualSide);
}
template <typename T>
std::pair<Position, Position> Game<T>::AIBestStep(const Sides& side) {
    // dummy AI
    std::pair<Position, Position> bestVariant;
    std::stringstream deb;

    Position choosenTarge;

    for (auto& target : targets[side]) {
        deb.clear();
        deb << "AI target for " << side << target.first << " from" << target.second;
        int lenBase = dist(target.second, target.first);
        deb << "="<<lenBase << ";";
        for (auto& step: actualObj.VariantsOfStep(target.second)) {
            deb << step;
            auto cellTo = cells.find(step);
            if(cellTo == cells.end() || cellTo->second == Sides::none) {
                bestVariant = {target.second, step};
                auto lenNew = dist(step, target.first);
                deb << "="<<lenNew << ";";
                if(lenNew < lenBase) {
                    deb << std::endl;
                    if (needDebug) std::cout << deb.str();
                    if (needDebug) std::cout << "AI best " << bestVariant.first << "->" << bestVariant.second << std::endl;
                    return bestVariant;
                }
            } else {
                deb << "Not Empty";
            }
            deb << " ";
        }
        deb << std::endl;
        if (needDebug) std::cout << deb.str() ;
    }

    return AIRandomStep(side);
}

template <typename T>
std::pair<Position, Position> Game<T>::AIRandomStep(const Sides& side) {
    for (auto& target : targets[side]) {
        for (auto& step: actualObj.VariantsOfStep(target.second)) {

            auto cellTo = cells.find(step);
            if(cellTo == cells.end() || cellTo->second == Sides::none) {
                return {target.second, step};
            }
        }
    }
    throw "No one step";
}
