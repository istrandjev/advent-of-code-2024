#include <iostream> 
#include <string>
#include <regex>


#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

ll mul(const smatch& m) {
    return atoi(m[1].str().c_str()) * atoi(m[2].str().c_str());
}
ll solve(string s, const string& regex_str) {
    regex e(regex_str);
    ll res = 0;
    bool enabled = true;
    for (smatch m; regex_search(s, m, e); s = m.suffix().str()) {
        if (m[0] == "do()") {
            enabled = true;
        } else if (m[0] == "don't()") {
            enabled = false;
        } else {
            if (enabled) {
                res += mul(m);
            }
        }
    }

    return res;
}

int main() {
    string input;
    string s;
    while (getline(cin, input)) {
        s += input + "\n";
    }
    cout << "Part 1 " << solve(s, "mul\\((\\d{1,3}),(\\d{1,3})\\)") << endl;
    cout << "Part 2 " << solve(s, "mul\\((\\d{1,3}),(\\d{1,3})\\)|do\\(\\)|don't\\(\\)") << endl;
}