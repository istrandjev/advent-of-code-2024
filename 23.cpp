#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <math.h>
#include <stack>
#include <deque>
#include <numeric>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <regex>

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

struct Graph {
    vector<set<int> > ne;
    vector<string> reverse_codes;
    set<int> historian;
};

Graph get_neighbours_list(const vector<pair<string, string> >& string_edges) {
    unordered_map<string, int> int_codes;
    vector<string> reverse_codes;
    vector<pair<int, int> > edges;
    set<int> historian;
    auto to_int = [&](const string& s) {
        auto it = int_codes.find(s);
        int code;
        if (it == int_codes.end()) {
            code = int_codes.size();
            int_codes[s] = code;
            reverse_codes.push_back(s);
        } else {
            code = it->second;
        }
        if (s[0] == 't') {
            historian.insert(code);
        }
        return code;
    };
    for (auto e: string_edges) {
        edges.push_back({to_int(e.first), to_int(e.second)});
    }
    int n = (int)int_codes.size();
    vector<set<int> > ne(n);
    for (auto e : edges) {
        ne[e.first].insert(e.second);
        ne[e.second].insert(e.first);
    }
    return {ne, reverse_codes, historian};
}

ll solve_part1(const vector<pair<string, string> >& string_edges) {
    auto graph = get_neighbours_list(string_edges);
    auto ne = graph.ne;
    auto historian = graph.historian;
    ll answer = 0;

    set<vector<int>> found;
    for (int hi : historian) {
        vector<int> cne(all(ne[hi]));
        for (int i1 = 0; i1 < (int)cne.size(); ++i1) {
            int u = cne[i1];
            for (int i2 = i1 + 1; i2 < (int)cne.size(); ++i2) {
                int v = cne[i2];
                if (ne[u].count(v)) {
                    vector<int> cur{hi, u, v};
                    sort(all(cur));
                    found.insert(cur);
                }
            }
        }
    }

    return found.size();
}

bool are_all_connected(const set<vector<int>>& found, const vector<set<int> >& ne) {
    set<int> codes;
    for (auto group : found) {
        for (auto v : group) {
            codes.insert(v);
        }
    }
    for (auto u : codes) {
        for (auto v : codes) {
            if (u == v) {
                continue;
            }
            if (ne[u].count(v) != 1) {
                return false;
            }
        }
    }
    return true;
}
string solve_part2(const vector<pair<string, string> >& string_edges) {
    auto graph = get_neighbours_list(string_edges);
    auto ne = graph.ne;
    auto reverse_codes = graph.reverse_codes;
    int n = (int)ne.size();
    ll answer = 0;

    set<vector<int>> found;
    for (int hi = 0; hi < n; ++hi) {
        vector<int> cne(all(ne[hi]));
        for (int i1 = 0; i1 < (int)cne.size(); ++i1) {
            int u = cne[i1];
            for (int i2 = i1 + 1; i2 < (int)cne.size(); ++i2) {
                int v = cne[i2];
                if (ne[u].count(v)) {
                    vector<int> cur{hi, u, v};
                    sort(all(cur));
                    found.insert(cur);
                }
            }
        }
    }
    while (!are_all_connected(found, ne)) {
        map<int, int> cnt;
        for (auto group : found) {
            for (int v : group) {
                cnt[v]++;
            }
        }
        int mini = -1, min_cnt = found.size() * 3;
        for (auto it : cnt) {
            if (mini == -1 || it.second < min_cnt) {
                min_cnt = it.second;
                mini = it.first;
            }
        }

        vector<vector<int> > to_remove;
        for (auto group : found) {
            for (int v : group) {
                if (v == mini) {
                    to_remove.push_back(group);
                    break;
                }
            }
        }
        for (auto group : to_remove) {
            found.erase(group);
        }
    }

    set<string> names;
    for (auto group : found) {
        for (auto v : group) {
            names.insert(reverse_codes[v]);
        }
    }
    string result;
    for (auto it : names) {
        result += it;
        result.push_back(',');
    }
    result.pop_back();

    return result;
}

int main() {
    freopen("advent-of-code-2024/advent.in", "r", stdin);
    
    vector<pair<string, string> > connections;
    string s;
    while (getline(cin, s)) {
        vector<string> temp = split(s, '-');
        connections.push_back({temp[0], temp[1]});
    }
    cout << "Part 1: " << solve_part1(connections) << endl;
    cout << "Part 2: " << solve_part2(connections) << endl;
    return 0;
}