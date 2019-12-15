//
// Created by shaiyis on 12/13/19.
//

#ifndef FLIGHTSIMULATOR_VARDATA_H
#define FLIGHTSIMULATOR_VARDATA_H

#include <string>
using namespace std;



class VarData {
    double _value;
    string _progStr, _simStr;
    int _bind; // 1 if I influence, 0 else
public:
    VarData(double value, const string &progStr, const string &simStr, int bind);
    virtual ~VarData();
};


#endif //FLIGHTSIMULATOR_VARDATA_H
