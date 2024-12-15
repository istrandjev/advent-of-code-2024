#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef long long ll;

ll encode_box(int x, int y) {
    return x * 100 + y;
}

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

int move_to_index(char c) {
    switch(c) {
       case '<': return 3;
       case 'v': return 2;
       case '>': return 1;
       default: return 0;
    }
}

void do_move(char c, vector<string>& a, int& x, int& y) {
    int l = move_to_index(c);

    int tx = x + moves[l][0];
    int ty = y + moves[l][1];
    if (a[tx][ty] == '#') {
    } else if (a[tx][ty] == '.') {
        a[tx][ty] = a[x][y];
        a[x][y] = '.';
        x = tx;
        y = ty;
    } else if (a[tx][ty] == 'O' || l % 2 == 1) {
        int ttx = tx;
        int tty = ty;
        do_move(c, a, ttx, tty);
        if (tx != ttx || ty != tty) {
            swap(a[tx][ty], a[x][y]);
            x = tx;
            y = ty;
        }
    } else {
        vector<string> temp = a;
        int ttx1 = tx, tty1 = ty;
        int ttx2 = tx, tty2 = (a[tx][ty] == '[' ? ty + 1 : ty -1);
        int original_tx2 = ttx2, original_ty2 = tty2;
        do_move(c, temp, ttx1, tty1);
        do_move(c, temp, ttx2, tty2);
        bool moved = (tx != ttx1 || ty != tty1) && (original_tx2 != ttx2 || original_ty2 != tty2);
        if (moved) {
            a = temp;
            swap(a[tx][ty], a[x][y]);
            x = tx;
            y = ty;
        }
    }
}
ll solve(const vector<string>&field, const string& moves) {
    vector<string> a = field;
    int n = (int)a.size();
    int m = (int)a[0].size();
    
    int sx, sy;
    for (int i = 0;i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == '@') {
                a[i][j] = '.';
                sx = i;
                sy = j;
            }
        }
    }
    int cx = sx, cy = sy;
    for (char move : moves) {
        do_move(move, a, cx, cy);
    }

    ll answer = 0;
    for (int i = 0;i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] == 'O' || a[i][j] == '[') {
                answer += encode_box(i, j);
            }
        }
    }
    return answer;
}

vector<string> resize_map(const vector<string>&a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    vector<string> result(n, string(m * 2, '.'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            switch(a[i][j]) {
                case '#': {
                    result[i][j * 2] = '#';
                    result[i][j * 2 + 1] = '#';
                    break;
                } case 'O': {
                    result[i][j * 2] = '[';
                    result[i][j * 2 + 1] = ']';
                    break;
                } case '.': {
                    result[i][j * 2] = '.';
                    result[i][j * 2 + 1] = '.';
                    break;
                } default: {
                    result[i][j * 2] = '@';
                    result[i][j * 2 + 1] = '.';
                }
            }
        }
    }
    return result;
}

int main() {
    string s;
    vector<string> a;
    bool reading_moves = false;
    string moves;
    while(getline(cin, s)) {
        if (s.size() == 0) {
            reading_moves = true;
            continue;
        }
        if (!reading_moves) {
            a.push_back(s);
        } else {
            moves += s;
        }
    }
    cout << "Part 1: " << solve(a, moves) << endl;
    cout << "Part 2: " << solve(resize_map(a), moves) << endl;
    return 0;
}