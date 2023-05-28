#include <bits/stdc++.h>

#define sz(x) (int)(x).size()
#define rep(i, n) for (int i = 0; i < n; i++)
#define unq(x) { sort(x.begin(), x.end()); x.resize(unique (x.begin(), x.end()) - x.begin()); }

using namespace std;

const int N = 100 + 5;
const int INF = 1e9;

mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());

char a[N][N];
int b[N][N];
int n, cnt, r;

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

bool Valid(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

void Bfs1(const vector<pair<int, int>> &v) {
    queue<pair<int, int>> q;
    for (auto &[x, y] : v) {
        b[x][y] = 1;
        q.push({x, y});
    }
    while (!q.empty()) {
        pair<int, int> o = q.front();
        q.pop();
        if (b[o.first][o.second] == (r - 2) * 2 + 2) {
            continue;
        }
        for (int d = 0; d < 4; ++d) {
            int nx = o.first + dx[d], ny = o.second + dy[d];
            if (Valid(nx, ny) && !b[nx][ny]) {
                b[nx][ny] = b[o.first][o.second] + 1;
                q.push({nx, ny});
            }
        }
    }
}

void Bfs2(const vector<pair<int, int>> &v) {
    queue<pair<int, int>> q;
    for (auto &[x, y] : v) {
        b[x][y] = 1;
        q.push({x, y});
    }
    while (!q.empty()) {
        pair<int, int> o = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nx = o.first + dx[d], ny = o.second + dy[d];
            if (Valid(nx, ny) && !b[nx][ny]) {
                b[nx][ny] = min(r, b[o.first][o.second] + 1);
                q.push({nx, ny});
            }
        }
    }
}

void solve() {
    cnt = 15;
    n = 100;
    r = 11;
    vector<pair<int, int>> v;
    for (int i = 0; i < cnt; ++i) {
        int x, y;
        while (true) {
            x = rnd() % n;
            y = rnd() % n;
            if (!b[x][y]) {
                break;
            }
        }
        Bfs1({{x, y}});
        v.push_back({x, y});
    }

    memset(b, 0, sizeof(b));
    Bfs2(v);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (b[i][j] == r) {
                a[i][j] == '.';
            } else {
                if (rnd() % ((b[i][j] + 2) / 2) == 0) {
                    a[i][j] = '*';
                } else {
                    a[i][j] = '.';
                }
            }
        }
    }
    cout << n << '\n';
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