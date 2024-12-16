#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>

using namespace std;
typedef long long ll;

int moves[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
struct position {
    pair<int, int> p;
    int dir;
};

const ll factor = 100000LL;

ll encode_position(const position& pos) {
    return (ll)pos.p.first * factor * factor + (ll)pos.p.second * factor + pos.dir;
}

position decode_position(ll code) {
    position result;
    result.dir = code % factor;
    code /= factor;
    result.p.second =code % factor;
    code /= factor;
    result.p.first = code;
    return result;
}

unordered_map<ll, ll> run_dijkstra(const vector<string>& a, const pair<int, int>& start, const pair<int, int>& end, int start_dir) {
int n = (int)a.size();
    int m = (int)a[0].size();
    priority_queue<pair<ll, ll> > pq;
    unordered_map<ll, ll> dist;
    position cur({start, start_dir});
    pq.push({0, encode_position(cur)});

    while (!pq.empty()) {
        ll cur_code = pq.top().second;
        cur = decode_position(cur_code);
        ll d = -pq.top().first;
        pq.pop();
        ll kod = encode_position(cur);
        if (dist.count(kod)) {
            continue;
        } 
        dist[kod] = d;

        for (int l = 0; l < 4; ++l) {
            pair<int, int> nxt(cur.p.first + moves[l][0], cur.p.second + moves[l][1]);

            if (nxt.first < 0 || nxt.first >= n || nxt.second < 0 || nxt.second >= m) {
                continue;
            }
            if (a[nxt.first][nxt.second] != '.') {
                continue;
            }
            position nxt_p{nxt, l};
            ll nxt_kod = encode_position(nxt_p);
            ll edge = 1;
            if (l != cur.dir) {
                edge += 1000;
                if (l % 2 == cur.dir % 2) {
                    edge += 1000;
                }
            }

            auto it = dist.find(nxt_kod);
            if (it == dist.end() || d + edge < it->second) {
                pq.push({-(d + edge), encode_position(nxt_p)});
            }
        }
    }
    return dist;
}
ll solve_part1(const vector<string>& a, const pair<int, int>& start, const pair<int, int>& end) {
    auto dist = run_dijkstra(a, start, end, 1);
    ll answer = 100000000000000000LL;
    for (auto it : dist) {
        position cur = decode_position(it.first);
        if (cur.p == end) {
            answer = min(answer, it.second);
        }
    }
    return answer; 
}

vector<vector<vector<ll> > > to_field_distances(const vector<string>& a, const unordered_map<ll, ll>& dist) {
    int n = (int)a.size();
    int m = (int)a[0].size(); 
    vector<vector<vector<ll> > > result(4, vector<vector<ll> >(n, vector<ll>(m, -1)));

    for (auto it : dist) {
        position cur = decode_position(it.first);
        if (result[cur.dir][cur.p.first][cur.p.second] == -1 || result[cur.dir][cur.p.first][cur.p.second] > it.second) {
            result[cur.dir][cur.p.first][cur.p.second] = it.second;
        }
    }
    return result;
}
ll solve_part2(const vector<string>& a, const pair<int, int>& start, const pair<int, int>& end) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    auto dist = run_dijkstra(a, start, end, 1);
    auto straight = to_field_distances(a, dist);
    dist = run_dijkstra(a, end, start, 0);
    auto reverse = to_field_distances(a, dist);

    for (int sd = 1; sd < 4; ++sd) {
        dist = run_dijkstra(a, end, start, sd);
        auto temp = to_field_distances(a, dist);
        for (int d = 0; d < 4; ++d) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    reverse[d][i][j] = min(reverse[d][i][j], temp[d][i][j]);
                }
            }
        }
    }
    ll best = solve_part1(a, start, end);
    ll answer = 0;

    set<pair<int, int> > good;
    good.insert(start);
    good.insert(end);
    for (int d = 0; d < 4; ++d) {
        bool found = false;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (straight[d][i][j] == -1) {
                    continue;
                }

                for (int other_d = 0; other_d < 4; ++other_d) {
                    ll penalty;
                    if (other_d == (d + 2) % 4) {
                        penalty = 0;
                    } else if (other_d == d) {
                        penalty = 2000;
                    } else {
                        penalty = 1000;
                    }
                    if (straight[d][i][j] + reverse[other_d][i][j] + penalty == best) {
                       good.insert({i, j});
                    }
                }
            }
        }
    }

    return (int)good.size();
}

int main() {
    string s;
    vector<string> a;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    pair<int, int> start;
    pair<int, int> end;
    for (int i = 0; i < (int)a.size(); ++i) {
        for (int j = 0; j < (int)a[0].size(); ++j) {
            if (a[i][j] == 'S') {
                start = {i, j};
                a[i][j] = '.';
            } 
            if (a[i][j] == 'E') {
                end = {i, j};
                a[i][j] = '.';
            }
        }
    }
    cout << "Part 1: " << solve_part1(a, start, end) << endl;
    cout << "Part 2: " << solve_part2(a, start, end) << endl;
    return 0;
}