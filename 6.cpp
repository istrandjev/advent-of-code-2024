#include <iostream> 
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

struct result_part1 {
    set<pair<int, int> > locations;
    int startx, starty;
};
result_part1 solve_part1(vector<string> a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    int cx, cy;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] != '#' && a[i][j] != '.') {
                cx = i;
                cy = j;
            }
        }
    }
    int dir = 0;
    int startx = cx, starty = cy;
    set<pair<int, int> > locations;
    while (true) {
        locations.insert({cx, cy});
        int tx = cx + moves[dir][0];
        int ty = cy + moves[dir][1];
        if (tx < 0 || tx >= n || ty < 0 || ty >=m) {
            break;
        }
        if (a[tx][ty] == '#') {
            dir = (dir + 1) % 4;
            continue;
        }

        cx = tx;
        cy = ty;
    }
    locations.erase({startx, starty});
    return {locations, startx, starty};
}

int encode(int x, int y, int dir) {
    return x * 1000 * 10 + y * 10 + dir;
}
bool is_cycle(const vector<string>& a, int startx, int starty) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    int cx = startx, cy = starty;
    unordered_set<int> visited;
    int dir = 0;
    while (true) {
        if (visited.count(encode(cx, cy, dir))) {
            return true;
        }
        visited.insert(encode(cx, cy, dir));
        int tx = cx + moves[dir][0];
        int ty = cy + moves[dir][1];
        if (tx < 0 || tx >= n || ty < 0 || ty >=m) {
            break;
        }
        if (a[tx][ty] == '#') {
            dir = (dir + 1) % 4;
            continue;
        }

        cx = tx;
        cy = ty;
    }
    return false;
}

ll solve_part2(vector<string> a) {
    auto part1 = solve_part1(a);
    int answer = 0;
    for (pair<int, int> option : part1.locations) {
        a[option.first][option.second] = '#';
        if (is_cycle(a, part1.startx, part1.starty)) {
            answer++;
        }
        a[option.first][option.second] = '.';
    }
    return answer;
}
int main() {
    vector<string> a;
    string s;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    cout << "Part 1: " << solve_part1(a).locations.size() + 1 << endl;
    cout << "Part 2: " << solve_part2(a) << endl;
    
    return 0;
}