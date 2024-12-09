#include <iostream> 
#include <string>
#include <vector>
#include <map>

using namespace std;
typedef long long ll;

vector<ll> compress(vector<ll> result) {
    int idx = 0;
    while (idx < (int)result.size() && result[idx] != -1) {
        idx++;
    }
    for (int i = (int)result.size() - 1; i >= 0 && i > idx; --i) {
        if (result[i] == -1) {
            continue;
        }
        result[idx] = result[i];
        result[i] = -1;
        while (idx < i && result[idx] != -1) {
            idx++;
        }
    }
    return result;
}
ll get_sum(const vector<ll>& s) {
    ll result = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] != -1) {
            result += (ll)i * s[i];
        }
    }
    return result;
}
ll solve_part1(const string& s) {
    vector<ll> result;
    for (int i = 0; i < (int)s.size(); ++i) {
        ll c;
        if (i % 2 == 0) {
            c = i / 2;
        } else {
            c = -1;
        }
        for (int j = 0; j < (int)(s[i] - '0'); ++j) {
            result.push_back(c);
        }
    }
    result = compress(result);
    return get_sum(result);
}

map<int, int> get_free_intervals(const vector<ll>& result) {
    map<int, int> answer;
    for (int i = 0; i < (int)result.size(); ++i) {
        if (result[i] == -1) {
            int j;
            for (j = i + 1; j < (int)result.size() && result[j] == -1; ++j);
            int len = j - i;
            answer[i] = len;
            i = j - 1;
        }
    }
    return answer;
}
vector<ll> compress2(vector<ll> result) {
    map<int, int> free_intervals = get_free_intervals(result);
    int last_considered = 1000000000;
    for (int i = (int)result.size() - 1; i >= 0; --i) {
        if (result[i] == -1 || result[i] >=  last_considered) {
            continue;
        }
        int len = 1;
        while (i - len >= 0 && result[i - len] == result[i]) {
            len++;
        }
        last_considered = result[i];

        for (auto free_int : free_intervals) {
            if (free_int.first >= i) {
                break;
            }
            if (free_int.second >= len) {
                int index = free_int.first;
                for (int j = 0; j < len; ++j) {
                    result[i - j] = -1;
                    result[index + j] = last_considered;
                }
                int remaining = free_int.second - len;
                if (remaining > 0) {
                    free_intervals[index + len] = remaining;
                }
                free_intervals.erase(index);
                break;
            }
        }    
        i -= len - 1;
    }
    return result;
}

ll solve_part2(const string& s) {
    vector<ll> result;
    for (int i = 0; i < (int)s.size(); ++i) {
        ll c;
        if (i % 2 == 0) {
            c = i / 2;
        } else {
            c = -1;
        }
        for (int j = 0; j < (int)(s[i] - '0'); ++j) {
            result.push_back(c);
        }
    }
    result = compress2(result);
    return get_sum(result);
}
int main() {
    string s;
    getline(cin, s);
    cout << "Part 1: " << solve_part1(s) << endl;
    cout << "Part 2: " << solve_part2(s) << endl;
    return 0;
}