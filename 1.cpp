#define _CRT_SECURE_NO_DEPRECATE
#include <iostream> 
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <math.h>
#include <stack>
#include <deque>
#include <numeric>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <bitset>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <regex>

#define all(v) v.begin(),v.end()
using namespace std;
typedef long long ll;
typedef long double ld;

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