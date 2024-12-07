#include <iostream> 
#include <string>
#include <vector>

using namespace std;
typedef long long ll;


struct equation {
    ll value;
    vector<ll> operands;
};

bool possible(const equation& e) {
    int n = (int)e.operands.size();

    for (ll mask = 0; mask < (1 << (n - 1)); ++mask) {
        ll temp = 0;
        for (int i = 1; i < n; ++i) {
            if (mask & (1 << (i - 1))) {
                temp = temp + e.operands[i];
            } else {
                temp = temp * e.operands[i];
            }
        }
        if (temp == e.value) {
            return true;
        }
    }
    return false;
}

ll concatenate(ll x, ll y) {
    ll p = 1;
    while (p <= y) {
        p *= 10;
    }
    return x * p + y;
}
bool possible2(const equation& e) {
    int n = (int)e.operands.size();

    ll max_mask = 1;
    for (int i = 0; i < n - 1; ++i) {
        max_mask *= 3;
    }

    for (ll mask = 0; mask < max_mask; ++mask) {
        ll temp = 0;
        ll cmask = mask;
        for (int i = 1; i < n; ++i) {
            if (cmask % 3 == 0) {
                temp = temp + e.operands[i];
            } else if (cmask % 3 == 1) {
                temp = temp * e.operands[i];
            } else {
                temp = concatenate(temp, e.operands[i]);
            }
            cmask /= 3;
        }
        if (temp == e.value) {
            return true;
        }
    }
    return false;
}

ll solve_part1(const vector<equation>& equations) {
    ll result = 0;
    for (auto e : equations) {
        if (possible(e)) {
            result += e.value;
        }
    }
    return result;
}

ll solve_part2(const vector<equation>& equations) {
    ll result = 0;
    for (auto e : equations) {
        if (possible2(e)) {
            result += e.value;
        }
    }
    return result;
}


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
equation parse_equation(string s) {
    vector<string> parts = split(s, ':');
    equation result;
    result.value = atoll(parts[0].c_str());
    vector<string> operand_strings = split(parts[1], ' ');
    for (auto os : operand_strings) {
        result.operands.push_back(atoll(os.c_str()));
    }
    return result;
}

int main() {
    vector<equation> equations;
    string s;
    while (getline(cin, s)) {
        equations.push_back(parse_equation(s));
    }
    cout << "Part 1: " << solve_part1(equations) << endl;    
    cout << "Part 1: " << solve_part2(equations) << endl;
    return 0;
}