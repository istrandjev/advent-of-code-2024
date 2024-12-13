#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;
typedef long long ll;

struct Machine {
    ll dx, dy;
};

struct Game {
    Machine a, b;
    pair<ll, ll> prize;
};

Game parse_game(const string& s) {
    regex e("Button .: X\\+([0-9]+), Y\\+([0-9]+)");
    smatch m; 
    regex_search(s, m, e);
    Machine a{atoll(m[1].str().c_str()), atoll(m[2].str().c_str())};
    string t = m.suffix().str();
    regex_search(t, m, e);
    Machine b{atoll(m[1].str().c_str()), atoll(m[2].str().c_str())};
    regex e2("Prize: X=([0-9]+), Y=([0-9]+)");
    t = m.suffix().str();
    regex_search(t, m, e2);
    return Game{a, b, {atoll(m[1].str().c_str()), atoll(m[2].str().c_str())}};
}

ll solve_game(const Game& game) {
    ll num = game.prize.second * game.a.dx - game.a.dy * game.prize.first;
    ll denom = game.b.dy * game.a.dx - game.a.dy * game.b.dx;
    if (num % denom) {
        return -1;
    }
    ll v = num / denom;
    if (v < 0) {
        return -1;
    }
    ll num2 = game.prize.first - v * game.b.dx;
    ll denom2 = game.a.dx;
    if (num2 % denom2) {
        return -1;
    }
    ll u = num2 / denom2;
    if (u < 0) {
        return -1;
    }
    return u * 3LL + v;
}

ll solve_games(const vector<Game>& games) {
    ll answer = 0;
    for (auto game : games) {
        ll temp = solve_game(game);
        if (temp != -1) {
            answer += temp;
        }
    }
    return answer;
}

int main() {
    freopen("advent-of-code-2024/advent.in", "r", stdin);
    string s;
    vector<Game> games;

    
    while (true) {
        if (!getline(cin, s)) {
            break;
        }
        string current = s;
        getline(cin, s);
        current += "\n" + s;
        getline(cin, s);
        current += "\n" + s;
        games.push_back(parse_game(current));
        if (!getline(cin, s)) {
            break;
        }
    }
    cout << "Part 1: " << solve_games(games) << endl;

    for (int i = 0; i < games.size(); ++i) {
        games[i].prize.first += 10000000000000LL;
        games[i].prize.second += 10000000000000LL;
    }
    cout << "Part 2: " << solve_games(games) << endl;
    return 0;
}