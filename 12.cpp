#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

struct group_properties {
    ll area, perimeter, sides;
};

ll get_perimeter_delta(const vector<vector<int> >& a, int i, int j, int index) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    ll perimeter_delta = 0;
    for (int l = 0; l < 4; ++l) {
        int tx = i + moves[l][0];
        int ty = j + moves[l][1];
        if (tx < 0 || tx >= n || ty <0 || ty >=m || a[tx][ty] != index) {
            perimeter_delta++;
        }
    }
    return perimeter_delta;
}

ll get_sides_delta(const vector<vector<int> >& a, int i, int j, int index) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    ll sides_delta = 0;
    for (int l = 0; l < 4; ++l) {
        int tx = i + moves[l][0];
        int ty = j + moves[l][1];

        int prev_x, prev_y;
        if (l % 2 == 0) {
            prev_x = i;
            prev_y = j - 1;
        } else {
            prev_x = i - 1;
            prev_y = j;
        }
        if (tx < 0 || tx >= n || ty <0 || ty >=m) {
            if (prev_x < 0 || prev_y < 0 || a[prev_x][prev_y] != index) {
                sides_delta++;
            }
        } else if (a[tx][ty] != index) {
            if (prev_x < 0 || prev_y < 0) {
                sides_delta++;
            } else if (l % 2 == 0 && (a[tx][prev_y] == index || a[prev_x][prev_y] != index)) {
                sides_delta++;
            } else if (l % 2 == 1 && (a[prev_x][ty] == index || a[prev_x][prev_y] != index)) {
                sides_delta++;
            }
        }
    }
    return sides_delta;
}

group_properties process_group(const vector<vector<int> >& a, int index) {
    int n = (int)a.size();
    int m = (int)a[0].size();

    group_properties result{0, 0, 0};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] != index) {
                continue;
            }
            result.area++;
            result.perimeter += get_perimeter_delta(a, i, j, index);
            result.sides += get_sides_delta(a, i, j, index);
        }
    }
    return result;
}


pair<ll, ll> solve(const vector<string>& a) {
    int n = (int)a.size();
    int m = (int)a[0].size();

    vector<vector<int> > groups(n, vector<int>(m, -1));
    int group_number = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (groups[i][j] != -1) {
                continue;
            }
            groups[i][j] = group_number;
            queue<pair<int, int> > q;
            q.push({i, j});
            while (!q.empty()) {
                auto cur = q.front();
                q.pop();
                for (int l = 0; l < 4; ++l) {
                    int tx = cur.first + moves[l][0];
                    int ty = cur.second + moves[l][1];

                    if (tx < 0 || tx >= n || ty <0 || ty >=m) {
                        continue;
                    }
                    if (groups[tx][ty] != -1 || a[tx][ty] != a[i][j]) {
                        continue;
                    }
                    groups[tx][ty] = group_number;
                    q.push({tx, ty});
                }
            }
            group_number++;
        }
    }
    ll answer1 = 0, answer2 = 0;
    for (int gn = 0; gn < group_number; ++gn) {
        auto temp = process_group(groups, gn);
        answer1 += temp.area * temp.perimeter;
        answer2 += temp.area * temp.sides;
    }
    return {answer1, answer2};
}

int main() {
    vector<string> a;
    string s;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    auto answer = solve(a);
    cout << "Part 1: " << answer.first << endl;
    cout << "Part 1: " << answer.second << endl;
    return 0;
}