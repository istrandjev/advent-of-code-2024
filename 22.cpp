#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

ll next_pseudorandom(ll secret) {
    ll temp = secret << 6;
    secret = secret ^ temp;
    secret &= ((1LL << 24) - 1);
    secret = secret ^ (secret >> 5);
    secret &= ((1LL << 24) - 1);
    secret = (secret << 11) ^ secret;
    secret &= ((1LL << 24) - 1);
    return secret;
}

ll get_secret_after(ll secret, int steps) {
    for (int i = 0; i < steps; ++i) {
        secret = next_pseudorandom(secret);
    }
    return secret;
}

ll solve_part1(const vector<ll>& secrets) {
    ll result = 0;
    for (auto s : secrets) {
        result += get_secret_after(s, 2000);
    }
    return result;
}

typedef vector<int> helper_type;

ll get_for_sequence(int sequence_code, const vector<helper_type>& helper) {
    ll answer = 0;
    for (int i = 0; i < (int)helper.size(); ++i) {
        if (helper[i][sequence_code] != -1) {
            answer += helper[i][sequence_code];
        }
    }
    return answer;
}
helper_type get_prices(const vector<int>& changes, const vector<int>& prices) {
    int code = 0;
    for (int i = 0; i < 3; ++i) {
        code = code * 21 + changes[i] + 10;
    }
    helper_type result;
    result.resize(21 * 21 * 21 * 21, -1);
    for (int i = 3; i < (int)changes.size(); ++i) {
        code = code % (21 * 21 * 21);
        code = code * 21 + changes[i] + 10;
        if (result[code] == -1) {
            result[code] = prices[i];
        }
    }
    return result;
}
ll solve_part2(const vector<ll>& secrets) {
    vector<vector<int> > changes(secrets.size());
    vector<vector<int> > prices(secrets.size());
    for (int si = 0; si < (int)secrets.size(); ++si) {
        ll secret = secrets[si];
        int current = secret % 10;
        for (int i = 0; i < 2000; ++i) {
            secret = next_pseudorandom(secret);
            int digit = secret % 10;
            changes[si].push_back(digit - current);
            prices[si].push_back(digit);
            current = digit;
        }
    }
    vector<helper_type> helper(secrets.size());
    for (int si = 0; si < (int)secrets.size(); ++si) {
        helper[si] = get_prices(changes[si], prices[si]);
    }
    ll answer = 0;
    for (int sequence_code = 0; sequence_code < 21 * 21 * 21 * 21; ++sequence_code) {
        answer = max(answer, get_for_sequence(sequence_code, helper));
    }
    return answer;
}

int main() {
    vector<ll> secrets;
    ll temp;
    while (cin >> temp) {
        secrets.push_back(temp);
    }
    cout << "Part 1: " << solve_part1(secrets) << endl;
    cout << "Part 2: " << solve_part2(secrets) << endl;
    return 0;
}
