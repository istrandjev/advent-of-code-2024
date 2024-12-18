#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <cstdio>

using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
ll solve_part1(const vector<pair<int, int> >& a, int limit, int n, int m) {
    queue<pair<int, int> > q;
    q.push({0, 0});
    set<pair<int, int> > obstacles;
    for (int i = 0; i <= limit; ++i) {
        obstacles.insert(a[i]);
    }
    vector<vector<int> > vis(n, vector<int>(m, -1));
    vis[0][0] = 0;
    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        for (int l = 0; l < 4; ++l) {
            pair<int, int> temp{
                cur.first + moves[l][0],
                cur.second + moves[l][1]
            };
            if (temp.first < 0 || temp.first >= n || temp.second < 0 || temp.second >= m) {
                continue;
            }
            if (obstacles.count(temp)) {
                continue;
            }
            if (vis[temp.first][temp.second] != -1) {
                continue;
            }
            vis[temp.first][temp.second] = vis[cur.first][cur.second] + 1;
            q.push(temp);
        }
    }
    return vis[n - 1][m - 1];
}

ll solve_part2(const vector<pair<int, int> >& a, int n, int m) {
    int beg = 0, end = (int)a.size();
    while (end - beg > 1) {
        int mid = (end + beg) / 2;
        if (solve_part1(a, mid, n, m) == -1) {
            end = mid;
        } else {
            beg = mid;
        }
    }
    return end;
}
vector<pair<int, int> > parse_input() {
    vector<pair<int, int> > a;
    string s;
    while (getline(cin, s)) {
        pair<int, int> b;
        sscanf(s.c_str(), "%d,%d",&b.second, &b.first);
        a.push_back(b);
    }
    return a;
}
int main() {
    auto a = parse_input();
    cout << "Part 1: " << solve_part1(a, 1024, 71, 71) << endl;
    int index = solve_part2(a, 71, 71);
    cout << "Part 2: " << a[index].second << "," << a[index].first << endl;
    return 0;
}