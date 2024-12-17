#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>


using namespace std;
typedef long long ll;

struct Program {
    vector<ll> registers;

    int op;
    vector<ll> instructions;
    vector<ll> outputs;

    bool do_operation() {
        if (op >= (int)instructions.size() || op < 0) {
            return false;
        }
        switch (instructions[op]) {
            
            case 0: do_adv();op += 2; break;
            case 1: do_bxl();op += 2; break;
            case 2: do_bst();op += 2; break;
            case 3: do_jnz(); break;
            case 4: do_bxc();op += 2; break;
            case 6: do_bdv();op += 2; break;
            case 7: do_cdv();op += 2; break;
            case 5: do_out();op += 2; break;
        }
        return true;
    }
    void run_to_halt() {
        while (do_operation());
    
    }
    ll literal_operand() {
        return instructions[op + 1];
    }

    ll combo_operand() {
        int value = instructions[op + 1];
        if (value <= 3) {
            return value;
        }
        if (value <= 6) {
            return registers[value - 4];
        }
        cout << "This should not happen!" << endl;
        exit(1);
    }

    ll do_division() {
        ll numerator = registers[0];
        ll den_power = combo_operand();
        ll den = 1;
        for (ll i = 0; i < den_power && den <= numerator; ++i) {
            den *= 2;
        }
        return registers[0] / den;
    }
    void do_adv() {
        registers[0] = do_division();
    }
    void do_bxl() {
        registers[1] = registers[1] ^ literal_operand();
    }
    void do_bst() {
        registers[1] = combo_operand() % 8;
    }
    void do_jnz() {
        if (registers[0] == 0) {
            op += 2;
            return;
        } else {
            op = literal_operand();
        }
    }
    void do_bxc() {
        registers[1] = registers[1] ^ registers[2];
    }
    void do_out() {
        ll value = combo_operand();
        outputs.push_back(value % 8);
    }
    void do_bdv() {
        registers[1] = do_division();
    }
    void do_cdv() {
        registers[2] = do_division();
    }
};

string solve_part1(const Program& program) {
    Program p = program;
    p.run_to_halt();
    ostringstream result;
    
    for (int i = 0; i < (int)p.outputs.size(); ++i) {
        result << p.outputs[i];
        if (i + 1 != (int)p.outputs.size()) {
            result << ",";
        }
        
    }
    return result.str();
}

set<ll> solve_upto(const Program& program, int limit) {
    set<ll> result;
    set<ll> prevs;
    if (limit == 1) {
        prevs = set<ll>{0};
    } else {
        prevs = solve_upto(program, limit - 1);
    }
    ll deg8 = 1LL << ((limit - 1) * 3);

    for (ll prev : prevs) {
        for (ll units = 0; units < 8; ++units) {
            ll temp = prev + units * deg8;
            ll b = temp / deg8;
            b = (b % 8) ^ 5;
            for (ll to_c = 0; to_c < 8; ++to_c) {
                ll rega = temp + to_c * (1 << b) * deg8;
                Program p = program;
                p.registers[0] = rega;
                p.run_to_halt();
                bool good = true;
                for (int i = 0; i < limit; ++i) {
                    if (p.outputs[i] != p.instructions[i]) {
                        good = false;
                    }
                }
                if (good) {
                    result.insert(rega);
                }
            }
        }
    }
    return result;
}
ll solve_part2(const Program& program) {
    auto result = solve_upto(program, program.instructions.size());
    return *result.begin();
}

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

Program readProgram() {
    Program result;
    string s;
    result.registers.resize(3);
    scanf("Register A: %lld", &result.registers[0]);
    getline(cin, s);
    scanf("Register B: %lld", &result.registers[1]);
    getline(cin, s);
    scanf("Register C: %lld", &result.registers[2]);

    result.op = 0;
    cin >> s >> s;
    vector<string> parts = split(s, ',');
    for (auto part : parts) {
        result.instructions.push_back(atoll(part.c_str()));
    }
    return result;
}

int main() {
    auto program = readProgram();
    cout << "Part 1: " << solve_part1(program) << endl;
    cout << "Part 2: " << solve_part2(program) << endl;
    return 0;
}