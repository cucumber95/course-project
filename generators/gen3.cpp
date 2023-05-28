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
int n, k, r;

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

bool Valid(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

int timer = 0;
int group[N][N];

void GroupDfs(int i, int j) {
    group[i][j] = timer;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx[d], nj = j + dy[d];
        if (Valid(ni, nj) && a[ni][nj] == '*' && group[ni][nj] == -1) {
            GroupDfs(ni, nj);
        }
    }
}

int GroupsCount() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            group[i][j] = -1;
        }
    }
    timer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (group[i][j] == -1 && a[i][j] == '*') {
                GroupDfs(i, j);
                timer++;
            }
        }
    }
    return timer;
}

void solve() {
    n = 50;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (rnd() % 3 == 0) {
                a[i][j] = '*';
            } else {
                a[i][j] = '.';
            }
        }
    }

    if (GroupsCount() <= 24) {
        while (GroupsCount() > 20) {
            int x, y;
            while (1) {
                x = rnd() % n;
                y = rnd() % n;
                if (a[x][y] == '.') {
                    bool fl = 0;
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (Valid(nx, ny) && a[nx][ny] == '*') {
                            fl = 1;
                        }
                    }
                    if (fl) {
                        break;
                    }
                }
            }
            a[x][y] = '*';
        }
    }

    if (GroupsCount() <= 20) {
        cout << n << ' ' << GroupsCount() << '\n';
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << a[i][j];
            }
            cout << '\n';
        }
    }
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
//    freopen ("snowmen.in", "r", stdin);
    freopen ("output.txt", "w", stdout);
    int qq = 1;
    cin >> qq;
    rep (i, qq) {
        solve();
    }
}
