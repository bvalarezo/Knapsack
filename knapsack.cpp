/*******************************************************************************
 * Name        : knapsack.h
 * Author      : Bryan Valarezo
 * Version     : 1.0
 * Date        : 12/3/2016
 * Description : Implementation of the knapsack algorithm
 * Pledge      : I pledge my honor that I've abided by the Stevens Honor System
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

// creates a matrix which contains the best choices.
vector<vector<int> > create_matrix(int n, int w, vector<vector<int> > k) {
	vector<vector<int> > items(n + 1, vector<int>(w + 1));
	for (int j = 0; j < w; j++) {
		items[0][j] = 0;
	}
	for (int i = 0; i < n; i++) {
		items[i][0] = 0;
	}
	//done making matrix
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= w; j++) {

			int loseit = items[i - 1][j];

			if (j >= k[0][i - 1]) {
				int useit = k[1][i - 1] + items[i - 1][j - k[0][i - 1]];
				items[i][j] = max(useit, loseit);
			}
			if (j < k[0][i - 1]) {
				items[i][j] = loseit;
			}
		}
	}
	return items;
}

//trace back how to select the best choices
vector<int> knapsack(vector<vector<int> > k, vector<int> weights) {
	vector<int> items;
	int i = k.size() - 1;
	int j = k[0].size() - 1;
	while (!(i == 0 || j == 0)) {
		if (k[i][j] != k[i - 1][j]) {
			items.push_back(i - 1);
			j -= weights[i - 1];

		}
		i--;
	}
	reverse(items.begin(), items.end());
	return items;
}

int main(int argc, char * const argv[]) {
	int n;

	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <capacity> <filename>" << endl;
		return 1;
	}

	istringstream iss(argv[1]);

	if (!(iss >> n) || n < 0) {
		cerr << "Error: Bad value '" << argv[1] << "' for capacity." << endl;
		return 1;
	}
	iss.clear();

	string line;
	ifstream myfile(argv[2]);
	vector<string> items;
	vector<int> weights;
	vector<int> values;

	if (myfile.is_open()) {
		int count = 1;
		while (getline(myfile, line)) {
			istringstream ss(line);
			string key;

			getline(ss, key, ',');
			items.push_back(key);

			if (ss.eof()) {
				cerr << "Error: Line number " << count
						<< " does not contain 3 fields." << endl;
				return 1;
			}
			getline(ss, key, ',');
			istringstream isss(key);
			int weight;
			if (!(isss >> weight) || weight < 0) {
				cerr << "Error: Invalid weight '" << key
						<< "' on line number " << count << ".";
				return 1;
			} else {
				weights.push_back(weight);
			}

			if (ss.eof()) {
				cerr << "Error: Line number " << count
						<< " does not contain 3 fields." << endl;
				return 1;
			}

			getline(ss, key, ',');
			istringstream issss(key);
			int value;
			if (!(issss >> value) || value < 0) {
				cerr << "Error: Invalid value '" << key << "' on line number "
						<< count << ".";
				return 1;
			} else {
				values.push_back(value);
			}

			if (!ss.eof()) {
				cerr << "Error: Line number " << count
						<< " does not contain 3 fields." << endl;
				return 1;
			}
			count++;
		}
		myfile.close();

	} else {
		cout << "Error: Cannot open file '" << argv[2] << "'.";
		return 1;
	}


	vector<vector<int> > k;
	k.push_back(weights);
	k.push_back(values);

	cout << "Candidate items to place in knapsack:" << endl;

	vector<int> result = knapsack(create_matrix(weights.size(), n, k), weights);
	for (unsigned int b = 1; b <= items.size(); b++) {
		cout << "   Item " << b << ": " << items[b - 1] << " ("
				<< weights[b - 1] << " pound";
		if (weights[b - 1] > 1) {
			cout << "s";
		}
		cout << ", $" << values[b - 1] << ")" << endl;

	}
	cout << "Capacity: " << argv[1] << " pound";
	if (n != 1) {
		cout << "s";
	}
	cout << endl;
	cout << "Items to place in knapsack:";
	if (result.size() == 0) {
		cout << " None" << endl;
	} else {
		cout << endl;
		for (unsigned int b = 1; b <= result.size(); b++) {
			cout << "   Item " << result[b - 1] + 1 << ": "
					<< items[result[b - 1]] << " (" << weights[result[b - 1]]
					<< " pound";
			if (weights[result[b - 1]] > 1) {
				cout << "s";
			}
			cout << ", $" << values[result[b - 1]] << ")" << endl;
		}
	}

	int w = 0;
	int v = 0;
	for (unsigned int b = 1; b <= result.size(); b++) {
		w += weights[result[b - 1]];
		v += values[result[b - 1]];
	}
	cout << "Total weight: " << w << " pound";
	if (w != 1) {
		cout << "s";
	}
	cout << endl;
	cout << "Total value : $" << v << endl;
	return 0;

}
