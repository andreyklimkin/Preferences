#include <bits/stdc++.h>
#include "classes.h"
#include "Trivial.h"

using namespace std;
using qType = vector<Quest>;
using uType = vector<User>;
using cType = vector<Car>;
using ansType = vector<Answer>;

const int MIN_K = 2;
const int MAX_K = 100;

vector<Answer> makeAnsws(const uType& users, const cType& cars, const qType& quest) {
    vector<Answer> ans(quest.size());
    for(int i = 0; i < quest.size(); ++i) {
        ans[i].initUser(users[quest[i].id]);
        ans[i].initCar1(cars[quest[i].first]);
        ans[i].initCar2(cars[quest[i].second]);
    }

    return ans;
}

vector<AnsDist> getKNearest(int k, const Answer& respAns, const ansType &lrn) {
    vector<AnsDist> kNear;
    for(auto elem : lrn) {
        kNear.push_back({elem, respAns.getDist(elem)});
    }

    sort(kNear.begin(), kNear.end(), [](AnsDist A, AnsDist B) {return A.dist < B.dist;});
    kNear.erase(kNear.begin() + k, kNear.end());
    //printf("%.5lf %.5lf\n", kNear[0].dist, kNear[1].dist);
    return kNear;
}

Answer reverseCars(Answer A) {
    A.swapCars();
    return A;
}

int calcWrongAns(int k, const ansType& lrn, const ansType& test) {

    int delta = 0;

    for (int i = 0; i < test.size(); ++i) {
        vector<AnsDist> kNearestDist = getKNearest(k, test[i], lrn);
        vector<AnsDist> kNearestDistRev = getKNearest(k, reverseCars(test[i]), lrn);
        double forLeft = 0;
        double forRight = 0;

        for (int j = 0; j < kNearestDist.size(); ++j) {
            forLeft -= kNearestDist[j].dist;
            forRight -= kNearestDistRev[j].dist;
        }

        if ((forRight >= forLeft)) {
            ++delta;
        }
    }

    return delta;
}

int findBestK(const ansType& lrn, const ansType& test) {

    int best = 0;
    int minDelta = test.size();

    for(int k = MIN_K; k < MAX_K; ++k) {

        int curDelta = calcWrongAns(k, lrn, test);

        //cout << curDelta << endl;

        if(curDelta < minDelta) {
            minDelta = curDelta;
            best = k;
        }
    }

    cout << minDelta << endl;
    return best;
}

void TrivialSolve(const vector<User>& users, const vector<Car> &cars, const qType& learn, qType& check, qType& test) {
    vector<Answer> lrn, chck, tst;
    lrn = makeAnsws(users, cars, learn);
    chck = makeAnsws(users, cars, check);
    tst = makeAnsws(users, cars, test);
    int bestK;
    bestK = findBestK(lrn, chck);
    cout << "Best K = " << bestK << endl;
    cout << "Wrong Answers = " << calcWrongAns(bestK, lrn, tst) << " of " << tst.size() << endl;
}


