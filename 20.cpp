#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
bool in_grid(const pair<int, int>& t, int n, int m) {
    if (t.first < 0 || t.first >= n || t.second < 0 || t.second >= m) {
        return false;
    }
    return true;
}
vector<vector<int> > get_dist(const vector<string>& a, const pair<int, int>& start, const pair<int, int>& end) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    vector<vector<int> > dist(n, vector<int>(m, -1));
    queue<pair<int, int> > q;
    q.push(start);
    dist[start.first][start.second] = 0;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        for (int l = 0; l < 4; ++l) {
            pair<int, int> t(cur.first + moves[l][0], cur.second + moves[l][1]);
            if (!in_grid(t, n, m)) {
                continue;
            }
            if (a[t.first][t.second] == '#' || dist[t.first][t.second] != -1) {
                continue;
            }
            dist[t.first][t.second] = dist[cur.first][cur.second] + 1;
            q.push(t);
        }
    }
    return dist;
}

ll combs[60][60] = {0};
ll solve(const vector<string>& a, const pair<int, int>& start, const pair<int, int>& end, int cheat_limit, bool different_cheats) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    auto dist = get_dist(a, start, end);
    auto rdist = get_dist(a, end, start);
    int original = dist[end.first][end.second];
    ll answer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == '#') {
                continue;
            }
            for (int dx = -cheat_limit; dx <= cheat_limit; ++dx) {
                for (int dy = -(cheat_limit - abs(dx)); dy <= (cheat_limit - abs(dx)); ++dy) {
                    int tx = i + dx;
                    int ty = j + dy;
                    if (!in_grid({tx, ty}, n, m)) {
                        continue;
                    }
                    if (a[tx][ty] == '#') {
                        continue;
                    }
                    int temp = dist[i][j] + rdist[tx][ty] + abs(dx) + abs(dy) - 1;
                    int saved = original - temp;
                    if (saved >= 100) {
                        if (!different_cheats) {
                            answer++;
                        } else {
                            answer += combs[abs(dx) + abs(dy)][abs(dx)];
                        }
                    }
                }
            }
        }
    }
    return answer;
}

int main() {
    string s;
    vector<string> a;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    pair<int, int> start, end;
    combs[0][0] = 1;
    for (int n = 1; n < 60; ++n) {
        combs[n][0] = combs[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            combs[n][k] = combs[n - 1][k - 1] + combs[n - 1][k];
        }
    }
    for (int i = 0; i < (int)a.size(); ++i) {
        for (int j = 0; j < (int)a[i].size(); ++j) {
            if (a[i][j] == 'S') {
                start = {i, j};
                a[i][j] = '.';
            }
            if (a[i][j] == 'E') {
                end = {i, j};
                a[i][j] = '.';
            }
        }
    }
    cout << "Part 1: " << solve(a, start, end, 2, false) << endl;
    cout << "Part 2: " << solve(a, start, end, 20, false) << endl;
    cout << "Part 3: " << solve(a, start, end, 20, true) << endl;
    return 0;
}