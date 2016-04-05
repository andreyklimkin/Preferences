#include "classes.h"
#include <bits/stdc++.h>
#include "Trivial.h"

using namespace std;

int number_of_users = 0;
const int MAX_K_USER = 20;
const int MIN_K_USER = 2;
const int MAX_K_CAR = 10;
const int MIN_K_CAR = 2;
const int SIGMA = 3;

double Gaus(double d) {
    return exp(-(d * d) / (2 * SIGMA * SIGMA));
}

void Get_users(ifstream &file, vector<User>& users, Users_Knn& Users_par) {
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
void Get_Items(ifstream &file, vector<Car>& cars, Cars_Knn& Cars_par) {
    string s;
    int count = 0;
    vector<double> type, tr, eng, fuel;
    while(getline(file, s)) {
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

bool find_contradict(vector<Quest>& learn, Quest tmpl, int indx) {

    swap(tmpl.second, tmpl.first);

    for (int i = 0; i < indx; ++i) {
        if (learn[i] == tmpl) {
            learn.erase(learn.begin() + i);
            return true;
        }
    }

    return false;
}

void clean_quest(vector<Quest>& learn) {

    for (int i = 0; i < learn.size(); ++i) {
        if (find_contradict(learn, learn[i], i)) {
            learn.erase(learn.begin() + i);
            i -= 2;
        }
    }

}

void Divide_Pref_Data(ifstream &file, vector<Quest>& learn_quest, vector<Quest>& test_quest,
                      vector<Quest>& check_quest) {
    string s;
    getline(file, s);
    while(getline(file, s)) {
        int random_int;
        random_int = (rand() % 20);
        Quest* next = new Quest();
        sscanf(s.c_str(), "%d,%d,%d,%d", &next->id, &next->first, &next->second, &next->control);
        if(next->id > number_of_users) {
            number_of_users = next->id;
        }
        --next->id, --next->first, --next->second;
        if(random_int < 16) {
            learn_quest.push_back(*next);
        } else {
            if(random_int <= 17) {
                test_quest.push_back(*next);
            } else {
                check_quest.push_back(*next);
            }
        }
    }
    clean_quest(learn_quest);
}

vector<User_Graph> Make_User_Graph(const vector<Quest>& learn) {
    vector<set<pair<int, int>>> s(number_of_users);
    vector<User_Graph> res_gr;
    res_gr.resize(number_of_users);
    for(int i = 0; i < number_of_users; ++i) {
        res_gr[i].g.resize(NUMBER_OF_CARS);
    }

    for(int i = 0; i < learn.size(); ++i) {
        int user_id = learn[i].id;
        int first = learn[i].first;
        int second = learn[i].second;
        if(s[user_id].find({first, second}) == s[user_id].end()) {
            res_gr[user_id].Add_edge(first, second);
            s[user_id].insert({first, second});
        }
    }

    return res_gr;
}

void Calc_UsersGr_Relations(vector<User_Graph> &G) {

    for (int usr = 0; usr < G.size(); ++usr) {
        G[usr].is_better.resize(G[usr].g.size());
        for (int i = 0; i < G[usr].g.size(); ++i) {
            G[usr].is_better[i].resize(G[i].g.size());
            for (int j = 0; j < G[i].g.size(); ++j) {
                if (i == j) {
                    G[usr].is_better[i][j] = 0;
                } else {
                    G[usr].IsBetter(i, j);
                }
            }
        }
    }
}

void count_cars_dist(vector<vector<pair<double, int>>> &cars_dist,
                     const vector<Car>& cars, Cars_Knn& Cars_par) {
    for(int i = 0; i < cars.size(); ++i) {
        cars_dist[i].resize(0);
        for(int j = 0; j < cars.size(); ++j) {
            if (j == i) {
                continue;
            } else {
                cars_dist[i].push_back({Cars_par.Get_Dist(cars[i], cars[j]), j});
            }
        }
    }

    for(int i = 0; i < cars.size(); ++i) {
        sort(cars_dist[i].begin(), cars_dist[i].end());
    }

}


void count_users_dist(vector<vector<pair<double, int>>> &users_dist, const vector<vector<double>> &answ_dist,
                      const vector<User>& users, Users_Knn& Users_par) {

    for(int i = 0; i < users.size(); ++i) {
        users_dist[i].resize(0);
        for(int j = 0; j < users.size(); ++j) {
            if (j == i) {
                continue;
            } else {
                users_dist[i].push_back({Users_par.Get_Users_Dist(users[i], users[j]) + answ_dist[i][j], j});
                //users_dist[i].push_back({answ_dist[i][j], j});
                //users_dist[i].push_back({Users_par.Get_Users_Dist(users[i], users[j]), j});
            }
        }
    }

    for(int i = 0; i < users.size(); ++i) {
        sort(users_dist[i].begin(), users_dist[i].end());
    }

    for(int i = 0; i < users.size(); ++i) {
        double plus = users_dist[i][0].first;
        for(int j = 0; j < users.size(); ++j) {
            users_dist[i][j].first -= plus;
        }
    }
}

void count_users_answers_dist(const vector<User_Graph>& Gr, vector<vector<double>> &answ_dist) {
    for(int i = 0; i < Gr.size(); ++i) {
        for(int j = i + 1; j < Gr.size(); ++j) {
            answ_dist[i][j] = Gr[i].Get_Dist(Gr[j]);
            answ_dist[j][i] = answ_dist[i][j];
        }
    }
}

double Choose_Answ(Users_Knn& Users_par, Cars_Knn& Cars_par,
                vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &cars_dist,
                vector<User_Graph>& Gr, int user_id, int first, int second) {

    double vote_for_first = 0;
    double vote_for_second = 0;

    //Явное вычисление на графе
    /*if(Gr[user_id].IsBetter(first, second) &&
       !(Gr[user_id].IsBetter(second, first))) {
        return first;
    }

    if(Gr[user_id].IsBetter(second, first) &&
       !(Gr[user_id].IsBetter(first, second))) {
        return second;
    }*/

    for (int k = 0; k < Users_par.Get_K(); ++k) {
        int k_closest_ind = users_dist[user_id][k].second;
        double d = users_dist[user_id][k].first;
        vote_for_first += Gr[k_closest_ind].is_better[first][second] * Gaus(d);
        vote_for_second += Gr[k_closest_ind].is_better[second][first] * Gaus(d);
    }

    for (int k1 = 0; k1 < Cars_par.Get_K(); ++k1) {
        int left = cars_dist[first][k1].second;
        int right = second;
        double d = cars_dist[first][k1].first;
        vote_for_first += Gr[user_id].is_better[left][right] * Gaus(d);
    }

    for (int k2 = 0; k2 < Cars_par.Get_K(); ++k2) {
        int left = first;
        int right = cars_dist[second][k2].second;
        double d = cars_dist[second][k2].first;
        vote_for_second += Gr[user_id].is_better[right][left] * Gaus(d);
    }

    int alg_answ;
    if (vote_for_first > vote_for_second) {
        alg_answ = first;
    } else {
        alg_answ = second;
    }

    return alg_answ;
}

int Check_Par(const vector<User>& users, const vector<Car>& cars,
              const vector<Quest>& test_quest, Users_Knn& Users_par, Cars_Knn& Cars_par,
              vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &cars_dist,
              vector<User_Graph>& Gr) {

    double delta = 0;

    for(int i = 0; i < test_quest.size(); ++i) {
        int correct = test_quest[i].first;
        int alg_answ;
        alg_answ = Choose_Answ(Users_par, Cars_par, users_dist, cars_dist, Gr,
                               test_quest[i].id, test_quest[i].first, test_quest[i].second);
        delta += (alg_answ != correct);
    }

    return delta;
}


const int RAND_RANGE = 10;
//Метод отжига
bool anneal(vector<int> &w_user, vector<int> &w_car, double &T) {

    if (T < 0.01) {
        return false;
    }

    int rand_ind = rand() % (w_user.size() + w_car.size());

    if (rand_ind < w_car.size()) {
        int max_it = 5;
        while (max_it > 0) {
            double rand_shift = (rand() % RAND_RANGE) - (RAND_RANGE / 2);
            if (w_car[rand_ind] + rand_shift >= 0) {
                w_car[rand_ind] += rand_shift;
                break;
            }
            --max_it;
        }
    } else {
        rand_ind -= w_car.size();
        int max_it = 5;
        while (max_it > 0) {
            double rand_shift = (rand() % RAND_RANGE) - (RAND_RANGE / 2);
            if (w_user[rand_ind] + rand_shift >= 0) {
                w_user[rand_ind] += rand_shift;
                break;
            }
            --max_it;
        }
    }

    T *= 0.75;
    return true;
}

void random_fill(vector<int> &v, int MAX) {
    for(int i = 0; i < v.size(); ++i) {
        v[i] = random() % (MAX + 1);
    }
}


void Find_Best_Parametrs(const vector<User>& users, const vector<Car>& cars,
                         const vector<Quest>& test_quest, Cars_Knn& Cars_par, Users_Knn& Users_par,
                         vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &cars_dist,
                         const vector<vector<double>>& answ_dist, vector<User_Graph>& Gr) {

    int min_dev = INT32_MAX;
    int best_k_car, best_k_user;
    vector<int> best_w_user(4), best_w_car(4);
    //vector<int> w_car {1, 2, 3, 4};
    //vector<int> w_user {1, 2, 3, 4};
    vector<int> w_car(4); random_fill(w_car, 5);
    vector<int> w_user(4); random_fill(w_user, 5);
    Users_par.Change_weights(w_user);
    Cars_par.Change_weights(w_car);
    best_w_user = w_user, best_w_car = w_car;
    count_users_dist(users_dist, answ_dist, users, Users_par);
    count_cars_dist(cars_dist, cars, Cars_par);

    /*do {
        //Users_par.Change_weights(w_user);
        //count_users_dist(users_dist, answ_dist, users, Users_par);
        do {
            //Cars_par.Change_weights(w_car);
            //count_cars_dist(cars_dist, cars, Cars_par);
            for (int k_user = MIN_K_USER; k_user < MAX_K_USER; ++k_user) {
                for (int k_car = MIN_K_CAR; k_car < MAX_K_CAR; ++k_car) {
                    Cars_par.Change_K(k_car);
                    Users_par.Change_K(k_user);
                    int deviation = Check_Par(users, cars, test_quest, Users_par, Cars_par,
                                              users_dist, cars_dist, Gr);
                    if (deviation < min_dev) {
                        min_dev = deviation;
                        best_k_car = k_car;
                        best_k_user = k_user;
                        best_w_car = w_car;
                        best_w_user = w_user;
                    }
                }
            }
        } while (next_permutation(w_car.begin(), w_car.end()));
    } while(next_permutation(w_user.begin(), w_user.end())); */

    double MAX_T = 1000;
    //Users_par.Change_weights(w_user);
    //count_users_dist(users_dist, answ_dist, users, Users_par);
    do {

        Cars_par.Change_weights(w_car);
        count_cars_dist(cars_dist, cars, Cars_par);
        Users_par.Change_weights(w_user);
        count_users_dist(users_dist, answ_dist, users, Users_par);
        bool are_weights_better = false;
        double best_weights_dev = INT32_MAX;
        int best_weights_k_user, best_weights_k_car;
        for (int k_user = MIN_K_USER; k_user < MAX_K_USER; ++k_user) {
            for (int k_car = MIN_K_CAR; k_car < MAX_K_CAR; ++k_car) {
                Cars_par.Change_K(k_car);
                Users_par.Change_K(k_user);
                int deviation = Check_Par(users, cars, test_quest, Users_par, Cars_par,
                                          users_dist, cars_dist, Gr);

                if(deviation < best_weights_dev) {
                    best_weights_dev = deviation;
                    best_weights_k_user = k_user;
                    best_weights_k_car = k_car;
                }

                if (deviation < min_dev) {
                    min_dev = deviation;
                    best_k_car = k_car;
                    best_k_user = k_user;
                    are_weights_better = true;
                }
            }
        }

        if(are_weights_better) {
            best_w_user = w_user;
            best_w_car = w_car;
        } else {
            double p = exp(-(best_weights_dev - min_dev) / MAX_T);
            if (p >= (double) (rand()) / RAND_MAX) {
                best_w_user = w_user;
                best_w_car = w_car;
                min_dev = best_weights_dev;
                best_k_car = best_weights_k_car;
                best_k_user = best_weights_k_user;
            } else {
                w_user = best_w_user;
                w_car = best_w_car;
            }
        }

    } while (anneal(w_user, w_car, MAX_T));

    cout << "Test Deviation =" << min_dev << endl;

    Cars_par.Change_K(best_k_car);
    Cars_par.Change_weights(best_w_car);
    Users_par.Change_K(best_k_user);
    Users_par.Change_weights(best_w_user);
    //cout << min_dev << endl;
    cout << "Best User's K = " << best_k_user << endl << "Best Car's K = " << best_k_car << endl;
    cout << "Car's weights :" << endl;
    Cars_par.Show_w();
    cout << "User's weights :" << endl;
    Users_par.Show_w();
}

void Test_Quest(const vector<Quest>& check_quest, Users_Knn& Users_par, Cars_Knn& Cars_par,
                vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &cars_dist,
                vector<User_Graph>& Gr) {
    int wrong = 0;
    for(int i = 0; i < check_quest.size(); ++i) {
        int correct = check_quest[i].first;
        int alg_answ = Choose_Answ(Users_par, Cars_par, users_dist, cars_dist, Gr,
                                   check_quest[i].id, check_quest[i].first, check_quest[i].second);
        if(alg_answ != correct) {
            ++wrong;
        }
    }
    cout << "Wrong Answers = "<< wrong << " of " << check_quest.size() << endl;
}

int main() {
    srand(time(NULL));

    ifstream User_file, Item_file, Pref_file;
    User_file.open("users");
    Item_file.open("items");
    Pref_file.open("prefs");

    vector<User> users;
    vector<Car> cars;
    vector<Quest> learn_quest, check_quest, test_quest;

    Cars_Knn* Cars_par = new Cars_Knn();
    Users_Knn* Users_par = new Users_Knn();

    Get_users(User_file, users, *Users_par);
    Get_Items(Item_file, cars, *Cars_par);
    Divide_Pref_Data(Pref_file, learn_quest, test_quest, check_quest);

    //TRIVIAL
    /*cout << "TRIVIAL SOLUTION RESULTS:" << endl;
    TrivialSolve(users, cars, learn_quest, test_quest, check_quest);
    cout << endl; */
    // end
    //MAIN
    cout << "MAIN SOLUTION RESULTS:" << endl;
    vector<User_Graph> Users_Gr = Make_User_Graph(learn_quest);
    vector<vector<double>> answ_dist(Users_Gr.size(), vector<double> (Users_Gr.size(), 0));
    count_users_answers_dist(Users_Gr, answ_dist);
    Calc_UsersGr_Relations(Users_Gr);
    vector<vector<pair<double, int>>> users_dist(users.size());
    vector<vector<pair<double, int>>> cars_dist(cars.size());
    Find_Best_Parametrs(users, cars, test_quest, *Cars_par, *Users_par, users_dist, cars_dist, answ_dist, Users_Gr);
    Test_Quest(check_quest, *Users_par, *Cars_par, users_dist, cars_dist, Users_Gr);
    // end
}