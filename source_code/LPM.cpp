#include "LPM.h"

vector<par_type> Preparer_LPM::get_parametrs_type_cars() {
	vector<par_type> t;
	for (int i = 0; i < num_of_items_features; ++i) {
		t.push_back({ true, 1, 2, i });
	}

	t[2].is_category = false;

	return t;
}

vector<par_type> Preparer_LPM::get_parametrs_type_sushi() {
	vector<par_type> t(num_of_items_features);
	for (int i = 0; i < 2; ++i) {
		t[i] = { true, 0, 1, i };
	}

	t[2] = { true, 0, 11, 2 };
	t[3] = { true, 0, 4, 3 };
	t[4] = { true, 0, 3, 4 };
	t[5] = { false, 0, 0, 5 };
	t[6] = { false, 0, 0, 6 };

	return t;
}

vector<vector<attr>> Preparer_LPM::make_attr(const vector<Item>& items, const vector<par_type>& par_type) {

	vector<vector<attr>> lpm_items(number_of_items);

	for (int i = 0; i < lpm_items.size(); ++i) {
		for (int j = 0; j < items[i].attr.size(); ++j) {
			if (!par_type[j].is_category) {
				lpm_items[i].push_back({ items[i].attr[j], j });
				lpm_items[i].push_back({ -items[i].attr[j], j });
			}
			else {
				for (int k = par_type[j].left; k <= par_type[j].right; ++k) {
					if (items[i].attr[j] == k) {
						lpm_items[i].push_back({ 1, j });
					}
					else {
						lpm_items[i].push_back({ 0, j });
					}
				}

				for (int k = par_type[j].left; k <= par_type[j].right; ++k) {
					if (items[i].attr[j] != k) {
						lpm_items[i].push_back({ 1, j });
					}
					else {
						lpm_items[i].push_back({ 0, j });
					}
				}
			}
		}
	}

	return lpm_items;
}

void Vote_LPM::learn(const vector<Quest>& quest, const vector<vector<attr>>& items) {
	vector<vector<Quest>> q(num_user);
	for (int i = 0; i < quest.size(); ++i) {
		q[quest[i].id].push_back(quest[i]);
	}

	for (int i = 0; i < num_user; ++i) {
		learn_user(i, q[i], items);
	}
}

void Vote_LPM::learn_user(int user_id, const vector<Quest>& quest, const vector<vector<attr>>& items) {

	vector<rank_type> z_rank;
	for (int i = 0; i < attr_sz; ++i) {
		z_rank.push_back({ 1, i, items[0][i].ind });
	}

	bool changes = true;
	while (changes) {
		changes = false;

		for (auto q : quest) {

			vector<rank_type> dif_z;
			for (int i = 0; i < attr_sz; ++i) {
				if (abs(items[q.first][i].val - items[q.second][i].val) > 1e-6) {
					dif_z.push_back(z_rank[i]);
				}
			}

			sort(dif_z.begin(), dif_z.end(), [](rank_type &A, rank_type &B) {return A.rank < B.rank; });

			int min = dif_z[0].rank;
			while (dif_z.back().rank > min) {
				dif_z.pop_back();
			}

			vector<rank_type> first_votes;
			vector<rank_type> second_votes;

			for (auto z : dif_z) {
				if (items[q.first][z.ind].val >= items[q.second][z.ind].val) {
					first_votes.push_back(z);
				}

				if (items[q.first][z.ind].val <= items[q.second][z.ind].val) {
					second_votes.push_back(z);
				}
			}

			if (second_votes.size() >= first_votes.size()) {
				for (auto z : second_votes) {
					if (z_rank[z.ind].rank < z_rank.size()) {
						changes = true;
						z_rank[z.ind].rank++;
					}
				}
			}
		}
	}

	sort(z_rank.begin(), z_rank.end(), [](rank_type &A, rank_type &B) {return A.rank < B.rank; });

	vector<int> used(num_of_items_features, -1);

	for (int i = 0; i < z_rank.size(); ++i) {
		auto z = z_rank[i];
		if (used[z.attr_group] == -1 || used[z.attr_group] == i - 1) {
			attr_order[user_id].push_back(z.ind);
			used[z.attr_group] = i;
		}
	}
}

int Vote_LPM::test(const vector<Quest>& quest, const vector<vector<attr>>& items) {

	int wrong = 0;

	for (auto q : quest) {
		int i = 0;
		while (i < attr_order[q.id].size() && abs(items[q.first][attr_order[q.id][i]].val - items[q.second][attr_order[q.id][i]].val) < 1e-6) {
			++i;
		}

		if (i == attr_order[q.id].size()) {
			++wrong;
		}

		else {
			if (items[q.first][attr_order[q.id][i]].val < items[q.second][attr_order[q.id][i]].val) {
				++wrong;
			}
		}
	}

	return wrong;
}
