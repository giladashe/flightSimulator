//
// Created by shaiyis on 12/13/19.
//

#include "VarData.h"

using namespace std;
VarData::VarData(double value, const string &progStr, const string &simStr, int bind) :
_value(value), _progStr(progStr), _simStr(simStr), _bind(bind) {}

VarData::~VarData() {
}

double VarData::getValue() const {
    return _value;
}

void VarData::setValue(double value) {
    _value = value;
}

const string &VarData::getProgStr() const {
    return _progStr;
}

const string &VarData::getSimStr() const {
    return _simStr;
}

int VarData::getBind() const {
    return _bind;
}

void VarData::setProgStr(const string &progStr) {
    _progStr = progStr;
}

void VarData::setSimStr(const string &simStr) {
    _simStr = simStr;
}

void VarData::setBind(int bind) {
    _bind = bind;
}
