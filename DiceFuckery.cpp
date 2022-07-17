#include <iostream>
#include <vector>
#include <array>
#include <map>

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
	return true;
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

class PermutationsVector : private std::vector <std::vector <int>> {
public:
	using vector::operator[];
	using vector::push_back;
	using vector::size;
	void collect(int a);
	void send();
private:
	std::vector <int> collection;
	void collapse(int i, PermutationsVector vec);
};

void PermutationsVector::collect(int a) {
	PermutationsVector::collection.push_back(a);
}

void PermutationsVector::send() {
	PermutationsVector::push_back(PermutationsVector::collection);
	PermutationsVector::collection = {};
}

void PermutationsVector::collapse(int starting_value, PermutationsVector vec) {
	std::vector <int> temp;
	for (int i = 0; i < vec.size(); i++) {
		temp = { starting_value };
		for (int j = 0; j < vec[i].size(); j++) {
			temp.push_back(vec[i][j]);
		}
		PermutationsVector::push_back(temp);
	}
}

PermutationsVector keep_permutations(int target_value, int keep, dice die) {
	//initialise variables
	PermutationsVector out;
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

