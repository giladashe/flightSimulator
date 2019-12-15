//
// Created by shaiyis on 12/16/19.
//

#ifndef FLIGHTSIMULATOR_VARIABLES_H
#define FLIGHTSIMULATOR_VARIABLES_H


// singleton ProgramObjMap so all the program has access
class Variables {
    Variables(); // private constructor
    unordered_map<string, VarData*> progMap;
    static Variables* instance;
public:
    static Variables* getInstance();
    unordered_map<string, VarData*> getMap();
};



#endif //FLIGHTSIMULATOR_VARIABLES_H
