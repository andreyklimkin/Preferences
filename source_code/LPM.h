//
// Created by andreyklimkin on 10.05.16.
//

#include "classes.h"

#pragma once

#ifndef CARS_PREFERENCES_LPM_H
#define CARS_PREFERENCES_LPM_H

struct par_type {
	bool is_category;
	int left;
	int right;
	int ind;
};

struct rank_type {
	int rank;
	int ind;
	int attr_group;
};

struct attr {
	double val;
	int ind;
};

class Preparer_LPM {
public:
	static vector<par_type> get_parametrs_type_cars();
	static vector<par_type> get_parametrs_type_sushi();

	static vector<vector<attr>> make_attr(const vector<Item>& items, const vector<par_type>& par_type);
};

class Vote_LPM {
private:

	int attr_sz;
	int num_user;

public:

	vector<vector<int>> attr_order;

	Vote_LPM(int atr_sz, int nm_user) {
		attr_sz = atr_sz;
		num_user = nm_user;
		attr_order.resize(nm_user);
	}

	void learn(const vector<Quest>& quest, const vector<vector<attr>>& items);

	void learn_user(int user_id, const vector<Quest>& quest, const vector<vector<attr>>& items);

	void add_negative_attr(vector<Item>& items);

	int test(const vector<Quest>& quest, const vector<vector<attr>>& items);

};


#endif //CARS_PREFERENCES_LPM_H