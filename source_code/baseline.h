//
// Created by andreyklimkin on 10.04.16.
//

#include "classes.h"

#pragma once

#ifndef CARS_PREFERENCES_BASELINE_H
#define CARS_PREFERENCES_BASELINE_H

using qType = vector<Quest>;

class Baseline_Calc {
public:
    long long get_deviation(const vector<User_Graph> &g,const qType& questions);
};

#endif //CARS_PREFERENCES_BASELINE_H
