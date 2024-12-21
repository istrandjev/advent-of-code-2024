#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <map>
#include <math.h>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
string move_chars = "^>v<";
vector<string> num_pad{"789", "456", "123", "#0A"};
vector<string> robot_pad{"#^A", "<v>"};

map<char, map<char, string> > num_shortest_paths, robot_shortest_paths;

string go_up(const string& s) {
    map<char, pair<int, int> > to_coordinates;
    for (int i = 0; i < (int)robot_pad.size(); ++i) {
        for (int j = 0; j < (int)robot_pad[i].size(); ++j) {
            to_coordinates[robot_pad[i][j]] = {i, j};
        }
    }
    pair<int, int> prev = to_coordinates['A'];

    string result;
    for (int i = 0; i < (int)s.size(); ++i) {
        pair<int, int> cur = to_coordinates[s[i]];
        int dx = cur.second - prev.second;
        int dy = cur.first - prev.first;
        for (int i = 0; i < dy; ++i) {
            result.push_back('v');
        }

        for (int i = 0; i < abs(dx); ++i) {
            result.push_back(dx < 0 ? '<' : '>');
        }
        for (int i = 0; i < -dy; ++i) {
            result.push_back('^');
        }
        result.push_back('A');
        prev = cur;
    }
    return result;
}
int get_transition_cost(const string& s) {
    return go_up(go_up(go_up(go_up(go_up(s))))).size();
}
string reconstruct_shortest_path(char c, const map<char, pair<char, char> >& vis, bool is_robot, char source) {
    char cur = c;
    auto parent = vis.find(cur);
    string result;
    while (parent->second.first != cur) {
        result.push_back(parent->second.second);
        cur = parent->second.first;
        parent = vis.find(cur);
    }
    sort(all(result));
    string order = "A^>v<";

    if (is_robot){
        order = "Av<>^";
    } else {
        order = "A^<>v";      
    }
    bool can_switch = false;
    if (is_robot) {
        can_switch = (source != '<' && c != '<');
    } else {
        set<char> borders{'A', '0', '1', '4', '7'};
        can_switch = ((borders.count(c) + borders.count(source)) != 2);
    }
    vector<int> helper;
    for (auto c : result) {
        for (int i = 0; i < order.size(); ++i) {
            if (order[i] == c) {
                helper.push_back(i);
                break;
            }
        }
    }
    sort(all(helper));
    for (int i = 0; i < (int)result.size(); ++i) {
        result[i] = order[helper[i]];
    }
    result.push_back('A');
    if (can_switch) {
        string temp = result;
        int res = get_transition_cost(temp);
        reverse(all(temp));
        temp = temp.substr(1);
        temp += "A";
        if (get_transition_cost(temp) < res) {
            result = temp;
        }
    }
    return result;
}
string invert(const string& s, const vector<string>& pad) {
    int n = (int)pad.size();
    int m = (int)pad[0].size();
    pair<int, int> cur;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (pad[i][j] == 'A') {
                cur = {i, j};
            }
        }
    }
    int char_to_idx[256];
    for (int i = 0; i < (int)move_chars.size(); ++i) {
        char_to_idx[move_chars[i]] = i;
    }
    string result;
    for (char c : s) {
        if (c == 'A') {
            result.push_back(pad[cur.first][cur.second]);
        }
        int l = char_to_idx[c];
        cur.first += moves[l][0];
        cur.second += moves[l][1];
    }
    return result;
}
map<char, map<char, string> > get_shortest_paths(const vector<string>& pad) {
    int n = (int)pad.size();
    int m = (int)pad[0].size();
    map<char, map<char, string> > shortest_paths;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (pad[i][j] == '#') {
                continue;
            }
            queue<pair<int, int> > q;
            map<char, pair<char, char> > vis;
            vis.insert({pad[i][j], {pad[i][j], '\0'}});
            q.push({i, j});
            while (!q.empty()) {
                pair<int, int> cur = q.front();
                q.pop();
                for (int l = 0; l < 4; ++l) {
                    pair<int, int> temp{cur.first + moves[l][0], cur.second + moves[l][1]};
                    if (temp.first < 0 || temp.first >= n || temp.second < 0 || temp.second >= m) {
                        continue;
                    }
                    if (pad[temp.first][temp.second] == '#') {
                        continue;
                    }
                    char c = pad[temp.first][temp.second];
                    if (vis.count(c)) {
                        continue;
                    }
                    vis[c] = {pad[cur.first][cur.second], move_chars[l]};
                    q.push(temp);
                }
            }
            map<char, string> current_shortest_paths;
            for (int i1 = 0; i1 < n; ++i1) {
                for (int j1 = 0; j1 < m; ++j1) {
                    if (i1 == i && j1 == j || pad[i1][j1] == '#') {
                        continue;
                    }
                    current_shortest_paths[pad[i1][j1]] = reconstruct_shortest_path(pad[i1][j1], vis, pad==robot_pad, pad[i][j]);
                }
            }
            shortest_paths[pad[i][j]] = current_shortest_paths;
        }
    }
    return shortest_paths;
}
string get_shortest(const string& s, const vector<string>& pad, const map<char, map<char, string> >& shortest_paths) {
    char current = 'A';
    string result;
    for (char c : s) {
        if (c != current) {
            auto it = shortest_paths.find(current);
            auto it2 = it->second.find(c);
            result += it2->second;
            current = c;
        } else {
            result.push_back('A');
        }
        
    }
    return result;
}

map<pair<string, int>, ll> mem;

ll get_length_helper(const string& target, int depth) {
    if (depth == 0) {
        mem[{target, depth}] = (ll)target.size();
        return (ll)target.size();
    }
    auto it = mem.find({target, depth});
    if (it != mem.end()) {
        return it->second;
    }
    ll answer = 0;
    int start = 0;
    string s = get_shortest(target, robot_pad, robot_shortest_paths);
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == 'A') {
            answer += get_length_helper(s.substr(start, i - start + 1), depth - 1);
            start = i + 1;
        }
    }
    mem[{target, depth}] = answer;
    return answer;
}
ll get_length(const string& target, int depth) {
    string s = get_shortest(target, num_pad, num_shortest_paths);
    int start = 0;
    ll answer = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == 'A') {
            answer += get_length_helper(s.substr(start, i - start + 1), depth - 1);
            start = i + 1;
        }
    }
    return answer;
}

ll solve_parts(const vector<string>& a, int depth) {
    ll answer = 0;
    for (auto s : a) {
        ll temp = atoll(("1" + s.substr(0, 3)).c_str()) - 1000;
        ll length = get_length(s, depth);
        answer += temp * length;
    }
    return answer;
}

int main() {
    string s;
    vector<string> a;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    num_shortest_paths = get_shortest_paths(num_pad);
    robot_shortest_paths = get_shortest_paths(robot_pad);
    cout << "Part 1: " << solve_parts(a, 3) << endl;
    cout << "Part 2: " << solve_parts(a, 26) << endl;

    return 0; 
}