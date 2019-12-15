//
// Created by shaiyis on 12/13/19.
//

#include "VarData.h"

VarData::VarData(double value, const string &progStr, const string &simStr, int bind) :
_value(value), _progStr(progStr), _simStr(simStr), _bind(bind) {}

VarData::~VarData() {
}
