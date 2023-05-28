#include <bits/stdc++.h>

#define sz(x) (int)(x).size()
#define rep(i, n) for (int i = 0; i < n; i++)
#define unq(x) { sort(x.begin(), x.end()); x.resize(unique (x.begin(), x.end()) - x.begin()); }

using namespace std;

const int N = 100 + 5;
const int INF = 1e9;

mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

char a[N][N];
int n, k;

void solve() {
    n = 100;
    k = 100;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (rnd() % 6 > 0) {
                a[i][j] = '.';
            } else {
                a[i][j] = '*';
            }
        }
    }
    cout << n << ' ' << k << '\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << a[i][j];
        }
        cout << '\n';
    }
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
//    freopen ("snowmen.in", "r", stdin);
    freopen ("output.txt", "w", stdout);
    int qq = 1;
//    cin >> qq;
    rep (i, qq) {
        solve();
    }
}