#include <iostream>
#include <queue>
#include <string>
#include <vector>


using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
ll count_trailheads(const vector<string>& a, int x, int y) {
    int n = (int)a.size();
    int m = (int)a[0].size();

    queue<pair<int, int> > q;
    q.push({x, y});
    vector<vector<int> > dist(n, vector<int>(m, -1));
    dist[x][y] = 0;
    ll answer = 0;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        for (int l =  0; l < 4; ++l) {
            int tx = cur.first + moves[l][0];
            int ty = cur.second + moves[l][1];
            if (tx < 0 || tx >= n || ty < 0 || ty >= m) {
                continue;
            }
            if (a[tx][ty] != a[cur.first][cur.second] + 1) {
                continue;
            }
            if (dist[tx][ty] != -1) {
                continue;
            }
            dist[tx][ty] = dist[cur.first][cur.second] + 1;
            if (a[tx][ty] == '9') {
                answer++;
            }
            q.push({tx, ty});
        }
    }
    return answer;
}
ll hiking_trails(const vector<string>& a, int x, int y) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    ll answer = 0;
    if (a[x][y] == '9') {
        return 1;
    }
    for (int l =  0; l < 4; ++l) {
        int tx = x + moves[l][0];
        int ty = y + moves[l][1];
        if (tx < 0 || tx >= n || ty < 0 || ty >= m) {
            continue;
        }
        if (a[tx][ty] != a[x][y] + 1) {
            continue;
        }
        answer += hiking_trails(a, tx, ty);
    }
    return answer;
}
ll solve_part1(const vector<string>& a) {
    ll answer = 0;
    int n = (int)a.size();
    int m = (int)a[0].size();
    for (int i=  0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == '0') {
                answer += count_trailheads(a, i, j);
            }
        }
    }
    return answer;
}

ll solve_part2(const vector<string>& a) {
    ll answer = 0;
    int n = (int)a.size();
    int m = (int)a[0].size();
    for (int i=  0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == '0') {
                answer += hiking_trails(a, i, j);
            }
        }
    }
    return answer;
}

int main() {
    string s;
    vector<string> a;
    
    while(getline(cin, s)) {
        a.push_back(s);
    }
    

    cout << "Part 1: " << solve_part1(a) << endl;
    cout << "Part 1: " << solve_part2(a) << endl;
    return 0;
}