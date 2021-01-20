#include <cstdio>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <iostream>
#include <string>
#include <regex>

#include <cinttypes>

using namespace std;

char line[1024];
unordered_map <string, int64_t> bagstotal;
unordered_map<string, vector<pair<string, int>>> container_map;

int64_t count_bags(string &key) {
	auto ret = bagstotal.find(key);
	if (ret != bagstotal.end()) return ret->second;

	int64_t val = 1;
	vector < pair<string, int>> &vec = container_map[key];
	for (pair<string, int> &p : vec) {
		val += count_bags(p.first)*p.second;
	}

	bagstotal[key] = val;

	return val;
}

int main() {
	FILE *fin = fopen("input.txt", "r");
	if (!fin) {
		fprintf(stderr, "Can't open input\n");
		return 1;
	}


	std::regex container_splitter(" bags contain");
	std::regex contents_splitter(" bags?[\\.,]");
	std::regex bag_quant_name_re("\\s*(\\d+) (.*)");


	// default constructor = end-of-sequence:
	std::regex_token_iterator<std::string::iterator> rend;
	int submatches[] = { 1, 2 };

	while (fgets(line, sizeof(line), fin)) {
		string s(line);
		std::regex_token_iterator<std::string::iterator> a(s.begin(), s.end(), container_splitter, -1);
		string container(a->str());
		auto retinsert = container_map.insert(make_pair(container, vector<pair<string, int>>()));
		//cout << "[" << a->str() << "]" << endl;
		a++;
		string s1(a->str());
		std::regex_token_iterator<std::string::iterator> b(s1.begin(), s1.end(), contents_splitter, -1);

		for (; b != rend; b++) {
			string s2(b->str());

			std::regex_token_iterator<std::string::iterator> c(s2.begin(), s2.end(), bag_quant_name_re, submatches);
			if (c != rend) {

				//cout << "#" << c->str() << "#";
				int n = stoi(c->str());
				c++;
				//cout << "(" << c->str() << ")" << endl;

				string content(c->str());

				retinsert.first->second.push_back(make_pair(content, n));
			}

		}
	}
	fclose(fin);

	cout << count_bags(string("shiny gold")) - 1 << endl;

	return 0;
}