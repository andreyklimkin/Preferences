//
// Created by andreyklimkin on 09.01.16.
//
#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include "classes.h"

using namespace std;

void ReadCarsData::Get_users(ifstream &file, vector<User>& users, Users_Knn& Users_par) {
    string s;
    int count = 0;
    vector<double> ed, age, gender;
    while(getline(file, s)) {
        if (count) {
            User *next = new User();
            sscanf(s.c_str(), "%d,%d,%d,%d,%d,%d", &next->id, &next->educ, &next->age, &next->gender,
                   &next->region, &next->cor_contr);
            --next->id;
            if (next->educ) {
                ed.push_back(next->educ);
            }
            if (next->age) {
                age.push_back(next->age);
            }
            if (next->gender) {
                gender.push_back(next->gender);
            }
            users.push_back(*next);
        }
        ++count;
    }
    sort(ed.begin(), ed.end());
    sort(age.begin(), age.end());
    sort(gender.begin(), gender.end());
    Users_par.Set_kof(ed.back() - ed[0] + 1, age.back() - age[0] + 1,
                      gender.back() - gender[0] + 1);
}

void ReadCarsData::Get_Items(ifstream &file, vector<Car>& cars, Cars_Knn& Cars_par) {
    string s;
    int count = 0;
    vector<double> type, tr, eng, fuel;
    while (getline(file, s)) {
        if (count) {
            Car *next = new Car();
            sscanf(s.c_str(), "%d,%d,%d,%lf,%lf", &next->id, &next->type, &next->transm,
                   &next->eng_cap, &next->fuel_cons);
            --next->id;
            cars.push_back(*next);

            type.push_back(next->type);
            tr.push_back(next->transm);
            eng.push_back(next->eng_cap);
            fuel.push_back(next->fuel_cons);
        }
        ++count;
    }
    sort(type.begin(), type.end());
    sort(tr.begin(), tr.end());
    sort(eng.begin(), eng.end());
    sort(fuel.begin(), fuel.end());
    Cars_par.Set_kof(type.back() - type[0] + 1, tr.back() - tr[0] + 1,
                     eng.back() - eng[0] + 1, fuel.back() - fuel[0] + 1);
}

bool PreparerDataCars::find_contradict(vector<Quest>& learn, Quest tmpl, int indx) {

    swap(tmpl.second, tmpl.first);

    for (int i = 0; i < indx; ++i) {
        if (learn[i] == tmpl) {
            learn.erase(learn.begin() + i);
            return true;
        }
    }

    return false;
}

void PreparerDataCars::clean_quest(vector<Quest>& learn) {

    for (int i = 0; i < learn.size(); ++i) {
        if (find_contradict(learn, learn[i], i)) {
            learn.erase(learn.begin() + i);
            i -= 2;
        }
    }

}

void PreparerDataCars::Divide_Pref_Data(ifstream &file, vector<Quest>& learn_quest, vector<Quest>& test_quest,
                      vector<Quest>& check_quest) {
    string s;
    getline(file, s);
    while (getline(file, s)) {
        int random_int;
        random_int = (rand() % 20);
        Quest *next = new Quest();
        sscanf(s.c_str(), "%d,%d,%d,%d", &next->id, &next->first, &next->second, &next->control);
        if (next->id > number_of_users) {
            number_of_users = next->id;
        }
        --next->id, --next->first, --next->second;
        if (random_int < 12) {
            learn_quest.push_back(*next);
        } else {
            if (random_int <= 14) {
                test_quest.push_back(*next);
            } else {
                check_quest.push_back(*next);
            }
        }
    }
    PreparerDataCars::clean_quest(learn_quest);
}

void User_Graph::Add_edge(int a, int b) {
    g[a].push_back(b);
    g_matrix[a][b] = 1;
}

double sqr(double a) {
    return sqrt(a * a);
}

double pow2(double a) {
    return a * a;
}

void User_Graph::Dfs(int v, vector<int>& used, int &flag, int need) {
    used[v] = 1;
    if (need == v) {
        flag = 1;
        return;
    }

    for (auto ver : g[v]) {
        if (!used[ver]) {
            Dfs(ver, used, flag, need);
        }
    }
}

void User_Graph:: IsBetter(int first, int second) {
    int flag = 0;
    vector<int> used(g.size(), 0);
    Dfs(first, used, flag, second);
    is_better[first][second] = used[second];
    //return flag;
}

double User_Graph:: Get_Dist(const User_Graph& second) const {

    double same = 0;
    double dif = 0;
    for (int i = 0; i < g.size(); ++i) {
        for (int j = i + 1; j < g.size(); ++j) {
            if (!(g_matrix[i][j] - second.g_matrix[i][j]) && g_matrix[i][j]) {
                ++same;
            } else {
                if (!(g_matrix[i][j] - second.g_matrix[j][i]) && g_matrix[i][j]) {
                    ++dif;
                }
            }
        }
    }

    double kof_same = -1;
    double kof_dif = 1;

    return kof_same * same + kof_dif * dif;
}

int Knn::Get_K() {
    return k;
}
void Knn::Change_K(int new_k) {
    k = new_k;
}

void Users_Knn::Change_weights(const vector<float>& new_w) {
    for(int i = 0; i < new_w.size(); ++i) {
        weight[i] = new_w[i];
    }
}

void Cars_Knn::Change_weights(const vector<float>& new_w) {
    for (int i = 0; i < new_w.size(); ++i) {
        weight[i] = new_w[i];
    }
}

double Users_Knn::Get_Users_Dist(const User& first, const User& second) {

    double res = 0;
    if(first.educ && second.educ) {
        //res += sqr((first.educ - second.educ) / kof_educ) * weight[0];
        res += pow2((first.educ - second.educ) / kof_educ) * weight[0];
    }

    if(first.age && second.age) {
        //res += sqr((first.age - second.age) / kof_age) * weight[1];
        res += pow2((first.age - second.age) / kof_age) * weight[1];
    }

    if(first.gender && second.gender) {
        //res += sqr((first.gender - second.gender) / kof_gender) * weight[2];
        res += pow2((first.gender - second.gender) / kof_gender) * weight[2];
    }

    if(first.region && second.region) {
        //res += sqr((first.region != second.region)) * weight[3];
        res += pow2((first.region != second.region)) * weight[3];

    }

    return sqrt(res);
}

/*vector<int> Users_Knn::K_nearest(vector<pair<int, int>> users) {
    sort(users.begin(), users.end());
    vector<int> k_near;
    for (int i = 0; i < k; ++i) {
        k_near.push_back(users[i].second);
    }
    return k_near;
}*/

double Cars_Knn::Get_Dist(const Car& first, const Car& second) {
    double res = 0;
    res += pow2((first.type - second.type) / kof_type) * weight[0];
    res += pow2((first.transm - second.transm) / kof_trans) * weight[1];
    res += pow2((first.eng_cap - second.eng_cap) / kof_eng_cap) * weight[2];
    res += pow2((first.fuel_cons - second.fuel_cons) / kof_fuel_cons) * weight[3] ;
    return sqrt(res);
}

