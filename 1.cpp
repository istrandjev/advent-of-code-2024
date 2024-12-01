#include <iostream> 
#include <vector>
#include <algorithm>
#include <map>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;

int main() {
    vector<ll> a, b;
    int x, y;
    while (cin >> x >> y) {
        a.push_back(x);
        b.push_back(y);
    }
    sort(all(a));
    sort(all(b));
    int total_distance = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        total_distance += abs(a[i] - b[i]);
    }
    cout << total_distance << endl;

    map<ll, int> counts;
    for (auto v: b) {
        counts[v]++;
    }
    ll res_part_two = 0;
    for (auto v : a) {
        res_part_two += counts[v] * v;
    }
    cout << res_part_two << endl;
    return 0;
}