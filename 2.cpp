#include <iostream> 
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#define all(v) v.begin(),v.end()
using namespace std;

vector<int> parse_line(string line) {
    istringstream in(line);
    vector<int> result;
    int temp;
    while (in >> temp) {
        result.push_back(temp);
    }
    return result;
}

bool is_good(const vector<int>& a) {
    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i] <= a[i - 1]) {
            return false;
        }
        int t = abs(a[i] - a[i - 1]);
        if (t < 1 || t > 3) {
            return false;
        }
    }
    return true;
}

bool safe_part1(const vector<int>& a) {
    vector<int> v = a;
    if (is_good(v)) {
        return true;
    } else {
        reverse(all(v));
        if (is_good(v)) {
            return true;
        }
    }
    return false;
}

bool safe_part2(const vector<int>& a) {
    if (safe_part1(a)) {
        return true;
    }
    for (int i = 0 ;i < (int)a.size(); ++i) {
        vector<int> v = a;
        v.erase(v.begin() + i);
        if (safe_part1(v)) {
            return true;
        }
    }

    return false;
}

int main() {
    string line;
    vector<vector<int> > a;
    while (getline(cin, line)) {
        a.push_back(parse_line(line));
    }
    int answer = 0;
    for (vector<int>& v : a) {
        answer += safe_part1(v);
    }
    cout << "Part 1 " << answer << endl;
    answer = 0;
    for (vector<int>& v : a) {
        answer += safe_part2(v);
    }
    cout << "Part 2 " << answer << endl;
    return 0;
}