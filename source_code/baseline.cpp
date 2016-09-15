//
// Created by andreyklimkin on 10.04.16.
//
#include "baseline.h"

long long Baseline_Calc:: get_deviation(const vector<User_Graph> &g, const qType &questions) {
    long long count = 0;

    for (auto q : questions) {
        if (g[q.id].is_better[q.first][q.second] && !g[q.id].is_better[q.second][q.first]) {
            continue;
        }

        if (g[q.id].is_better[q.second][q.first] && !g[q.id].is_better[q.first][q.second]) {
            ++count;
            continue;
        }

        if (rand() % 2) {
            ++count;
        }
    }

    return count;
}
