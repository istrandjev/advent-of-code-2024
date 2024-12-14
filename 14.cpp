#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <cstdio>
#include <algorithm>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

struct robot {
    int sx, sy;
    int vx, vy;
};

robot parse_robot(const string& s) {
    robot result;
    sscanf(s.c_str(), "p=%d,%d v=%d,%d", &result.sx, &result.sy, &result.vx, &result.vy);
    return result;
}

pair<int, int> get_position(const robot& r, int n, int m, int seconds) {
    ll x = ((ll)r.sx + (ll)r.vx * (ll)seconds) % n;
    x = (x + n) % n;
    ll y = ((ll)r.sy + (ll)r.vy * (ll)seconds) % m;
    y = (y + m) % m;
    return {int(x), int(y)};
}

int get_quadrant(const pair<int, int>& position, int n, int m) {
    int x = position.first;
    int y = position.second;
    if (x == n / 2 || y == m / 2) {
        return -1;
    }
    if (x < n / 2) {
        return int(y < m / 2);
    } else {
        return int(y < m / 2) + 2;
    }
}
ll solve_part1(const vector<robot>& robots, int n, int m, int seconds) {
    vector<ll> quadrants(4);

    for (auto r : robots) {
        auto pos = get_position(r, n, m, seconds);
        int q = get_quadrant(pos, n, m);
        if (q != -1) {
            quadrants[q]++;
        }
    }
    ll res = 1;
    for (auto num : quadrants) {
        res *= num;
    }
    return res;
}

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
void print_map(const vector<robot>& robots, int n, int m, int steps) {
    set<pair<int, int> > points;

    
    for (auto r : robots) {
        auto pos = get_position(r, n, m, steps);
        points.insert(pos);
    }

    vector<pair<int, int> > selected;
    set<pair<int, int> > visited;

    for (auto p : points) {
        if (visited.count(p)) {
            continue;
        }
        queue<pair<int, int> > q;
        q.push(p);
        vector<pair<int, int> > current;
        current.push_back({p.second, p.first});
        while (!q.empty()) {
            pair<int, int> cur = q.front();
            q.pop();
            for (int l = 0; l < 4; ++l) {
                int tx = cur.first + moves[l][0];
                int ty = cur.second + moves[l][1];
                if (tx < 0 || tx >= n || ty < 0 || ty >= m) {
                    continue;
                }
                if (points.count({tx, ty}) == 0 || visited.count({tx, ty})) {
                    continue;
                }
                q.push({tx, ty});
                visited.insert({tx, ty});
                current.push_back({ty, tx});
            }
        }
        if (current.size() > 10) {
            for (auto p : current) {
                selected.push_back(p);
            }
        }
    }

    int minx = n + 1, maxx = 0, miny = m + 1, maxy = 0;
    for (auto p : selected) {
        minx = min(minx, p.first);
        maxx = max(maxx, p.first);
        miny = min(miny, p.second);
        maxy = max(maxy, p.second);
    }
    vector<string> a(maxx - minx + 1, string(maxy - miny + 1, ' '));
    for (auto p : selected) {
        a[p.first - minx][p.second - miny] = '*';
    }
    cout << endl;
    for (auto string_row: a) {
        cout << string_row << endl;
    }
}

void print_map_old(const vector<robot>& robots, int n, int m, int steps) {
    vector<set<int> > per_row(m);
    for (auto r : robots) {
        auto pos = get_position(r, n, m, steps);
        per_row[pos.second].insert(pos.first);
    }

    vector<string> a(m, string(n, ' '));
    for (int i = 0; i < m; ++i) {
        for (auto x : per_row[i]) {
            a[i][x] = '*';
        }
    }
    for (auto string_row: a) {
        cout << string_row << endl;
    }
}

pair<int, int> solve_coordinate(const vector<pair<int, int> >& a, int n) {
    vector<pair<int, int> > helper;
    for (int step = 0; step < n; ++step) {
        vector<int> coords;
        for (int i = 0; i < (int)a.size(); ++i) {
            int x = (a[i].first + step * a[i].second) % n;
            x = (x + n) % n;
            coords.push_back(x);
        }
        int current = 0;
        for (int i = 0; i < (int)coords.size(); ++i) {
            for (int j = i + 1; j < (int)coords.size(); ++j) {
                if (abs(coords[i] - coords[j]) <= 1) {
                    current++;
                }
            }
        }
        helper.push_back({current, step});
    }    
    return *max_element(all(helper));
}

ll chinese_theorem(const vector<pair<int, int> >& a) {
    ll answer = a[0].first;
    ll so_far = a[0].second;
    for (int i = 1; i < (int)a.size(); ++i) {
        for (int j = 0; j < a[i].second; ++j) {
            if (answer % a[i].second == a[i].first) {
                break;
            }
            answer += so_far;
        }
        so_far *= a[i].second;
    }
    return answer;
}

ll solve_part2(const vector<robot>& robots, int n, int m) {
    vector<pair<int, int> > by_x, by_y;
    for (auto r : robots) {
        by_x.push_back({r.sx, r.vx});
        by_y.push_back({r.sy, r.vy});
    }
    auto x_solution = solve_coordinate(by_x, n);
    auto y_solution = solve_coordinate(by_y, m);

    ll result = chinese_theorem({{x_solution.second, n}, {y_solution.second, m}});
    print_map(robots, n, m, result);
    return result;
}

int main() {
    string s;
    vector<robot> robots;
    while (getline(cin, s)) {
        robots.push_back(parse_robot(s));
    }
    cout << s << endl;
    int n = 101, m = 103;
    cout << "Part 1: " << solve_part1(robots, n, m, 100) << endl;
    cout << "Part 2: " << solve_part2(robots, n, m) << endl;
    return 0;
}