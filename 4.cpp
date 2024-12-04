#include <iostream> 
#include <string>
#include <vector>

using namespace std;

int moves[8][2] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
        { 1, -1 }, { 0, -1 }, { -1, -1 } };

bool valid_cell(const vector<string>& a, int x, int y) {
    return x >= 0 && x < (int)a.size() && y >= 0 && y < (int)a[0].size();
}
int dfs(const vector<string>&a, const string& s, int x, int y, int index, int dir) {
    if (index == (int)s.size() - 1) {
        return 1;
    }
    int answer = 0; 
    int tx = x + moves[dir][0];
    int ty = y + moves[dir][1];
    if (valid_cell(a, tx, ty) && a[tx][ty] == s[index + 1]) {
        answer += dfs(a, s, tx, ty, index + 1, dir);
    }
    return answer;
}
int find_words(const vector<string>& a, const string& s) {
    int answer = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        for (int j = 0; j < (int)a[i].size(); ++j) {
            if (a[i][j] == s[0]) {
                for (int dir = 0; dir < 8; ++dir) {
                    answer += dfs(a, s, i, j, 0, dir);
                }
            }
        }
    }
    return answer;
}

int find_crosses(const vector<string>& a) {
    int answer = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        for (int j = 0; j < (int)a[i].size(); ++j) {
            if(a[i][j] != 'A') {
                continue;
            }
            int mas = 0;
            for (int dir = 1; dir < 8; dir += 2) {
                 int ms = 0, ss = 0;
                 for (int step = 0; step <= 4; step += 4) {
                    int tx = i + moves[dir + step][0];
                    int ty = j + moves[dir + step][1];
                    if (valid_cell(a, tx, ty)) {
                        if (a[tx][ty] == 'M') {
                            ms++;
                        } else if (a[tx][ty] == 'S') {
                            ss++;
                        }
                    }
                }
                if (ms == 1 && ss == 1) {
                    mas++;
                }
            }
            if (mas == 2) {
                answer++;
            }
        }
    }
    return answer;
}
int main() {
    vector<string> a;
    string s;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    cout << "Part 1 " << find_words(a, "XMAS") << endl;
    cout << "Part 2 " << find_crosses(a) << endl;
    return 0;
}