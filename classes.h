//
// Created by andreyklimkin on 09.01.16.
//

#include <bits/stdc++.h>

using namespace std;

#pragma once

#ifndef CARS_PREFERENCES_CLASSES_H
#define CARS_PREFERENCES_CLASSES_H
const int NUMBER_OF_CARS = 10;
extern int number_of_users;

class Quest{
public:
    int id;
    int first;
    int second;
    int control;
    Quest() {

    }
    Quest(int a, int b, int c, int d): id(a), first(b), second(c), control(d) {

    }

    bool operator==(Quest A) {
        if (this->id == A.id && this->first == A.first && this->second == A.second) {
            return true;
        } else {
            return false;
        }
    }

};

class Car {
public:
    int id;
    int type;
    int transm;
    double eng_cap;
    double fuel_cons;
    Car() {

    }
    Car(int a, int b, int c, int d): type(a), transm(b), eng_cap(c), fuel_cons(d) {

    }
};

class User{
public:
    int id;
    int educ;
    int age;
    int gender;
    int region;
    int cor_contr;

    User() {

    }

    User(int a, int b, int c, int d, int cc): educ(a), age(b), gender(c), region(d), cor_contr(cc)  {

    }

};

class User_Graph {
public:
    int id;
    vector<vector<int>> g_matrix;
    vector<vector<int>> g;
    vector<vector<int>> is_better;

    User_Graph() {
        g_matrix.resize(NUMBER_OF_CARS, vector<int> (NUMBER_OF_CARS, 0));
    }

    double Get_Dist(const User_Graph& second) const;

    void Add_edge(int a, int b);

    void Dfs(int v, vector<int>& used, int &flag, int need);

    void IsBetter(int first, int second);
};

class Knn {
protected:
    vector<float> weight;
    int k;
public:
    Knn() {
        weight.resize(4, 0);
    }
    int Get_K();

    void Change_K(int new_k);

    void Show_w() {
        for(auto elem : weight) {
            printf("%.2f ", elem);
            //cout << elem << " ";
        }
        cout << endl;
    }
};

class Users_Knn:public Knn {
private:
    float kof_educ;
    float kof_age;
    float kof_gender;
public:

    double answ_kof;

    Users_Knn() {
        weight.resize(4, 0);
    }

    void Set_kof(float educ, float age, float gender) {
        kof_educ = educ, kof_age = age, kof_gender = gender;
    }

    double Get_Users_Dist(const User& first, const User& second);

    //vector<int> K_nearest(vector<pair<int, int>> users);

    void Change_weights(const vector<float>& new_w);

};


class Cars_Knn: public Knn {
private:
    float kof_type;
    float kof_trans;
    float kof_eng_cap;
    float kof_fuel_cons;
public:

    double answ_kof;

    void Set_kof(float type, float trans, float cap, float fuel) {
        kof_type = type, kof_trans = trans, kof_eng_cap = cap, kof_fuel_cons = fuel;
    }

    Cars_Knn() {
        weight.resize(4, 0);
    }

    double Get_Dist(const Car& first, const Car& second);

    void Change_weights(const vector<float>& new_w);
};


class ReadCarsData {
public:
    void Get_users(ifstream &file, vector<User> &users, Users_Knn &Users_par);
    void Get_Items(ifstream &file, vector<Car> &cars, Cars_Knn &Cars_par);
};


class PreparerDataCars {
public:
    static bool find_contradict(vector<Quest> &learn, Quest tmpl, int indx);

    static void clean_quest(vector<Quest> &learn);

    static void Divide_Pref_Data(ifstream &file, vector<Quest> &learn_quest, vector<Quest> &test_quest,
                          vector<Quest> &check_quest);
};

#endif //CARS_PREFERENCES_CLASSES_H
