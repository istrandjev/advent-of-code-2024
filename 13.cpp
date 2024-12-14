#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
typedef long long ll;

struct Machine {
    ll dx, dy;
};

struct Game {
    Machine a, b;
    pair<ll, ll> prize;
};

string leave_only_digits(const string& s) {
    string result;
    for (auto c : s) {
        if (!isdigit(c)) {
            result.push_back(' ');
        } else {
            result.push_back(c);
        }
    }
    return result;
}

Game parse_game(const string& s) {
    Game result;
    istringstream iss(leave_only_digits(s));
    iss >> result.a.dx >> result.a.dy;
    iss >> result.b.dx >> result.b.dy;
    iss >> result.prize.first >> result.prize.second;
    return result;
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
    string s;
    vector<Game> games;
    
    while (true) {
        string current;
        for (int i = 0; i < 3; ++i) {
            getline(cin, s);
            current += "\n" + s;
        }
        
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

