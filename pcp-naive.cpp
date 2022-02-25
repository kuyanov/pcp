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

int c1, c2;

vector<pair<string, string> > all;

string ans;
string cur1, cur2;

void gen1(int pos, int len) {
	if (pos >= len) return;
	if (abs((int)cur1.size() - (int)cur2.size()) > c2) return;
	if (pos > 0 && cur1.size() == cur2.size()) {
		ans = cur1;
		return;
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

void solve() {
	gen1(0, c1); // Change
	if (!ans.empty()) {
		cout << "YES" << endl;
		cout << ans.size() << endl;
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
	cur1.reserve(2000), cur2.reserve(2000);
	//cin >> c1 >> c2;
	c1 = 300, c2 = 1000;
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
	return 0;
}