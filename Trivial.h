#include "classes.h"
#include <bits/stdc++.h>


using qType = vector<Quest>;
using ll = long long;

class Answer{
public:
    int educ;
    int age;
    int gender;
    int region;

    int type1;
    int transm1;
    double engCap1;
    double fuelCons1;

    int type2;
    int transm2;
    double engCap2;
    double fuelCons2;

    void initUser(const User& U) {
        educ = U.educ, age = U.age, gender = U.gender, region = U.region;
    }

    void initCar1(const Car& C) {
        type1 = C.type, transm1 = C.transm, engCap1 = C.eng_cap, fuelCons1 = C.fuel_cons;
    }

    void initCar2(const Car& C) {
        type2 = C.type, transm2 = C.transm, engCap2 = C.eng_cap, fuelCons2 = C.fuel_cons;
    }

    void swapCars() {
        swap(type1, type2), swap(transm1, transm2), swap(engCap1, engCap2), swap(fuelCons1, fuelCons2);
    }

    double sqr(double a) const {
        return a * a;
    }

    double getDist(const Answer& A) const {
        double res = 0;
        res += sqrt( sqr(A.educ - educ) + sqr(A.gender - gender) + sqr(A.age - age) + sqr(A.region != region) );
        res += sqrt( sqr(A.type1 - type1) + sqr(A.transm1 - transm1) + sqr(A.engCap1 - engCap1) + sqr(A.fuelCons1 - fuelCons1) );
        res += sqrt( sqr(A.type2 - type2) + sqr(A.transm2 - transm2) + sqr(A.engCap2 - engCap2) + sqr(A.fuelCons2 - fuelCons2) );
        return res;
    }
};

struct AnsDist {
public:
    Answer ans;
    double dist;
};

void TrivialSolve(const vector<User> &users, const vector<Car> &cars, const qType& learn, qType& check, qType& test);
