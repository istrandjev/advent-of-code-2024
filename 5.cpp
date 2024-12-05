#define _CRT_SECURE_NO_DEPRECATE
#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;
typedef long long ll;

pair<int, int> parse_rule(const string& s) {
    pair<int, int> result;
    sscanf(s.c_str(), "%d|%d", &result.first, &result.second);
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
vector<int> parse_print(const string& s) {
    vector<string> parts = split(s, ',');
    vector<int> result;
    for (auto ps : parts) {
        result.push_back(atoi(ps.c_str()));
    }
    return result;
}
bool in_correct_order(const vector<int>& print, const map<int, set<int> >& required) {
    for (int i = 0; i < (int)print.size(); ++i) {
        for (int j = i + 1; j < (int)print.size(); ++j) {
            auto it = required.find(print[i]);
            if (it != required.end() && it->second.count(print[j])) {
                return false;
            }
        }
    }
    return true;
}
ll solve_part1(const vector<vector<int> >& prints, const map<int, set<int> >& required) {
    ll res = 0;
    for (auto print : prints) {
        if (in_correct_order(print, required)) {
            res += print[print.size() / 2];
        }
    }
    return res;
}

vector<int> correct_pages_order(const vector<int>& print, const map<int, set<int> >& required) {
    vector<int> corrected;
    set<int> already_added;
    while (corrected.size() < print.size()) {
        for (int i = 0; i < (int)print.size(); ++i) {
            if (already_added.count(print[i])) {
                continue;
            }
            bool good = true;
            for (int j = i + 1; j < (int)print.size(); ++j) {
                if (already_added.count(print[j])) {
                    continue;
                }
                auto it = required.find(print[i]);
                if (it != required.end() && it->second.count(print[j])) {
                    good = false;
                    break;
                }
            }
            if (good) {
                corrected.push_back(print[i]);
                already_added.insert(print[i]);
                break;
            }
        }
        
    }

    return corrected;
}
ll solve_part2(const vector<vector<int> >& prints, const map<int, set<int> >& required) {
    ll res = 0;
    for (auto print : prints) {
        if (!in_correct_order(print, required)) {
            vector<int> corrected = correct_pages_order(print, required);
            res += corrected[corrected.size() / 2];
        }
    }
    return res;
}
int main() {
    vector<pair<int, int> > rules;
    vector<vector<int> > prints;
    string s;
    while (getline(cin, s) && s.size() > 0) {
        rules.push_back(parse_rule(s));
    }
    map<int, set<int> > required;
    for (pair<int, int> rule: rules) {
        required[rule.second].insert(rule.first);
    }
    while (getline(cin, s)) {
        prints.push_back(parse_print(s));
    }
    
    cout << "Part 1: " << solve_part1(prints, required) << endl;
    cout << "Part 2: " << solve_part2(prints, required) << endl;
    return 0;
}