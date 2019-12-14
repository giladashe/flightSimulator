//
// Created by shaiyis on 12/13/19.
//

#include "VarData.h"

VarData::VarData(double value, const string &progStr, const string &simStr, int bind) :
value(value), progStr(progStr),simStr(simStr), bind(bind) {}

VarData::~VarData() {
}
