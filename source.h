#include <bits/stdc++.h>

using namespace std;

const int N = 1e4 + 10;
const int mul = 233, Mod = 998244353;

int query(int x, int y, int z);

int A_ans[N];

int guess(int n, int Taskid) {
    memset(A_ans, 0, sizeof A_ans);

    if (n < 5) return 0; // Should not happen based on constraints

    // Step 1: Find the first 5 elements
    vector<int> first_5 = {1, 2, 3, 4, 5};
    map<tuple<int, int, int>, int> q_res;
    vector<int> q_vals;

    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            for (int k = j + 1; k < 5; k++) {
                int res = query(first_5[i], first_5[j], first_5[k]);
                q_res[{first_5[i], first_5[j], first_5[k]}] = res;
                q_vals.push_back(res);
            }
        }
    }

    sort(q_vals.begin(), q_vals.end());
    int S_min = q_vals[0];
    int S_max = q_vals.back();

    map<int, int> counts;
    for (int v : q_vals) counts[v]++;
    int S_freq = -1;
    for (auto const& [val, count] : counts) {
        if (count >= 3) {
            S_freq = val;
            break;
        }
    }

    int v3 = (S_min + S_max - S_freq) / 2;
    int v1 = S_min - v3;
    int v5 = S_max - v3;

    vector<int> rem = q_vals;
    rem.erase(find(rem.begin(), rem.end(), S_min));
    rem.erase(find(rem.begin(), rem.end(), S_max));
    for (int i = 0; i < 3; i++) {
        rem.erase(find(rem.begin(), rem.end(), S_freq));
    }

    int v4 = rem[0] - v1;
    int v2 = rem.back() - v5;

    vector<int> vals = {v1, v2, v3, v4, v5};
    sort(vals.begin(), vals.end());

    vector<int> best_p;
    vector<int> p = vals;
    do {
        bool ok = true;
        for (int i = 0; i < 5; i++) {
            for (int j = i + 1; j < 5; j++) {
                for (int k = j + 1; k < 5; k++) {
                    int mx = max({p[i], p[j], p[k]});
                    int mn = min({p[i], p[j], p[k]});
                    if (mx + mn != q_res[{first_5[i], first_5[j], first_5[k]}]) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) break;
            }
            if (!ok) break;
        }
        if (ok) {
            best_p = p;
            break;
        }
    } while (next_permutation(p.begin(), p.end()));

    for (int i = 0; i < 5; i++) {
        A_ans[first_5[i]] = best_p[i];
    }

    // Step 2: Maintain sorted known elements
    vector<pair<int, int>> known;
    for (int i = 0; i < 5; i++) {
        known.push_back({best_p[i], first_5[i]});
    }
    sort(known.begin(), known.end());

    mt19937 rng(1919810);

    for (int x = 6; x <= n; x++) {
        int m = known.size();
        int i = rng() % (m - 1); // Pick a random gap

        int a_idx = known[i].second;
        int b_idx = known[i+1].second;
        int a_val = known[i].first;
        int b_val = known[i+1].first;

        int S = query(a_idx, b_idx, x);
        int Ax = -1;

        if (S < a_val + b_val) {
            Ax = S - b_val;
        } else if (S > a_val + b_val) {
            Ax = S - a_val;
        } else {
            if (i >= 1) {
                int c_idx = known[0].second;
                int d_idx = known[1].second;
                int c_val = known[0].first;
                int S_prime = query(c_idx, d_idx, x);
                Ax = S_prime - c_val;
            } else {
                int c_idx = known[m-2].second;
                int d_idx = known[m-1].second;
                int d_val = known[m-1].first;
                int S_prime = query(c_idx, d_idx, x);
                Ax = S_prime - d_val;
            }
        }

        A_ans[x] = Ax;
        known.push_back({Ax, x});
        // Keep known sorted
        int j = known.size() - 1;
        while (j > 0 && known[j].first < known[j-1].first) {
            swap(known[j], known[j-1]);
            j--;
        }
    }

    int ret = 0;
    for (int i = n; i >= 1; i--) {
        ret = 1ll * (ret + A_ans[i]) * mul % Mod;
    }
    return ret;
}
