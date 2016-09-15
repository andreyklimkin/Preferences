#include "classes.h"
#include "baseline.h"
#include "LPM.h"

using namespace std;

//global parametrs
int number_of_users; // 60 for cars data, any x in [0;5000] for sushi data (main points = 100, 200, 500, 1000)
int number_of_items; // 10 for both sushi and cars
int num_of_users_features; // 4 for cars data, 10 for sushi data, 
int num_of_items_features; // 4 for cars data, 7 for sushi data, 


int main() {
    //test_times - number of running algorithm (need to get average results)
	for (int test_times = 0; test_times < 1; ++test_times) {
		srand(clock());

		bool cars_data = true;
		global_fill(cars_data); // form global parametrs, cars_data = true for cars data set, false for sushi data set 

		FILE *User_file, *Items_file, *Pref_file;
		User_file = fopen("cars_data/users.txt", "r"); // cars_data/users.txt for cars data, sushi_data/sushi_users.txt for sushi data
		Items_file = fopen("cars_data/items.txt", "r"); // cars_data/items.txt for cars data, sushi_data/sushi_items.txt for sushi data
		Pref_file = fopen("cars_data/prefs.txt", "r"); // cars_data/prefs.txt for cars data, sushi_data/sushi_prefs.txt for sushi data

		vector<Quest> learn_quest, check_quest, test_quest;
		Items_Knn Items_par = Items_Knn();
		Users_Knn Users_par = Users_Knn();
		vector<User> users;
		vector<Item> items;

		ReaderCarsData Reader;
		Reader.get_users(User_file, users);
		Reader.get_cars(Items_file, items);

		//ReaderSushiData Reader;
		//Reader.get_users(User_file, users);
		//Reader.get_sushi(Items_file, items); 

		//Divide data for cars
		PreparerDataCars::divide_pref_data(Pref_file, learn_quest, check_quest, test_quest);

		// Divide data for sushi 
		//PreparerDataSushi::divide_pref_data(Pref_file, learn_quest, check_quest, test_quest);

		//Normalize attributes (only for KNN algorithms, not for LPM)
		PreparerData::normalize_weights<User>(users);
		PreparerData::normalize_weights<Item>(items);

		cout << "MAIN SOLUTION RESULTS:" << endl;
		KnnSolver clf = KnnSolver();
		vector<User_Graph> Users_Gr = User_Graph::Make_User_Graph(learn_quest);
		vector<vector<double>> answ_dist(Users_Gr.size(), vector<double>(Users_Gr.size(), 0));
		Distance::count_users_answers_dist(Users_Gr, answ_dist);
		User_Graph::Calc_UsersGr_Relations(Users_Gr);
		vector<vector<pair<double, int>>> users_dist(users.size());
		vector<vector<pair<double, int>>> items_dist(items.size());

		int wrong;
		//If you do not want to see detailed report of any following method, give for this method last parametr=0
		clf.find_best_parametrs(users, items, check_quest, Users_par, Items_par, users_dist, items_dist, answ_dist, Users_Gr, 1);
		wrong = clf.test_algo(test_quest, Users_par, Items_par, users_dist, items_dist, Users_Gr, 1);
		unsigned int end_time = clock();
        //running time of the algorithm
		cout << "TIME = " << end_time / (double)CLOCKS_PER_SEC << endl;
		
		FILE *f; f = fopen("main_res_cars.txt", "a");
		fprintf(f, "%.2f%\n", ((double)wrong / test_quest.size()) * 100);


		cout << "BaseLine SOLUTION RESULTS:" << endl;
		Baseline_Calc Bc;
		wrong = Bc.get_deviation(Users_Gr, test_quest);
		cout << "Wrong Answers = " << wrong << " of " << test_quest.size();
		printf("(%.2f%%)\n", ((double)wrong / test_quest.size()) * 100);

		f = fopen("baseline_res_cars.txt", "a");
		fprintf(f, "%.2f%\n", ((double)wrong / test_quest.size()) * 100);

		// LPM classifier
		/*auto par_types = Preparer_LPM::get_parametrs_type_sushi();
		auto items_attr = Preparer_LPM::make_attr(items, par_types);
		Vote_LPM clf(items_attr[0].size(), number_of_users);
		clf.learn(learn_quest, items_attr);
		int wrong = clf.test(test_quest, items_attr);
		FILE *f = fopen("lpm_res_cars.txt", "a");
		fprintf(f, "%.2f%\n", ((double)wrong / test_quest.size()) * 100);   */
	}
}