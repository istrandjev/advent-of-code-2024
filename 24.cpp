#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <map>
#include <cstdio>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

vector<string> split(const string& s, char delim=' ') {
    vector<string> res;
    string cur;
    for (auto c : s) {
        if (c == delim) {
            res.push_back(cur);
            cur = "";
        } else {
            cur.push_back(c);
        }
    }
    if (s.back() != delim) {
        res.push_back(cur);
    }
    return res;
}

struct Command {
    string lhs, rhs;
    string operation;
    string result;
};

ostream& operator<<(ostream& out, const Command& c) {
    out << c.lhs << " " << c.operation << " " << c.rhs << " -> " << c.result << endl;
    return out;
}
vector<int> topo_sort(const vector<vector<int> > &ne) {
    int n = ne.size();
    queue<int> q;
    vector<int> in_degree(n);
    vector<int> answer;
    for (int i = 0; i < n; ++i) {
        for (int v : ne[i]) {
            in_degree[v]++;
        }
    }
    for (int i = 0; i < (int)in_degree.size(); ++i) {
        if (in_degree[i] == 0) {
            answer.push_back(i);
            q.push(i);
        }
    }
    while (!q.empty()) {
        int c = q.front();
        q.pop();
        for (int v : ne[c]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                answer.push_back(v);
                q.push(v);
            }
        }
    }
    return answer;
}

int compute_value(const Command& command, const map<string, int>& values) {
    if (values.count(command.lhs) == 0) {
        cout << "Problem for comand " << command << "Missing left operand!" << endl;
        return 0;
    }
    if (values.count(command.rhs) == 0) {
        cout << "Problem for comand " << command << "Missing right operand!" << endl;
        return 0;
    }
    int lhs = values.find(command.lhs)->second;
    int rhs = values.find(command.rhs)->second;
    if (command.operation == "OR") {
        return lhs | rhs;
    } else if (command.operation == "XOR") {
        return lhs ^ rhs;
    } else if (command.operation == "AND") {
        return lhs & rhs;
    } else {
        cout << "Invalid operation! " << command.operation << endl;
        exit(1);
    }
}
ll solve_part1(const map<string, int>& values_input, const vector<Command>& commands) {
    int n = (int)commands.size();
    map<string, int> values = values_input;
    vector<vector<int> > ne(n);
    map<string, int> name_to_command;
    for (int i = 0; i < (int)commands.size(); ++i) {
        name_to_command[commands[i].result] = i;
    }
    auto add_edges_for_operand = [&](const string& operand, int idx) {
        if (values.count(operand)) {
            return;
        }
        ne[name_to_command[operand]].push_back(idx);
    };
    for (int i = 0; i < n; ++i) {
        add_edges_for_operand(commands[i].lhs, i);
        add_edges_for_operand(commands[i].rhs, i);
    }
    vector<int> sorted = topo_sort(ne);
    for (auto idx : sorted) {
        int val = compute_value(commands[idx], values);
        values[commands[idx].result] = val;
    }
    auto it = values.lower_bound("z00");
    vector<int> digits;
    while (it != values.end()) {
        digits.push_back(it->second);
        it++;
    }
    reverse(all(digits));
    ll result = 0;
    for (int digit : digits) {
        result = result * 2LL + (ll)digit;
    }
    return result;
}

string get_name(char c, int index) {
    char buf[20];
    sprintf(buf, "%c%02d", c, index);
    return buf;
}


bool check_x_and_y(ll x, ll y, int digits, const vector<Command>& commands) {
    map<string, int> values;
    for (int i = digits; i < 45; ++i) {
        values[get_name('x', i)] = 0;
        values[get_name('y', i)] = 0;
    }
    for (int xdi = 0; xdi < digits; ++xdi) {
        values[get_name('x', xdi)] = int((x & (1LL << xdi)) != 0);
    }
    for (int ydi = 0; ydi < digits; ++ydi) {
        values[get_name('y', ydi)] = int((y & (1LL << ydi)) != 0);
    }

    ll result = solve_part1(values, commands);
    ll mask = (1LL << (digits)) - 1LL;
    if ((result & mask) != ((x + y) & mask)) {
        return false;
    }
    for (int i = digits; i < 45; ++i) {
        values[get_name('x', i)] = 1;
    }
    if ((result & mask) != ((x + y) & mask)) {
        return false;
    }
    return true;
}
ll rand_ll(int digits) {
    ll result = 0;
    for (int i = 0; i < digits; ++i) {
        if (rand() % 2) {
            result |= (1LL << i);
        }
    }
    return result;
}
bool check_digits(int digits, const vector<Command>& commands) {
    ll all_ones = (1LL << digits) - 1LL;
    if (!check_x_and_y(all_ones, all_ones, digits, commands)) {
        return false;
    }
    if (!check_x_and_y(all_ones, 0, digits, commands)) {
        return false;
    }

    for (int i = 0; i < 5; ++i) {
        if (!check_x_and_y(rand_ll(digits), rand_ll(digits), digits, commands)) {
            return false;
        }
    }
    return true;
}
string solve_part2(const map<string, int>& values_input, const vector<Command>& input_commands) {
    auto commands = input_commands;
    int n = (int)commands.size();
    map<string, int> values = values_input;

    vector<string> swapped;
    for (int di = 1; di < 45; ++di) {
        cout << "Checking ... " << di << endl;
        if (!check_digits(di, commands)) {
            auto temp = commands;
            bool found = false;
            vector<pair<int, int> > possibles;
            for (int i = 0; i < (int)temp.size() && !found; ++i) {
                for (int j = i + 1; j < (int)temp.size() && !found; ++j) {
                    temp = commands;
                    swap(temp[i].result, temp[j].result);
                    if (check_digits(di, temp)) {
                        string a = temp[j].result, b = temp[i].result;
                        printf("Possible %s and %s\n", a.c_str(), b.c_str());
                        possibles.push_back({i, j});
                    } 
                }
            }
            auto to_swap = possibles.back();
            printf("Swapping %s and %s\n", commands[to_swap.first].result.c_str(), commands[to_swap.second].result.c_str());
            swap(commands[to_swap.first].result, commands[to_swap.second].result);
            swapped.push_back(commands[to_swap.first].result);
            swapped.push_back(commands[to_swap.second].result);
        }
    }
    sort(all(swapped));
    ostringstream os;
    for (int i = 0; i < (int)swapped.size(); ++i) {
        os << swapped[i];
        if (i + 1 != swapped.size()) {
            os << ",";
        }
    }
    
    return os.str();
}
pair<string, int> parse_value(const string& s) {
    auto temp = split(s, ':');
    int value;
    sscanf(temp[1].c_str(), "%d", &value);
    return {temp[0], value};
}
Command parse_command(const string& s) {
    auto temp = split(s, ' ');
    return Command{min(temp[0], temp[2]), max(temp[0], temp[2]), temp[1], temp[4]};
}
int main() {
    freopen("advent-of-code-2024/advent.in", "r", stdin);
    string s;
    map<string, int> values;

    while (getline(cin, s)) {
        if (s.size() == 0) {
            break;
        }
        values.insert(parse_value(s));
    }

    vector<Command> commands;
    while (getline(cin, s)) {
        commands.push_back(parse_command(s));
    }
    cout << "Part 1: " << solve_part1(values, commands) << endl;
    cout << "Part 2: " << solve_part2(values, commands) << endl;
    return 0;
}


/*
Part 2: Checking ... 1
Checking ... 2
Checking ... 3
Checking ... 4
Checking ... 5
Checking ... 6
Checking ... 7
Possible fkp and z06Swapping z06 and fkpChecking ... 8
Checking ... 9
Checking ... 10
Checking ... 11
Checking ... 12
Possible ngr and z11Swapping z11 and ngrChecking ... 13
Checking ... 14
Checking ... 15
Checking ... 16
Checking ... 17
Checking ... 18
Checking ... 19
Checking ... 20
Checking ... 21
Checking ... 22
Checking ... 23
Checking ... 24
Checking ... 25
Checking ... 26
Checking ... 27
Checking ... 28
Checking ... 29
Checking ... 30
Checking ... 31
Checking ... 32
Possible z31 and mfmSwapping mfm and z31Checking ... 33
Checking ... 34
Checking ... 35
Checking ... 36
Checking ... 37
Checking ... 38
Checking ... 39
Possible krj and bptSwapping bpt and krjChecking ... 40
Checking ... 41
Checking ... 42
Checking ... 43
Checking ... 44
fkp <-> z06
z06 <-> fkp
ngr <-> z11
z11 <-> ngr
z31 <-> mfm
mfm <-> z31
krj <-> bpt
bpt <-> krj


*/