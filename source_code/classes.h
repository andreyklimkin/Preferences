//
// Created by andreyklimkin on 09.01.16.
//
//#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <set>
#include <algorithm>
#include <climits>
#include <limits.h>
#include <cstring>
#include <fstream>
#include <cstdint>

using namespace std;

#pragma once

#ifndef CARS_PREFERENCES_CLASSES_H
#define CARS_PREFERENCES_CLASSES_H
extern int number_of_users;
extern int number_of_items;
extern int num_of_users_features;
extern int num_of_items_features;
#define M_PI 3.14159265358979323846


class Math {
public:
	static double pow2(double a);
	static double Gaus(double d);
	static double rand01();
	static const int SIGMA = 3;
};


class Quest{
public:
	int id;
	int first;
	int second;
	int control;
	Quest() { }
	Quest(int a, int b, int c, int d) : id(a), first(b), second(c), control(d) { };

	bool operator==(Quest A) {
		if (this->id == A.id && this->first == A.first && this->second == A.second) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator<(const Quest A) const {
		return this->id < A.id;
	}

};


class Item {
public:
	vector<double> attr;
	int id;
	Item() {
		attr.resize(num_of_items_features);
	}
	Item(const vector<double> &init_attr) {
		attr.resize(init_attr.size());
		for (int i = 0; i < attr.size(); ++i) {
			attr[i] = init_attr[i];
		}
	}
};


class User {
public:
	vector<double> attr;
	int id;

	User() {
		attr.resize(num_of_users_features);
	}

	User(const vector<double> &init_attr) {
		attr.resize(init_attr.size());
		for (int i = 0; i < attr.size(); ++i) {
			attr[i] = init_attr[i];
		}
	}

};


class User_Graph {
public:
	vector<vector<int>> g_matrix;
	vector<vector<int>> g;
	vector<vector<int>> is_better;

	User_Graph() {
		g_matrix.resize(number_of_items, vector<int>(number_of_items, 0));
	}

	static vector<User_Graph> Make_User_Graph(const vector<Quest>& learn);

	static void Calc_UsersGr_Relations(vector<User_Graph> &G);

	double get_answers_dist(const User_Graph& second) const;

	void Add_edge(int a, int b);

private:

	void Dfs(int v, vector<int>& used, int need);

	void IsBetter(int first, int second);
};


class Knn {
protected:
	vector<double> weight;
	int k;
public:
	int Get_K();

	void Change_K(int new_k);

	void Show_w() {
		for (auto elem : weight) {
			printf("%.2f ", elem);
			//cout << elem << " ";
		}
		printf("\n");
	}
};


class Users_Knn :public Knn {
public:

	double answ_kof;

	Users_Knn() {
		weight.resize(num_of_users_features, 0);
	}

	double Get_Users_Dist(const User& first, const User& second);


	void Change_weights(const vector<double>& new_w);

};


class Items_Knn : public Knn {
public:

	double answ_kof;

	Items_Knn() {
		weight.resize(num_of_items_features, 0);
	}

	double Get_Dist(const Item& first, const Item& second);

	void Change_weights(const vector<double>& new_w);
};


class Distance {
public:

	static void count_items_dist(const vector<Item>& items, Items_Knn& Items_par,
		vector<vector<pair<double, int>>> &items_dist);

	static void count_users_answers_dist(const vector<User_Graph>& Gr, vector<vector<double>> &answ_dist);

	static void count_users_dist(const vector<User>& users, Users_Knn& Users_par, const vector<vector<double>> &answ_dist,
		vector<vector<pair<double, int>>> &users_dist);
};


class KnnSolver {
private:
	int ANNEAL_STARTS;
	int MIN_K_ITEM, MAX_K_USER;
	int MIN_K_USER, MAX_K_ITEM;
	int NEED_TEST_QUEST;
	int NUM_OF_SHUFFLE = 5;

	double check_par(const vector<User>& users, const vector<Item>& items,
		vector<Quest>& check_quest, Users_Knn& Users_par, Items_Knn& Items_par,
		vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &items_dist,
		vector<User_Graph>& Gr);

	void random_fill(vector<double> &v, double left, double right) {
		for (int i = 0; i < v.size(); ++i) {
			v[i] = Math::rand01() * (right - left) + left;
		}
	}

	//Метод отжига
	bool anneal(vector<double> &w_user, vector<double> &w_item, double &users_kof, double &items_kof, double &T);

	bool appropriate_kof(double kof) {
		if (kof >= 0 && kof <= 1) {
			return true;
		}
		else {
			return false;
		}
	}

public:
	KnnSolver(int min_k_user = 1, int max_k_user = 40, int min_k_car = 1, int max_k_car = 10, int an_st = 30, int ntq = 400) {
		MIN_K_USER = min_k_user, MAX_K_USER = max_k_user;
		MIN_K_ITEM = min_k_car, MAX_K_ITEM = max_k_car;
		ANNEAL_STARTS = an_st;
		NEED_TEST_QUEST = ntq;
	}

	void find_best_parametrs(const vector<User>& users, const vector<Item>& items,
		vector<Quest>& check_quest, Users_Knn& Users_par, Items_Knn& Items_par,
		vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &items_dist,
		const vector<vector<double>>& answ_dist, vector<User_Graph>& Gr, int fl);

	double choose_answ(Users_Knn& Users_par, Items_Knn& Cars_par,
		vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &cars_dist,
		vector<User_Graph>& Gr, int user_id, int first, int second, bool is_test = false);

	int test_algo(const vector<Quest>& test_quest, Users_Knn& Users_par, Items_Knn& Items_par,
		vector<vector<pair<double, int>>> &users_dist, vector<vector<pair<double, int>>> &items_dist,
		vector<User_Graph> &Gr, int fl = 1);
};


class ReaderCarsData {
public:
	void get_users(FILE* file, vector<User> &users);
	void get_cars(FILE* file, vector<Item> &items);
};


class ReaderSushiData {
public:
	void get_users(FILE *file, vector<User> &users);
	void get_sushi(FILE *file, vector<Item> &items);
};


class PreparerData {
public:
	template<typename T>
	static void normalize_weights(vector<T> &w) {
		for (int i = 0; i < w[0].attr.size(); ++i) {
			vector<double> v;
			for (int j = 0; j < w.size(); ++j) {
				v.push_back(w[j].attr[i]);
			}

			double mean = PreparerData::get_mean(v);
			double var = PreparerData::get_var(v, mean);

			for (int j = 0; j < w.size(); ++j) {
				w[j].attr[i] -= mean;
				w[j].attr[i] /= sqrt(var);
			}
		}
	}

	static double get_mean(const vector<double> &v);
	static double get_var(const vector<double> &v, double mean);
};


class PreparerDataSushi : public PreparerData {
public:

	static void divide_pref_data(FILE *file, vector<Quest> &learn_quest, vector<Quest> &check_quest,
		vector<Quest> &test_quest);
};


class PreparerDataCars : public PreparerData  {
public:
	static bool find_contradict(vector<Quest> &learn, Quest tmpl, int indx);

	static void clean_quest(vector<Quest> &learn);

	static void divide_pref_data(FILE *file, vector<Quest> &learn_quest, vector<Quest> &check_quest,
		vector<Quest> &test_quest);

};


void static global_fill(bool fl) {
	if (fl) {
		number_of_users = 60;
		number_of_items = 10;
		num_of_users_features = 4;
		num_of_items_features = 4;
	}
	else {
		number_of_users = 1000;
		number_of_items = 10;
		num_of_users_features = 10;
		num_of_items_features = 7;
	}
}

#endif //CARS_PREFERENCES_CLASSES_
