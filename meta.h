#pragma once

#include <utility>
#include <iostream>
using Picture = int;

using Coord1D = int;
using Coord2D = std::pair<Coord1D, Coord1D>;

using Position = Coord2D;
bool operator< (const Position& A, const Position& B);
bool operator< (const Position& A, const int& B);

bool operator> (const Position& A, const Position& B);
bool operator<= (const Position& A, const int& B);

bool operator>= (const Position& A, const Position& B);
bool operator== (const Position& A, const int& B);
Position operator- (const Position& A, const Position& B);
bool operator> (const Position& A, const int& B);

int dist(const Position& A, const Position& B);

std::ostream& operator<< (std::ostream& os, const Position& A);

enum class Sides : unsigned {
    none = 0,
    white = 1,
    black = 2
};

static constexpr int BOARD_SIZE = 8;
static constexpr int NUM_SIDES = 3;
static constexpr bool needDebug = true;

Sides operator++(Sides& side);
Sides operator++(Sides& side, int);
static constexpr Position MaxValue (BOARD_SIZE, BOARD_SIZE);
std::ostream& operator<< (std::ostream& os, const Sides& A);
