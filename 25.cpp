#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;


vector<int> parse_lock(const vector<string>& a) {
    vector<int> res(a[0].size());
    for (int j = 0; j < (int)a[0].size(); ++j) {
        int h;
        for (h = 0; h < (int)a.size() && a[h][j] == '#'; h++);
        res[j] = h; 
    }
    return res;
}

bool fit(const vector<int>& lock, const vector<int>& key) {
    for (int i = 0; i < (int)lock.size(); ++i) {
        if (lock[i] + key[i] > 7) {
            return false;
        }
    }
    return true;
}

int solve(const vector<vector<int> >& locks, const vector<vector<int> >& keys) {
    int answer = 0;
    for (auto l : locks) {
        for (auto k : keys) {
            if (fit(l, k)) {
                answer++;
            }
        }
    }
    return answer;
}

int main() {
    string s;
    vector<vector<int> > locks, keys;
    while(getline(cin, s)) {
        vector<string> a(1, s);
        while (getline(cin, s)) {
            if (s.size() == 0) {
                break;
            }
            a.push_back(s);
        }
        if (a[0][0] == '#') {
            locks.push_back(parse_lock(a));
        } else {
            reverse(all(a));
            keys.push_back(parse_lock(a));
        }
    }

    cout << "Part 1: " << solve(locks, keys) << endl;
    return 0;
}