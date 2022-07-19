#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cmath>

std::map <std::array <int, 2>, std::vector <std::vector <int>>> permutations = {};
int dice_faces = 12;
int dice_keep = 2;
int dice_roll = 3;

class dice {
public:
	int faces = 0;
	double probability = 0;
	int explosions = 0;
	dice(int faces) {
		dice::faces = faces;
		dice::probability = 1 / faces;
	}

	double get_probability_of_value(int value);
	int get_next_value(int current_value);
	bool value_is_valid(int value);
private:
	std::map <int, std::array <int, 2>> value_map;
	int max_value = 0;
	std::array <int, 2> get_value_counter(int value);
};

bool dice::value_is_valid(int value) {
	int i = 0;
	while (i < value) {
		i += dice::faces;
	}
	if (i == value) {
		return false;
	}
	if (i >= 1) {
		return true;
	}
	return false;
}

std::array <int, 2> dice::get_value_counter(int value) {
	std::array <int, 2> counter;
	int cvalue = 0;

	if (value % dice::faces == 0) {
		throw std::invalid_argument("Dice cannot have value " + value);
	}
	else if (dice::value_map.contains(value)) {
		counter = dice::value_map[value];
	}
	else if (dice::max_value == 0) {
		counter = { 0, 0 };
	}
	else {
		counter = dice::value_map[dice::max_value];
	}
	cvalue = dice::faces * counter[0] + counter[1];
	while (cvalue < value) {
		counter[1]++;
		if (counter[1] == dice::faces) {
			counter[1] = 1;
			counter[0]++;
		}
		cvalue = dice::faces * counter[0] + counter[1];
		dice::value_map.insert({ cvalue, counter });
		while (dice::max_value < cvalue) {
			dice::max_value++;
		}
	}
	return counter;
}

int dice::get_next_value(int current_value) {
	std::array <int, 2> counter;
	counter = dice::get_value_counter(current_value);
	counter[1]++;
	if (counter[1] == dice::faces) {
		counter[1] = 1;
		counter[0]++;
	}
	return dice::faces * counter[0] + counter[1];
}

double dice::get_probability_of_value(int value) {
	std::array <int, 2> counter;
	counter = dice::get_value_counter(value);
	double out = dice::probability;
	for (int i = 0; i < counter[0]; i++) {
		out *= dice::probability;
	}
	return out;
}

class PermutationVector : private std::vector <std::vector <int>> {
public:
	using vector::operator[];
	using vector::push_back;
	using vector::size;
	using vector::insert;
	using vector::back;
	void collect(int a);
	void send();
	void collapse(int i, PermutationVector vec);
private:
	std::vector <int> collection;
};

void PermutationVector::collect(int a) {
	PermutationVector::collection.push_back(a);
}

void PermutationVector::send() {
	PermutationVector::push_back(PermutationVector::collection);
	PermutationVector::collection = {};
}

void PermutationVector::collapse(int starting_value, PermutationVector vec) {
	std::vector <int> temp;
	for (int i = 0; i < vec.size(); i++) {
		temp = { starting_value };
		for (int j = 0; j < vec[i].size(); j++) {
			temp.push_back(vec[i][j]);
		}
		PermutationVector::push_back(temp);
	}
}

class PermutationCollector {
public:
	PermutationVector get_permutations(int target_value, int keep, int max, dice die);
	int get_combinations(int max_value, int no_of_dice, dice die);
private:
	std::map <std::string, PermutationVector> permutations_map;
	std::string get_key(int target_value, int keep);
};

int PermutationCollector::get_combinations(int max_value, int no_of_dice, dice die) {
	int count_of_valid_values = 0;
	int out = 1;
	for (int i = 0; i <= max_value; i++) {
		if (die.value_is_valid(i)) {
			count_of_valid_values++;
		}
	}
	for (int i = 0; i < no_of_dice; i++) {
		out *= count_of_valid_values;
	}
	return out;
}

std::string PermutationCollector::get_key(int target_value, int keep) {
	std::string out = "";
	out += std::to_string(target_value);
	out += "keep";
	out += std::to_string(keep);
	return out;
}

PermutationVector PermutationCollector::get_permutations(int target_value, int keep, int max, dice die) {
	std::string key = PermutationCollector::get_key(target_value, keep);
	PermutationVector out;
	PermutationVector temp;

	if (keep > target_value) {
		throw std::invalid_argument("argument target_value cannot be larger than argument keep");
	}
	if (keep < 1) {
		throw std::invalid_argument("Argument keep must be greater than 0");
	}
	if (PermutationCollector::permutations_map.contains(key)) {
		return PermutationCollector::permutations_map.at(key);
	}
	if (keep == 1) {
		std::vector <int> keep1 = {};
		if (die.value_is_valid(target_value)) {
			keep1.push_back(target_value);
			out.push_back(keep1);
		}		
		return out;
	}
	if (keep == 2) {
		std::vector <int> temp_v;
		for (int i = target_value - 1; i >= target_value - i; i--) {
			if (die.value_is_valid(i) && die.value_is_valid(target_value - i) && i <= max) {
				temp_v = { i, target_value - i };
				out.push_back(temp_v);
			}
		}
		PermutationCollector::permutations_map.insert({ key, out });
		return out;
	}
	if (target_value == keep) {
		std::vector <int> keep1 = {};
		for (int i = 0; i < keep; i++) {
			keep1.push_back(1);
		}
		out.push_back(keep1);
		PermutationCollector::permutations_map.insert({ key, out });
		return out;
	}
	for (int i = std::min(target_value - keep + 1, max); i > (target_value - i)/keep; i--) {
		if (die.value_is_valid(i)) {
			temp = PermutationCollector::get_permutations(target_value - i, keep - 1, i, die);
			out.collapse(i, temp);
		}
	}
	PermutationCollector::permutations_map.insert({ key, out });
	return out;
}

PermutationVector keep_permutations(int target_value, int keep, dice die) {
	//initialise variables
	PermutationVector out;
	std::vector <int> permutation(keep, 1);
	//sanitise inputs
	if (keep > target_value) {
		throw std::invalid_argument("argument target_value cannot be larger than argument keep");
	}
	if (keep < 1) {
		throw std::invalid_argument("Argument keep must be greater than 0");
	}
	//run code
	
}
std::vector <double> get_probabilities(int max_value, int dice_roll, int dice_keep, int dice_faces) {
	std::vector <double> out;
	int current_value = 0;
	dice die(dice_faces);

	while (current_value < max_value) {
		if (current_value < dice_keep) {
			out.push_back(0);
		}
		else {

		}
	}
	return out;
}

int main() {
	PermutationCollector x;
	dice die(10);
	PermutationVector Y = x.get_permutations(20, 4, 20, die);
	for (int i = 0; i < Y.size(); i++) {
		for (int j = 0; j < Y[i].size(); j++) {
			std::cout << Y[i][j] << ", ";
		}
		std::cout << "\n";
	}
}