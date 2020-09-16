#include "baseobj.h"

bool BaseObj::Step(const Position& from, const Position& to) {
    if ((to <= MaxValue && to > 0) &&
            permissibleStep(from, to)) {
        return true;
    }
    return false;
}