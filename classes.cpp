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

void User_Graph::Add_edge(int a, int b) {
    g[a].push_back(b);
    g_matrix[a][b] = 1;
}

double sqr(double a) {
    return sqrt(a * a);
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

void Users_Knn::Change_weights(const vector<int>& new_w) {
    for(int i = 0; i < new_w.size(); ++i) {
        weight[i] = new_w[i];
    }
}

void Cars_Knn::Change_weights(const vector<int>& new_w) {
    for(int i = 0; i < new_w.size(); ++i) {
        weight[i] = new_w[i];
    }
}

double Users_Knn::Get_Users_Dist(const User& first, const User& second) {

    double res = 0;
    if(first.educ && second.educ) {
        res += sqr((first.educ - second.educ) / kof_educ) * weight[0];
    }

    if(first.age && second.age) {
        res += sqr((first.age - second.age) / kof_age) * weight[1];
    }

    if(first.gender && second.gender) {
        res += sqr((first.gender - second.gender) / kof_gender) * weight[2];
    }

    if(first.region && second.region) {
        res += sqr((first.region != second.region)) * weight[3];
    }

    return res;
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
    res += sqr((first.type - second.type) / kof_type) * weight[0];
    res += sqr((first.transm - second.transm) / kof_trans) * weight[1];
    res += sqr((first.eng_cap - second.eng_cap) / kof_eng_cap) * weight[2];
    res += sqr((first.fuel_cons - second.fuel_cons) / kof_fuel_cons) * weight[3] ;
    return res;
}

