//
// Created by shaiyis on 12/16/19.
//

#include "Variables.h"

unordered_map<string, VarData*> Variables::getMap() {
    return this->progMap;
}
Variables* Variables::