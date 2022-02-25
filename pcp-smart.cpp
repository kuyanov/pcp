#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace std;

int c1, c2, c3;

vector<pair<string, string> > all;

unordered_map<string, string> s1, s2;
string ans;
string cur1, cur2;

void gen1(int pos, int len) {
	//cout << cur1 << endl << cur2 << endl;
	//cout << endl;
	if (pos >= len) return;
	if (abs((int)cur1.size() - (int)cur2.size()) > c3) return;
	string diff = cur1.size() < cur2.size() ? cur2.substr(cur1.size()) : cur1.substr(cur2.size());
	if (pos > 0) {
		if (cur1.size() < cur2.size()) {
			if (!s2[diff].empty() && s2[diff].size() <= cur1.size()) return;
			s2[diff] = cur1;
		}
		else {
			if (!s1[diff].empty() && s1[diff].size() <= cur1.size()) return;
			s1[diff] = cur1;
		}
	}
	int sz1 = cur1.size(), sz2 = cur2.size();
	int l = min(sz1, sz2);
	for (auto elem : all) {
		cur1 += elem.first, cur2 += elem.second;
		int r = min(cur1.size(), cur2.size());
		if (cur1.substr(l, r - l) == cur2.substr(l, r - l)) {
			gen1(pos + 1, len);
		}
		for (int i = cur1.size() - 1; i >= sz1; i--) cur1.pop_back();
		for (int i = cur2.size() - 1; i >= sz2; i--) cur2.pop_back();
	}
}

unordered_map<string, int> used1, used2;

void gen2(int pos, int len) {
	if (pos >= len) return;
	if (abs((int)cur1.size() - (int)cur2.size()) > c3) return;
	if (!ans.empty()) return;
	string diff = cur1.size() < cur2.size() ? cur2.substr(cur1.size()) : cur1.substr(cur2.size());
	reverse(diff.begin(), diff.end());
	auto it1 = used1.find(diff), it2 = used2.find(diff);
	if (it1 != used1.end() && (*it1).second <= pos) return;
	if (it2 != used2.end() && (*it2).second <= pos) return;
	if (pos > 0) {
		if (cur1.size() < cur2.size()) {
			if (diff.size() < 22) used1[diff] = pos;
			auto it = s1.find(diff);
			if (it != s1.end()) {
				reverse(cur1.begin(), cur1.end());
				ans = (*it).second + cur1;
			}
		}
		else {
			if (diff.size() < 22) used2[diff] = pos;
			auto it = s2.find(diff);
			if (it != s2.end()) {
				reverse(cur1.begin(), cur1.end());
				ans = (*it).second + cur1;
			}
		}
	}
	int sz1 = cur1.size(), sz2 = cur2.size();
	int l = min(sz1, sz2);
	for (auto elem : all) {
		for (char c : elem.first) cur1.push_back(c);
		for (char c : elem.second) cur2.push_back(c);
		int r = min(cur1.size(), cur2.size());
		if (cur1.substr(l, r - l) == cur2.substr(l, r - l)) {
			gen2(pos + 1, len);
		}
		for (int i = cur1.size() - 1; i >= sz1; i--) cur1.pop_back();
		for (int i = cur2.size() - 1; i >= sz2; i--) cur2.pop_back();
	}
}

void solve() {
	gen1(0, c1); // Change
	cout << "Done gen1" << endl;
	for (auto& elem : all) reverse(elem.first.begin(), elem.first.end()),
		reverse(elem.second.begin(), elem.second.end());
	gen2(0, c2); // Change
	for (auto& elem : all) reverse(elem.first.begin(), elem.first.end()),
		reverse(elem.second.begin(), elem.second.end());
	cout << "Done gen2" << endl;
	if (!ans.empty()) {
		cout << "YES" << endl;
		cout << ans << endl;
	}
	else {
		cout << "NO" << endl;
	}
}

vector<int> parse() {
	int n = (int)ans.size();
	vector<vector<pair<bool, int> > > dp(n + 1, vector<pair<bool, int> >(n + 1, { 0, -1 }));
	dp[0][0] = { 1, -1 };
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (!dp[i][j].first) continue;
			for (int ind = 0; ind < (int)all.size(); ind++) {
				string s1 = all[ind].first, s2 = all[ind].second;
				if (ans.substr(i, s1.size()) == s1 && ans.substr(j, s2.size()) == s2) {
					dp[i + s1.size()][j + s2.size()] = { 1, ind };
				}
			}
		}
	}
	int i = n, j = n;
	vector<int> ans_ind;
	while (i > 0 && j > 0) {
		int ind = dp[i][j].second;
		if (ind == -1) {
			return {};
		}
		ans_ind.push_back(ind);
		int len1 = all[ind].first.size(), len2 = all[ind].second.size();
		i -= len1, j -= len2;
	}
	reverse(ans_ind.begin(), ans_ind.end());
	return ans_ind;
}

bool check(vector<int> ans_ind) {
	if (ans_ind.empty()) return 0;
	string s1, s2;
	for (auto ind : ans_ind) {
		s1 += all[ind].first, s2 += all[ind].second;
	}
	return s1 == s2;
}

int main()
{
	s1.reserve(2000), s2.reserve(2000);
	c1 = 50, c2 = 100, c3 = 100;
	//cin >> c1 >> c2 >> c3;
	double start = clock();
	int n;
	cin >> n;
	all.resize(n);
	for (int i = 0; i < n; i++) {
		string s1, s2;
		cin >> s1 >> s2;
		all[i] = { s1, s2 };
	}
	solve();
	auto ans_ind = parse();
	cout << (int)ans_ind.size() << endl;
	for (int i : ans_ind) {
		cout << i << ' ';
	}
	cout << endl;
	bool ok = check(ans_ind);
	if (ok) cout << "OK" << endl;
	else cout << "Fail" << endl;
	double tm = (clock() - start) / CLOCKS_PER_SEC;
	cout << "Time: " << tm << endl;

	/*int c;
	cin >> c;
	if (c == 1) {
		cout << "52\n1 0 1 1 0 1 0 1 1 0 1 1 2 1 1 2 2 1 0 0 0 1 0 1 1 1 1 0 1 1 2 1 2 1 1 2 2 2 1 0 0 0 2 1 1 1 2 1 2 2 0 2\n";
	}
	else if (c == 2) {
		cout << "55\n2 0 0 2 0 2 0 1 2 0 1 2 0 0 1 1 1 2 0 1 0 0 1 1 2 0 1 0 1 1 2 0 0 0 1 2 0 2 0 1 1 2 0 1 1 0 1 1 0 1 0 1 1 0 1\n";
	}
	else if (c == 3) {
		cout << "96\n0 2 2 0 0 1 0 2 2 2 1 0 0 0 2 1 0 2 2 1 0 2 1 0 0 2 1 0 2 0 2 2 1 0 2 1 0 0 2 2 0 0 2 1 0 2 1 1 0 0 1 1 0 2 1 1 1 1 2 2 1 1 2 2 0 1 0 0 1 2 1 1 2 1 0 2 1 0 2 1 0 2 1 0 2 1 0 2 1 0 2 1 0 2 1 1\n";
	}
	else if (c == 4) {
		cout << "112\n1 0 0 1 1 1 0 2 1 0 0 1 2 1 1 1 0 0 2 1 0 2 1 1 2 1 2 1 0 0 0 1 0 0 2 1 1 1 1 0 0 1 1 2 1 0 2 1 0 0 1 1 1 0 2 2 1 2 1 1 1 0 2 1 0 2 1 0 0 2 1 0 2 1 2 1 2 1 1 2 1 2 2 0 1 0 2 2 0 0 1 1 0 2 1 1 1 0 2 1 2 1 0 2 1 2 2 1 2 0 2 2\n";
	}
	else if (c == 5) {
		cout << "119\n1 0 1 1 0 2 1 0 1 0 1 1 0 1 1 0 2 1 1 0 2 1 0 1 0 2 1 2 1 1 0 1 2 0 2 2 1 1 0 2 0 1 0 2 1 1 1 0 1 0 1 0 2 2 1 1 0 1 1 0 1 0 2 2 1 1 0 2 1 1 0 1 0 2 2 1 0 1 0 2 1 1 0 1 0 2 1 1 2 1 0 2 1 1 2 1 0 2 0 2 1 2 2 0 2 1 1 2 0 2 1 0 2 2 1 2 2 0 2\n";
	}
	else if (c == 6) {
		cout << "132\n0 0 1 1 2 1 0 0 2 0 0 2 1 2 1 1 2 1 2 1 1 2 0 0 0 0 1 2 0 0 0 0 1 2 1 2 1 2 1 2 1 2 2 1 2 1 2 1 2 1 2 0 0 1 2 0 0 1 2 1 2 2 0 0 1 2 2 1 2 1 2 2 1 2 1 2 2 1 2 1 2 1 2 1 2 2 1 2 2 1 2 0 0 1 2 2 1 2 1 2 2 1 2 1 2 1 2 2 1 2 0 0 1 2 1 2 2 1 2 1 2 2 1 2 2 1 2 1 2 2 1 2\n";
	}
	else if (c == 7) {
		cout << "216\n0 1 0 0 1 0 1 2 0 1 1 1 0 0 0 0 1 0 1 0 1 0 1 0 0 1 0 0 1 0 0 1 2 0 1 0 0 1 0 1 2 0 1 0 1 1 1 2 0 1 2 0 1 1 1 2 0 2 1 1 0 1 1 1 0 2 1 1 1 1 2 0 0 0 0 1 1 1 0 2 1 1 0 1 0 1 0 1 0 0 0 0 1 1 1 2 0 0 1 0 0 1 2 0 1 0 1 0 1 0 2 1 1 0 1 2 0 1 0 1 1 1 2 0 0 1 0 0 1 1 1 2 0 1 1 1 2 0 2 1 1 0 1 2 0 1 0 2 1 1 0 2 1 1 1 1 2 0 1 1 1 2 1 1 2 1 1 0 2 1 1 0 2 1 0 1 1 2 1 1 2 1 1 0 1 2 1 0 1 1 2 0 1 1 0 1 2 1 1 0 2 0 1 1 2 1 1 1 2 1 2 1 1 1 2 1\n";
	}
	else if (c == 8) {
		cout << "252\n2 2 1 2 2 2 2 1 2 2 1 2 2 2 2 1 2 2 0 2 1 0 2 2 2 1 2 0 2 2 0 2 1 2 2 0 2 2 1 2 0 2 2 2 1 2 2 2 1 2 2 0 2 2 1 0 2 2 2 1 0 2 2 2 1 2 2 2 1 2 2 2 0 2 1 2 2 2 2 2 1 0 2 2 2 1 0 2 0 2 1 2 0 2 2 2 1 0 2 1 2 2 2 0 2 1 2 2 2 0 0 2 0 0 2 1 2 2 2 0 2 0 2 1 2 0 2 2 2 1 0 2 0 2 0 2 0 2 1 2 0 0 2 0 0 2 1 2 2 2 0 0 0 0 2 0 2 0 2 0 2 2 2 1 0 2 2 2 0 0 0 0 2 1 2 2 2 2 2 1 0 2 2 2 0 2 2 2 1 0 2 1 2 2 2 0 2 1 2 0 2 1 2 2 2 0 2 0 2 1 2 0 2 0 0 2 0 2 1 2 0 0 2 0 0 2 0 0 2 0 2 0 2 0 2 0 0 0 0 2 2 2 0 2 1 2 0 2 0 0 2 0\n";
	}
	else if (c == 9) {
		cout << "216\n2 0 2 2 2 0 2 0 2 2 2 0 2 2 1 0 1 2 2 0 2 1 2 2 1 0 2 2 1 2 0 2 1 2 2 0 2 2 0 2 2 1 2 0 1 2 2 0 1 2 2 0 2 2 0 2 2 2 1 0 2 2 2 2 0 1 2 2 0 1 2 1 0 2 1 2 2 0 1 0 2 2 2 1 0 2 2 2 1 1 2 1 1 0 2 2 2 1 2 1 0 2 1 2 2 0 1 2 1 2 1 2 1 0 2 1 1 2 1 1 0 2 2 2 1 1 1 1 2 1 2 1 2 1 2 2 0 1 2 2 2 1 1 1 1 0 2 2 2 2 0 1 2 2 2 1 2 2 0 1 0 2 2 2 1 0 2 1 0 2 2 2 1 2 1 0 2 1 2 1 1 2 1 0 2 1 1 2 1 1 2 1 1 2 1 2 1 2 1 2 1 1 1 1 2 2 2 1 0 2 1 2 1 1 2 1\n";
	}
	else if (c == 10) {
		cout << "216\n2 2 1 2 1 0 2 1 1 1 2 2 2 2 1 2 1 2 1 2 1 2 2 1 2 2 1 2 2 1 0 2 1 2 2 1 2 1 0 2 1 2 1 1 1 0 2 1 0 2 1 1 1 0 2 0 1 1 2 1 1 1 2 0 1 1 1 1 0 2 2 2 2 1 1 1 2 0 1 1 2 1 2 1 2 1 2 2 2 2 1 1 1 0 2 2 1 2 2 1 0 2 1 2 1 2 1 2 0 1 1 2 1 0 2 1 2 1 1 1 0 2 2 1 2 2 1 1 1 0 2 1 1 1 0 2 0 1 1 2 1 0 2 1 2 0 1 1 2 0 1 1 1 1 0 2 1 1 1 0 1 1 0 1 1 2 0 1 1 2 0 1 2 1 1 0 1 1 0 1 1 2 1 0 1 2 1 1 0 2 1 1 2 1 0 1 1 2 0 2 1 1 0 1 1 1 0 1 0 1 1 1 2 0 1 1\n";
	}
	else if (c == 11) {
		cout << "216\n2 1 2 2 1 2 1 0 2 1 1 1 2 2 2 2 1 2 1 2 1 2 1 2 2 1 2 2 1 2 2 1 0 2 1 2 2 1 2 1 0 2 1 2 1 1 1 0 2 1 0 2 1 1 1 0 2 0 1 1 2 1 1 1 2 0 1 1 1 1 0 2 2 2 2 1 1 1 2 0 1 1 2 1 2 1 2 1 2 2 2 2 1 1 1 0 2 2 1 2 2 1 0 2 1 2 1 2 1 2 0 1 1 2 1 0 2 1 2 1 1 1 0 2 2 1 2 2 1 1 1 0 2 1 1 1 0 2 0 1 1 2 1 0 2 1 2 0 1 1 2 0 1 1 1 1 0 2 1 1 1 0 1 1 0 1 1 2 0 1 1 2 0 1 2 1 1 0 1 1 0 1 1 2 1 0 1 2 1 1 0 2 1 1 2 1 0 1 1 2 0 2 1 1 0 1 1 1 0 1 0 1 1 1 0 1\n";
	}
	else if (c == 12) {
		cout << "216\n1 0 1 1 1 0 1 0 1 1 1 0 1 1 2 0 2 1 1 0 1 2 1 1 2 0 1 1 2 1 0 1 2 1 1 0 1 1 0 1 1 2 1 0 2 1 1 0 2 1 1 0 1 1 0 1 1 1 2 0 1 1 1 1 0 2 1 1 0 2 1 2 0 1 2 1 1 0 2 0 1 1 1 2 0 1 1 1 2 2 1 2 2 0 1 1 1 2 1 2 0 1 2 1 1 0 2 1 2 1 2 1 2 0 1 2 2 1 2 2 0 1 1 1 2 2 2 2 1 2 1 2 1 2 1 1 0 2 1 1 1 2 2 2 2 0 1 1 1 1 0 2 1 1 1 2 1 1 0 2 0 1 1 1 2 0 1 2 0 1 1 1 2 1 2 0 1 2 1 2 2 1 2 0 1 2 2 1 2 2 1 2 2 1 2 1 2 1 2 1 2 2 2 2 1 1 1 2 0 1 2 1 2 2 1 2\n";
	}
	else if (c == 13) {
		cout << "240\n1 1 0 1 1 2 1 0 1 1 2 2 1 0 1 1 1 2 2 2 0 1 1 1 1 1 2 1 0 1 2 0 1 1 2 2 2 1 2 1 0 1 1 1 1 2 2 2 2 0 1 2 1 1 1 1 2 2 1 0 1 1 0 1 1 1 2 2 1 0 1 1 2 1 0 1 2 1 2 2 1 0 1 1 2 2 2 2 1 2 2 1 0 1 1 1 1 1 2 1 2 2 1 0 1 2 0 1 1 2 1 2 2 2 1 2 1 0 1 1 2 1 1 2 2 2 1 0 1 1 1 0 1 1 1 1 2 2 1 0 1 2 2 2 0 1 2 1 2 2 2 1 1 1 2 2 2 1 1 1 0 1 2 1 1 1 0 1 2 2 2 2 0 1 1 2 2 1 1 1 1 2 1 0 1 1 1 2 0 1 2 2 2 2 0 1 1 1 2 2 1 1 1 1 2 1 0 1 2 1 2 0 1 2 2 2 2 2 1 2 2 1 1 1 1 2 1 2 0 1 2 2 1 2 2 1 2 1 2 2\n";
	}
	else if (c == 14) {
		cout << "217\n1 0 2 1 1 0 0 2 1 2 1 1 0 1 0 0 2 1 0 2 3 1 1 0 2 2 1 0 0 2 1 1 1 0 2 3 1 1 0 0 0 2 2 1 0 0 2 3 3 1 1 1 0 2 1 2 1 0 0 0 0 2 1 1 0 1 0 2 1 2 3 3 1 1 0 0 2 1 0 2 1 1 0 0 0 0 2 3 1 1 0 2 1 1 0 0 2 3 3 3 2 1 0 0 2 1 1 0 0 2 3 2 0 1 0 2 3 1 1 0 0 2 3 2 1 3 1 0 2 2 1 0 0 2 3 2 3 0 0 2 1 1 1 0 2 3 2 2 3 3 1 1 0 0 0 2 2 1 1 0 0 0 2 3 3 1 1 1 0 0 2 3 3 1 0 0 0 0 2 3 1 0 0 2 3 3 3 2 1 0 2 3 2 0 1 0 2 2 1 3 1 0 2 1 3 0 0 2 3 0 2 3 2 3 2 2 3\n";
	}
	else if (c == 15) {
		cout << "273\n1 2 1 1 2 1 2 1 1 2 1 1 2 1 2 1 1 2 1 2 3 0 2 1 2 1 1 2 1 0 0 2 1 1 2 1 2 3 0 0 0 2 1 2 1 1 2 1 0 0 0 0 2 1 1 2 1 2 1 1 2 3 3 3 3 1 1 2 1 2 3 0 2 1 2 1 2 2 1 2 1 2 1 1 2 1 0 0 2 1 1 2 1 3 0 2 1 1 2 1 2 3 0 0 0 2 1 2 1 1 2 0 0 2 1 2 1 1 2 1 0 0 0 0 2 1 1 2 1 2 1 3 3 3 0 2 1 2 3 0 0 0 0 0 2 1 2 3 0 2 2 0 0 2 1 0 0 0 0 0 0 2 1 0 0 3 3 3 0 0 0 0 0 0 0 2 1 1 0 2 2 3 3 3 3 3 3 3 1 1 2 1 2 3 3 2 2 2 1 2 1 2 1 1 2 1 2 1 1 3 0 2 1 1 2 1 2 3 0 2 1 2 0 0 2 1 2 1 1 2 1 0 0 2 1 3 3 3 0 2 1 2 3 0 0 0 2 2 0 0 2 1 0 0 0 0 3 3 3 0 0 0 0 2 2 3 3 3 3 3 2 2 3\n";
	}
	else if (c == 16) {
		cout << "302\n2 3 0 2 2 3 2 3 0 3 3 0 2 0 0 3 2 1 1 1 2 3 0 2 3 0 2 2 3 0 2 2 3 2 3 0 2 2 3 2 3 0 3 3 0 2 2 3 2 1 2 3 0 2 0 0 3 2 3 2 3 0 3 1 3 3 0 2 2 1 1 3 0 3 3 0 3 3 0 2 0 0 0 3 2 3 2 1 0 2 0 0 1 0 2 2 1 1 1 1 2 1 2 3 0 3 2 1 1 1 2 2 3 2 1 0 3 1 3 3 0 2 0 3 3 0 2 3 2 1 2 1 2 0 0 0 3 2 1 3 0 0 3 2 1 2 1 3 1 3 1 1 1 1 2 1 0 1 1 2 1 3 1 0 0 0 3 1 2 0 3 3 0 0 1 1 3 0 2 1 3 0 0 1 1 0 2 2 1 0 1 3 0 2 2 3 2 3 0 1 0 3 2 3 2 3 0 3 3 0 2 0 1 2 3 0 3 3 0 2 0 0 3 2 1 1 3 3 0 2 0 0 3 2 1 1 3 0 3 1 0 0 3 2 1 1 3 0 3 3 0 0 1 0 1 3 0 3 1 0 2 0 0 1 3 0 1 0 1 0 3 2 1 1 1 0 1 2 0 1 2 1 0 1 3 1 1 3 1 2 0 0 0 3 1 1 1 1 0 0 1 1\n";
	}
	else if (c == 17) {
		cout << "193\n3 1 3 3 3 0 3 3 2 3 3 2 2 3 2 0 1 0 3 3 2 2 2 0 1 2 0 3 2 0 1 2 2 2 0 1 3 1 2 2 0 1 2 2 3 3 0 3 0 1 2 0 1 0 3 3 2 2 3 2 2 0 1 0 3 2 0 1 2 0 1 2 2 0 3 2 1 2 0 1 0 1 2 3 2 2 3 0 1 0 3 3 2 0 1 2 0 1 0 3 2 0 1 2 0 1 0 3 2 2 2 0 1 0 3 2 2 2 2 2 0 3 2 2 2 2 2 2 2 3 2 2 2 2 2 2 2 2 0 1 2 2 2 2 2 2 2 2 0 1 2 2 2 2 2 2 2 0 1 2 2 2 2 2 2 0 1 2 2 2 2 2 0 1 2 2 2 2 0 1 2 2 2 0 1 2 2 0 1 2 0 1 0\n";
	}
	else if (c == 18) {
		cout << "198\n2 1 2 2 0 2 1 0 2 3 1 2 2 2 0 2 1 2 1 0 2 2 0 2 3 1 2 1 0 2 2 2 0 2 2 1 2 1 2 1 2 1 0 3 2 0 2 2 0 2 2 0 2 2 1 2 2 1 0 2 1 0 2 1 0 3 2 0 2 1 2 3 1 2 3 1 2 3 2 1 0 2 2 0 2 2 2 0 2 2 2 0 2 2 1 2 3 1 2 1 0 3 1 0 3 1 0 3 2 0 2 2 2 0 2 3 2 3 2 3 2 1 0 3 1 0 2 3 2 1 2 3 3 2 3 1 2 3 2 0 2 2 3 2 2 0 2 2 1 0 3 2 1 2 1 0 3 2 1 2 3 2 0 2 2 1 2 3 2 0 2 2 1 0 3 2 0 2 2 1 0 3 3 2 1 0 3 3 2 3 3 2 3 2 3 3 2 3\n";
	}
	else if (c == 19) {
		cout << "200\n0 3 3 2 3 0 0 3 3 3 3 3 2 3 2 3 2 2 2 0 0 3 3 3 3 2 3 0 3 2 0 3 3 3 2 3 3 2 3 2 0 3 2 2 3 3 2 0 0 3 2 3 3 3 3 2 0 3 2 3 2 3 3 2 2 0 3 2 0 3 3 2 3 3 2 3 0 0 3 2 0 3 3 3 3 3 2 3 3 2 3 2 3 2 0 3 2 2 2 3 3 2 0 0 0 3 2 3 3 3 3 3 3 2 0 3 2 3 2 3 2 3 3 2 2 2 2 3 0 0 0 0 0 3 3 3 3 3 3 3 3 3 3 3 0 3 2 3 2 3 2 3 2 3 2 3 3 3 2 2 2 2 2 2 3 2 1 0 0 3 3 3 3 3 3 3 0 3 2 3 2 3 2 3 3 3 2 2 2 2 3 2 1 3 3 3 2 3 2 2\n";
	}
	else if (c == 20) {
		cout << "206\n3 2 2 1 2 1 1 1 3 3 1 3 2 3 1 2 1 3 2 1 1 3 1 2 1 3 3 2 1 1 3 1 3 2 2 1 3 3 0 2 2 1 2 1 3 1 3 2 1 1 2 1 1 1 3 1 3 1 2 1 3 2 1 3 3 1 3 1 1 1 3 1 2 1 3 1 3 2 3 1 3 3 1 1 1 3 1 3 1 2 1 3 0 2 3 0 3 2 3 1 1 1 3 1 1 1 3 0 2 2 1 3 0 3 0 3 2 1 1 2 1 3 1 3 1 3 1 2 1 3 2 1 3 1 3 1 3 1 1 1 3 1 2 1 3 1 3 1 3 1 3 3 1 1 1 3 1 3 1 3 1 3 1 3 0 3 2 3 1 3 1 3 1 3 1 3 1 2 1 3 0 2 3 1 3 1 3 1 1 1 3 1 1 1 3 0 0 0 3 0 3 0 3 0 0 0\n";
	}
	else if (c == 21) {
		cout << "256\n3 2 3 3 2 3 2 3 3 2 3 3 2 3 2 3 3 2 3 2 3 3 0 2 3 2 3 3 0 1 3 2 3 3 3 0 1 3 1 3 0 2 3 1 3 2 3 2 1 1 3 0 3 2 2 2 3 2 3 3 1 2 1 3 2 3 3 0 3 3 0 1 1 3 0 1 3 0 1 3 1 2 1 3 2 3 2 1 3 2 3 2 1 1 3 0 1 3 2 3 3 1 2 1 3 1 3 0 1 2 3 2 1 1 3 1 3 3 1 2 1 2 3 2 3 2 2 1 3 3 2 3 3 0 2 2 3 2 3 0 1 3 0 3 3 0 2 2 3 2 3 2 1 3 2 3 3 1 3 0 3 3 0 1 3 0 1 1 3 0 1 3 2 3 2 1 3 2 3 2 2 2 1 3 1 3 0 1 3 2 3 3 0 3 2 1 1 3 1 3 0 1 3 2 3 3 1 2 1 1 3 1 3 0 1 1 2 1 1 3 1 2 1 2 1 2 3 2 1 3 3 1 3 1 2 3 2 2 2 3 3 0 3 3 1 3 0 1 1 3 1 2 1 1 2 1\n";
	}*/

	return 0;
}