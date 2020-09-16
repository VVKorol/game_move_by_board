#include "meta.h"


bool operator< (const Position& A, const Position& B) {
    return A.first < B.first && A.second < B.second;
}
bool operator< (const Position& A, const int& B) {
    return A.first < B && A.second < B;
}

bool operator<= (const Position& A, const int& B) {
    return A.first <= B && A.second <= B;
}

bool operator>= (const Position& A, const Position& B) {
    return A.first >= B.first && A.second >= B.second;
}
bool operator> (const Position& A, const Position& B) {
    return A.first > B.first && A.second > B.second;
}
bool operator== (const Position& A, const int& B) {
    return A.first == B && A.second == B;
}
int dist(const Position& A, const Position& B) {
    return std::abs(A.first - B.first) + std::abs(A.second - B.second);
}

Position operator- (const Position& A, const Position& B) {
    return Position(A.first - B.first, A.second - B.second);
}
bool operator> (const Position& A, const int& B) {
    return A.first > B || A.second > B;
}

Sides operator++(Sides& side) {
    side = static_cast<Sides>((static_cast<int>(side) + 1) % NUM_SIDES);
    if (side==Sides::none)
        ++side;
    return side;
}
Sides operator++(Sides& side, int) {
   Sides res = side;
   ++side;
   return res;
}



std::ostream& operator<< (std::ostream& os, const Position& A) {
    os << "(" << A.first << ", " 
              << A.second << ")";
    return os;
}


std::ostream& operator<< (std::ostream& os, const Sides& A) {
    os << (
          A == Sides::black ? "black" :
          A == Sides::white ? "white" : 
          A == Sides::none ? "none" :
          "unknown");
    return os;
}