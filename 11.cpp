#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;
typedef long long ll;

vector<ll> process_stone(ll value) {
    if (value == 0) {
        return vector<ll>(1, 1);
    }
    ostringstream out;
    out << value;
    string s = out.str();
    if (s.size() % 2 == 0) {
        vector<ll> result;
        result.push_back(atoll(s.substr(0, s.size() / 2).c_str()));
        result.push_back(atoll(s.substr(s.size() / 2, s.size()).c_str()));
        return result;
    }
    return vector<ll>(1, value * 2024);
}
map<pair<ll, int>, ll> mem;

ll solve_stone(ll value, int times) {
    auto it = mem.find({value, times});
    if (it != mem.end()) {
        return it->second;
    }
    if (times == 0) {
        mem[{value, times}] = 1;
        return 1;
    }
    auto next_stones = process_stone(value);
    ll result = 0;
    for (auto next_stone : next_stones) {
        result += solve_stone(next_stone, times - 1);
    }
    mem[{value, times}] = result;
    return result;
}
ll solve_part1(const vector<ll>& stones, int times) {
    ll answer = 0 ;
    for (auto stone : stones) {
        answer += solve_stone(stone, times);
    }
    return answer;
}


int main() {
    vector<ll> stones;
    ll temp;
    while(cin >> temp) {
        stones.push_back(temp);
    }
    cout << "Part 1: " << solve_part1(stones, 25) << endl;
    cout << "Part 1: " << solve_part1(stones, 75) << endl;
    return 0;
}