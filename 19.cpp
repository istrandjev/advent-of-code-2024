#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

vector<string> split(const string& s, char delim=' ') {
    vector<string> res;
    string cur;
    for (auto c : s) {
        if (c == delim) {
            res.push_back(cur);
            cur = "";
        } else {
            cur.push_back(c);
        }
    }
    if (s.back() != delim) {
        res.push_back(cur);
    }
    return res;
}

string strip(const string& s) {
    int idx = 0;
    while (isspace(s[idx])) {
        idx++;
    }
    string result = s.substr(idx);
    while (result.size() && isspace(result.back())) {
        result.pop_back();
    }
    return result;
}
vector<string> a; 
string target;
ll mem[1000];

#define update(x) (mem[x] != - 1 ? mem[x] : solve(x))
ll solve(int idx) {
    ll answer = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (target.substr(idx, a[i].size()) == a[i]) {
            ll tmp;
            if (idx + a[i].size() == target.size()) {
                tmp = 1;
            } else {
                tmp = update(idx + a[i].size());
            }
            answer += tmp;
        }
    }
    return mem[idx] = answer;
}

ll get_num_ways(const string& pattern) {
    target = pattern;
    memset(mem, -1, sizeof(mem));
    return solve(0);
}

pair<ll, ll> solve_parts(const vector<string>& towels, const vector<string>& patterns) {
    ll answer1 = 0;
    ll answer2 = 0;
    a = towels;
    for (auto p : patterns) {
        ll temp = get_num_ways(p);
        answer1 += (temp != 0);
        answer2 += temp;
    }
    return {answer1, answer2};
}

int main() {
    freopen("advent-of-code-2024/advent.in", "r", stdin);
    string s;
    getline(cin, s);
    vector<string> towels;
    for (auto token : split(s, ',')) {
        towels.push_back(strip(token));
    }
    
    getline(cin, s);
    vector<string> patterns;

    while (getline(cin, s)) {
        patterns.push_back(s);
    }
    auto answer = solve_parts(towels, patterns);
    cout << "Part 1: " << answer.first << endl;
    cout << "Part 2: " << answer.second << endl;
    return 0;
}