#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;
typedef long long ll;

map<char, vector<pair<int, int> > > get_per_frequency(const vector<string>&a) {
    map<char, vector<pair<int, int> > > result;
    int n = (int)a.size();
    int m = (int)a[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i][j] != '.') {
                result[a[i][j]].push_back({i, j});
            }
        }
    }
    return result;
}

vector<pair<int, int> > get_antinodes1(int n, int m, const vector<pair<int, int> >& nodes) {
    vector<pair<int, int> > result;
    for (int i = 0; i < (int)nodes.size(); ++i) {
        for (int j = i + 1; j < (int)nodes.size(); ++j) {
            int dx = nodes[i].first - nodes[j].first;
            int dy = nodes[i].second - nodes[j].second;

            int tx = nodes[i].first + dx;
            int ty = nodes[i].second + dy;
            if (tx >= 0 && tx < n && ty >= 0 && ty < m) {
                result.push_back({tx, ty});
            }
            tx = nodes[j].first - dx;
            ty = nodes[j].second - dy;
            if (tx >= 0 && tx < n && ty >= 0 && ty < m) {
                result.push_back({tx, ty});
            }
        }
    }
    return result;
}
long long gcd(long long a, long long b) {
    long long r;
    if (b > a)
        swap(b, a);
    if (a == 0) {
        if (b != 0)
            return b;
        else
            return 1;
    }
    if (b == 0)
        return a;
    while (a % b) {
        r = a % b;
        a = b;
        b = r;
    }
    return b;
}

vector<pair<int, int> > get_antinodes2(int n, int m, const vector<pair<int, int> >& nodes) {
    vector<pair<int, int> > result;
    for (int i = 0; i < (int)nodes.size(); ++i) {
        for (int j = i + 1; j < (int)nodes.size(); ++j) {
            int dx = nodes[i].first - nodes[j].first;
            int dy = nodes[i].second - nodes[j].second;
            int d = gcd(abs(dx), abs(dy));
            dx /= d;
            dy /= d;

            
            int tx = nodes[i].first;
            int ty = nodes[i].second;
            while (tx >= 0 && tx < n && ty >= 0 && ty < m) {
                tx -= dx;
                ty -= dy;
            }
            tx += dx;
            ty += dy;
            while (tx >= 0 && tx < n && ty >= 0 && ty < m) {
                result.push_back({tx, ty});
                tx += dx;
                ty += dy;
            }
        }
    }
    return result;
}

ll solve_part1(const vector<string>& a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    auto per_frequency = get_per_frequency(a);

    set<pair<int, int> > antinodes;
    for (auto freq : per_frequency) {
        auto temp = get_antinodes1(n, m, freq.second);
        for (auto position : temp) {
            antinodes.insert(position);
        }
    }
    return antinodes.size();
}
ll solve_part2(const vector<string>& a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    auto per_frequency = get_per_frequency(a);

    set<pair<int, int> > antinodes;
    for (auto freq : per_frequency) {
        auto temp = get_antinodes2(n, m, freq.second);
        for (auto position : temp) {
            antinodes.insert(position);
        }
    }
    return antinodes.size();

}
int main() {
    string s;
    vector<string> a;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    
    cout << "Part 1: " << solve_part1(a) << endl;
    cout << "Part 2: " << solve_part2(a) << endl;
    return 0;
}