//
// Created by andreyklimkin on 09.01.16.
//
#include "classes.h"

using namespace std;

void ReaderSushiData::get_users(FILE *file, vector<User>& users) {

    for (int i = 0; i < number_of_users; ++i) {
        User next = User();
        next.id = i;
        int trash;
        fscanf(file, "%d", &trash);
        for (int j = 0; j < num_of_users_features; ++j) {
            fscanf(file, "%lf", &next.attr[j]);
        }
        users.push_back(next);

    }
}

void ReaderSushiData::get_sushi(FILE *file, vector<Item>& items) {

    for (int i = 0; i < number_of_items; ++i) {
        Item next = Item();
        fscanf(file, "%d", &next.id);
        for (int j = 0; j < num_of_items_features; ++j) {
            fscanf(file, "%lf", &next.attr[j]);
        }
        items.push_back(next);
    }
}

void PreparerDataSushi::divide_pref_data(FILE *file, vector<Quest>& learn_quest, vector<Quest>& check_quest,
                                        vector<Quest>& test_quest) {
    int trash1, trash2;
    fscanf(file, "%d%d", &trash1, &trash2);
    for (int i = 0; i < number_of_users; ++i) {
        fscanf(file, "%d%d", &trash1, &trash2);

        vector<int> order(number_of_items);
        for (int j = 0; j < number_of_items; ++j) {
            fscanf(file, "%d", &order[j]);
        }

        for (int s1 = 0; s1 < number_of_items; ++s1) {
            for (int s2 = s1 + 1; s2 < number_of_items; ++s2) {
                int rand_int = rand() % 100;
                if (rand_int < 50) {
                    learn_quest.push_back({i, order[s1], order[s2], 0});
                } else if (rand_int < 75) {
                    check_quest.push_back({i, order[s1], order[s2], 0});
                } else {
                    test_quest.push_back({i, order[s1], order[s2], 0});
                }
            }
        }
    }
    //cout << learn_quest.size() << " " << test_quest.size() << " " << check_quest.size();
}

double PreparerData::get_mean(const vector<double> &v) {

    double sum = 0;
    for(auto elem : v) {
        sum += elem;
    }
    return sum / v.size();
}

double PreparerData::get_var(const vector<double> &v, double mean) {

    double sum = 0;
    for (auto elem : v) {
        sum += (elem - mean) * (elem - mean);
    }
    return sum / (v.size() - 1);
}




void ReaderCarsData::get_users(FILE* file, vector<User>& users) {
	for (int i = 0; i < number_of_users; ++i) {
        User next = User();
        int cor_contr;
		if (fscanf(file, "%d,%lf,%lf,%lf,%lf,%d", &next.id, &next.attr[0], &next.attr[1], &next.attr[2],
			&next.attr[3], &cor_contr) != num_of_users_features + 2) {
			break;
		}
        --next.id;
		users.push_back(next);
    }
}

void ReaderCarsData::get_cars(FILE* file, vector<Item>& items) {
    for(int i = 0; i < number_of_items; ++i) {
        Item next = Item();
		if (fscanf(file, "%d,%lf,%lf,%lf,%lf", &next.id, &next.attr[0], &next.attr[1],
			&next.attr[2], &next.attr[3]) != num_of_items_features + 1) {
			break;
		}
        --next.id;
        items.push_back(next);
    }
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

void PreparerDataCars::divide_pref_data(FILE* file, vector<Quest>& learn_quest, vector<Quest>& check_quest,
										vector<Quest>& test_quest) {
    string s;
	while (1) {
        int random_int;
        random_int = (rand() % 100);
        Quest *next = new Quest();
		if (fscanf(file, "%d,%d,%d,%d", &next->id, &next->first, &next->second, &next->control) != 4) {
			break;
		}
        if (next->id > number_of_users) {
            number_of_users = next->id;
        }
        --next->id, --next->first, --next->second;
        if (random_int < 50) {
            learn_quest.push_back(*next);
        } else {
            if (random_int < 75) {
                check_quest.push_back(*next);
            } else {
                test_quest.push_back(*next);
            }
        }
    }
    PreparerDataCars::clean_quest(learn_quest);
}



void User_Graph::Add_edge(int a, int b) {
    g[a].push_back(b);
    g_matrix[a][b] = 1;
}

double Math::pow2(double a) {
    return a * a;
}

double Math::Gaus(double d) {
    double res =  exp(-(d * d) / (2 * SIGMA * SIGMA)) / sqrt (2 * M_PI * SIGMA * SIGMA);
    return res;
}

double Math::rand01 () {
    return (double) rand() / RAND_MAX;
}


vector<User_Graph> User_Graph::Make_User_Graph(const vector<Quest>& learn) {
    vector<set<pair<int, int>>> s(number_of_users);
    vector<User_Graph> res_gr;
    res_gr.resize(number_of_users);
    for(int i = 0; i < number_of_users; ++i) {
        res_gr[i].g.resize(number_of_items);
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

void User_Graph::Calc_UsersGr_Relations(vector<User_Graph> &G) {

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

void User_Graph::Dfs(int v, vector<int>& used, int need) {
    used[v] = 1;
    if (need == v) {
        return;
    }

    for (auto ver : g[v]) {
        if (!used[ver]) {
            Dfs(ver, used, need);
        }
    }
}

void User_Graph::IsBetter(int first, int second) {
    vector<int> used(g.size(), 0);
    Dfs(first, used, second);
    is_better[first][second] = used[second];
}

double User_Graph::get_answers_dist(const User_Graph& second) const {

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

void Users_Knn::Change_weights(const vector<double>& new_w) {
    for(int i = 0; i < new_w.size(); ++i) {
        weight[i] = new_w[i];
    }
}

void Items_Knn::Change_weights(const vector<double>& new_w) {
    for (int i = 0; i < new_w.size(); ++i) {
        weight[i] = new_w[i];
    }
}

double Users_Knn::Get_Users_Dist(const User& first, const User& second) {

    double res = 0;
    for(int i = 0; i < first.attr.size(); ++i) {
        if (i >= 3) {
            res += (first.attr[i] != second.attr[i]) * weight[i];
        } else {
            res += Math::pow2((first.attr[i] - second.attr[i])) * weight[i];
        }
    }
    /*if(first.educ && second.educ) {
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

    } */

    return sqrt(res);
}

double Items_Knn::Get_Dist(const Item& first, const Item& second) {
    double res = 0;
    for(int i = 0; i < first.attr.size(); ++i) {
        if (i == 2) {
            res += (first.attr[i] != second.attr[i]) * weight[i];
        } else {
            res += Math::pow2((first.attr[i] - second.attr[i])) * weight[i];
        }
    }
    return sqrt(res);
}


void Distance::count_items_dist(const vector<Item>& items, Items_Knn& Items_par,
                      vector<vector<pair<double, int>>> &items_dist) {
    for(int i = 0; i < items.size(); ++i) {
        items_dist[i].resize(0);
        for(int j = 0; j < items.size(); ++j) {
            if (j == i) {
                continue;
            } else {
                items_dist[i].push_back({Items_par.Get_Dist(items[i], items[j]), j});
            }
        }
    }

    for(int i = 0; i < items.size(); ++i) {
        sort(items_dist[i].begin(), items_dist[i].end());
    }

    for(int i = 0; i < items.size(); ++i) {
        double plus = items_dist[i][0].first;
        for(int j = 0; j < items.size() - 1; ++j) {
            items_dist[i][j].first -= plus;
        }
    }

}


void Distance::count_users_dist(const vector<User>& users, Users_Knn& Users_par, const vector<vector<double>> &answ_dist,
								vector<vector<pair<double, int>>> &users_dist) {
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
        for(int j = 0; j < users.size() - 1; ++j) {
            users_dist[i][j].first -= plus;
        }
    }
}

void Distance::count_users_answers_dist(const vector<User_Graph>& Gr, vector<vector<double>> &answ_dist) {
    for(int i = 0; i < Gr.size(); ++i) {
        for(int j = i + 1; j < Gr.size(); ++j) {
            answ_dist[i][j] = Gr[i].get_answers_dist(Gr[j]);
            answ_dist[j][i] = answ_dist[i][j];
        }
    }
}


double KnnSolver::choose_answ(Users_Knn& Users_par, Items_Knn& Cars_par,
                   vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &cars_dist,
                   vector<User_Graph>& Gr, int user_id, int first, int second, bool is_test) {

    double vote_for_first = 0;
    double vote_for_second = 0;

	//Явное вычисление на графе
	/*if (is_test) {
		if (Gr[user_id].is_better[first][second] &&
			!(Gr[user_id].is_better[second][first])) {
			return first;
		}

		if (Gr[user_id].is_better[second][first] &&
			!(Gr[user_id].is_better[first][second])) {
			return second;
		}
	} */


    for (int k = 0; k < Users_par.Get_K(); ++k) {
        int k_closest_ind = users_dist[user_id][k].second;
        double d = users_dist[user_id][k].first;
        vote_for_first += Users_par.answ_kof * Gr[k_closest_ind].is_better[first][second] * Math::Gaus(d);
        vote_for_second += Users_par.answ_kof * Gr[k_closest_ind].is_better[second][first] * Math::Gaus(d);
    }

    for (int k1 = 0; k1 < Cars_par.Get_K(); ++k1) {
        int left = cars_dist[first][k1].second;
        int right = second;
        double d = cars_dist[first][k1].first;
        vote_for_first += Cars_par.answ_kof * Gr[user_id].is_better[left][right] * Math::Gaus(d);
    }

    for (int k2 = 0; k2 < Cars_par.Get_K(); ++k2) {
        int left = first;
        int right = cars_dist[second][k2].second;
        double d = cars_dist[second][k2].first;
        vote_for_second += Cars_par.answ_kof * Gr[user_id].is_better[right][left] * Math::Gaus(d);
    }

    int alg_answ;
    if (vote_for_first > vote_for_second) {
        alg_answ = first;
    } else {
        alg_answ = second;
    }

    return alg_answ;
}

double KnnSolver::check_par(const vector<User>& users, const vector<Item>& items,
                 vector<Quest>& check_quest, Users_Knn& Users_par, Items_Knn& Items_par,
                 vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &items_dist,
                 vector<User_Graph>& Gr) {

    vector<double> deltas;

    for(int it = 0; it < NUM_OF_SHUFFLE; ++it) {
        double delta = 0;
        random_shuffle(check_quest.begin(), check_quest.end());
        for (int i = 0; i < min((int)check_quest.size(), NEED_TEST_QUEST); ++i) {
            int correct = check_quest[i].first;
            int alg_answ;
            alg_answ = choose_answ(Users_par, Items_par, users_dist, items_dist, Gr,
                                   check_quest[i].id, check_quest[i].first, check_quest[i].second);
            delta += (alg_answ != correct);
        }
        deltas.push_back(delta);
    }

    return PreparerData::get_mean(deltas);
}

//Метод отжига
bool KnnSolver::anneal(vector<double> &w_user, vector<double> &w_item,
                       double &users_kof, double &items_kof, double &T) {

    if (T < 1e-8) {
        return false;
    }

    int range_for_weights = w_user.size() + w_item.size();
    int range_for_kof = 8;
    int Max_ind_range = range_for_weights + range_for_kof;

    static double step = 5;
    if(T == 100) {
        step = 5;
    }

    int rand_ind = rand() % (Max_ind_range);

    if (rand_ind < w_item.size()) {
        int max_it = 5;
        while (max_it > 0) {
            double rand_shift = (Math::rand01() * 2 * step) - step;
            if (w_item[rand_ind] + rand_shift >= 0) {
                w_item[rand_ind] += rand_shift;
                break;
            }
            --max_it;
        }
    } else if(rand_ind >= w_item.size() && rand_ind < w_user.size() + w_item.size()) {
        rand_ind -= w_item.size();
        int max_it = 5;
        while (max_it > 0) {
            double rand_shift = (Math::rand01() * 2 * step) - step;
            if (w_user[rand_ind] + rand_shift >= 0) {
                w_user[rand_ind] += rand_shift;
                break;
            }
            --max_it;
        }
    } else if (rand_ind >= range_for_weights) {
        int max_it = 10;
        while (max_it > 0) {
            double rand_shift = Math::rand01() * 2 - 1;
            if (appropriate_kof(users_kof + rand_shift) && appropriate_kof(items_kof - rand_shift)) {
                users_kof += rand_shift;
                items_kof -= rand_shift;
                break;
            }
            --max_it;
        }
    }

	/*
    double max_prop = 4;
    if(items_kof > max_prop * users_kof) {
        users_kof = items_kof / max_prop;
        items_kof = 1 - users_kof;
    } */

    step *= (0.9);
    T *= 0.75;
    return true;
}

void KnnSolver::find_best_parametrs(const vector<User>& users, const vector<Item>& items,
                         vector<Quest>& check_quest, Users_Knn& Users_par, Items_Knn& Items_par,
                         vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &items_dist,
                         const vector<vector<double>>& answ_dist, vector<User_Graph>& Gr, int fl) {

    int best_k_item, best_k_user;
    double best_users_kof, best_items_kof;
    double min_dev = INT32_MAX;
    vector<double> best_w_user(num_of_users_features), best_w_items(num_of_items_features);
	for (int it = 0; it < ANNEAL_STARTS; ++it) {

		int cur_best_k_item, cur_best_k_user;
		double cur_best_users_kof, cur_best_items_kof;
		double cur_min_dev = INT32_MAX;
		vector<double> cur_best_w_user(num_of_users_features), cur_best_w_items(num_of_items_features);

		double users_kof = min(Math::rand01(), 0.8);
		users_kof = max(0.2, users_kof);
		double items_kof = 1 - users_kof;
		double left_bound = 0, right_bound = 1;
		vector<double> w_items(num_of_items_features);
		random_fill(w_items, left_bound, right_bound);
		vector<double> w_user(num_of_users_features);
		random_fill(w_user, left_bound, right_bound);
		Users_par.Change_weights(w_user);
		Items_par.Change_weights(w_items);
		Distance::count_users_dist(users, Users_par, answ_dist, users_dist);
		Distance::count_items_dist(items, Items_par, items_dist);

		double MAX_T = 100;
		do {
			Items_par.Change_weights(w_items);
			Items_par.answ_kof = items_kof;
			Distance::count_items_dist(items, Items_par, items_dist);
			Users_par.Change_weights(w_user);
			Users_par.answ_kof = users_kof;
			Distance::count_users_dist(users, Users_par, answ_dist, users_dist);
			bool are_weights_better = false;
			double best_weights_dev = INT32_MAX;
			int best_weights_k_user, best_weights_k_items;

			double tmp_items_kof = Items_par.answ_kof;
			double tmp_users_kof = Users_par.answ_kof;

			Items_par.answ_kof = 0;
			for (int k_user = MIN_K_USER; k_user < MAX_K_USER; ++k_user) {
				Items_par.Change_K(0);
				Users_par.Change_K(k_user);
				double deviation = check_par(users, items, check_quest, Users_par, Items_par,
					users_dist, items_dist, Gr);

				if (deviation < best_weights_dev) {
					best_weights_dev = deviation;
					best_weights_k_user = k_user;
				}
			}

			Users_par.answ_kof = 0;
			Items_par.answ_kof = tmp_items_kof;
			best_weights_dev = INT32_MAX;
			for (int k_item = MIN_K_ITEM; k_item < MAX_K_ITEM; ++k_item) {
				Items_par.Change_K(k_item);
				Users_par.Change_K(0);
				double deviation = check_par(users, items, check_quest, Users_par, Items_par,
					users_dist, items_dist, Gr);

				if (deviation < best_weights_dev) {
					best_weights_dev = deviation;
					best_weights_k_items = k_item;
				}
			}

			Items_par.Change_K(best_weights_k_items);
			Users_par.Change_K(best_weights_k_user);
			Users_par.answ_kof = tmp_users_kof;
			Items_par.answ_kof = tmp_items_kof;
			double deviation = check_par(users, items, check_quest, Users_par, Items_par,
				users_dist, items_dist, Gr);

			if (deviation < cur_min_dev) {
				are_weights_better = true;
			}

			if (are_weights_better) {
				cur_min_dev = deviation;
				cur_best_w_user = w_user;
				cur_best_w_items = w_items;
				cur_best_items_kof = items_kof;
				cur_best_users_kof = users_kof;
			}
			else {
				double p = exp(-(best_weights_dev - cur_min_dev) / MAX_T);
				if (p >= Math::rand01()) {
					cur_best_w_user = w_user;
					cur_best_w_items = w_items;
					cur_min_dev = best_weights_dev;
					cur_best_k_item = best_weights_k_items;
					cur_best_k_user = best_weights_k_user;
					cur_best_items_kof = items_kof;
					cur_best_users_kof = users_kof;
				}
				else {
					w_user = cur_best_w_user;
					w_items = cur_best_w_items;
					cur_best_items_kof = items_kof;
					cur_best_users_kof = users_kof;
				}
			}

		} while (anneal(w_user, w_items, users_kof, items_kof, MAX_T));
		if (cur_min_dev < min_dev) {
			min_dev = cur_min_dev, best_k_user = cur_best_k_user, best_k_item = cur_best_k_item;
			best_users_kof = cur_best_users_kof, best_items_kof = cur_best_items_kof;
			best_w_items = cur_best_w_items, best_w_user = cur_best_w_user;
		}
	}

    Items_par.Change_K(best_k_item);
    Items_par.answ_kof = best_items_kof;
    Items_par.Change_weights(best_w_items);
    Users_par.Change_K(best_k_user);
    Users_par.answ_kof = best_users_kof;
    Users_par.Change_weights(best_w_user);
    //cout << cur_min_dev << endl;

	if (fl) {
		printf("Test Deviation = %.3f%%\n\n", ((double)min_dev / NEED_TEST_QUEST) * 100);
		cout << "Best User's K = " << best_k_user << endl << "Best Items's K = " << best_k_item << endl;
		cout << "Item's weights :" << endl;
		cout << "Item's_kof = " << best_items_kof << endl;
		Items_par.Show_w();
		cout << "User's weights :" << endl;
		Users_par.Show_w();
		cout << "Users_kof = " << best_users_kof << endl;
	}
}

int KnnSolver::test_algo(const vector<Quest>& test_quest, Users_Knn& Users_par, Items_Knn& Items_par,
	vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &items_dist,
	vector<User_Graph> &Gr, int fl) {

	int wrong = 0;
	for (int i = 0; i < test_quest.size(); ++i) {
		int correct = test_quest[i].first;
	

		int alg_answ = choose_answ(Users_par, Items_par, users_dist, items_dist, Gr,
			test_quest[i].id, test_quest[i].first, test_quest[i].second, true);
		if (alg_answ != correct) {
			++wrong;
		}
	}

	if (fl) {
		cout << "Wrong Answers = " << wrong << " of " << test_quest.size();
		printf("(%.3f%%)\n\n", ((double)wrong / test_quest.size()) * 100);
	}

	return wrong;
}