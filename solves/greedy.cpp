#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>

#define sz(x) (int)(x).size()
#define rep(i, n) for (int i = 0; i < n; i++)
#define unq(x) { sort(x.begin(), x.end()); x.resize(unique (x.begin(), x.end()) - x.begin()); }

using namespace std;

const int N = 100 + 5;
const int INF = 1e9;

int a[N][N];
int n, k, group[N][N], team[N][N], timer = 0, groups_count = 0, teams_count = 0;
int dx1[4] = {1, -1, 0, 0};
int dy1[4] = {0, 0, 1, -1};
int dx2[8] = {0, 1, 2, 1, 0, -1, -2, -1};
int dy2[8] = {2, 1, 0, -1, -2, -1, 0, 1};
vector<int> team_list[N * N], team_alive_list[N * N];
vector<pair<int, int>> group_pts[N * N];
set<pair<pair<int, int>, int>> team_options_set[N * N];
vector<pair<pair<int, int>, int>> team_options[N * N];

bool Valid(int i, int j) {
    return (i >= 0 && i < n && j >= 0 && j < n);
}

bool IsDead(int i, int j) {
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (Valid(ni, nj) && !a[ni][nj]) {
            return false;
        }
    }
    return true;
}

void GroupDfs(int i, int j) {
    group[i][j] = timer;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (Valid(ni, nj) && a[ni][nj] && group[ni][nj] == -1) {
            GroupDfs(ni, nj);
        }
    }
}

void TeamDfs(int i, int j) {
    team[i][j] = timer;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (Valid(ni, nj) && a[ni][nj] && team[ni][nj] == -1) {
            TeamDfs(ni, nj);
        }
    }
    for (int d = 0; d < 8; ++d) {
        int ni = i + dx2[d], nj = j + dy2[d];
        if (Valid(ni, nj) && a[ni][nj] && team[ni][nj] == -1) {
            TeamDfs(ni, nj);
        }
    }
}

int was[N][N];

bool DfsIsDead(int i, int j, int c) {
    was[i][j] = timer;
    bool dead = true;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (Valid(ni, nj) && a[ni][nj] == c && was[ni][nj] != timer) {
            dead &= DfsIsDead(ni, nj, c);
        }
        if (Valid(ni, nj) && !a[ni][nj]) {
            dead = false;
        }
    }
    return dead;
}

void DfsDeleteGroup(int i, int j, int c) {
    was[i][j] = timer;
    a[i][j] = 0;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (Valid(ni, nj) && a[ni][nj] == c && was[ni][nj] != timer) {
            DfsDeleteGroup(ni, nj, c);
        }
    }
}

int F(int cost, int value) {
    return 4 * value - cost;
}

void Clear() {
    timer = 0;
    groups_count = 0;
    teams_count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            was[i][j] = 0;
        }
    }
    for (int i = 0; i < n * n; ++i) {
        team_list[i].clear();
        team_alive_list[i].clear();
        group_pts[i].clear();
        team_options_set[i].clear();
        team_options[i].clear();
    }
}

void Solve() {
    cin >> n >> k;
    Clear();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char c;
            cin >> c;
            a[i][j] = (c == '*');
            group[i][j] = team[i][j] = -1;
        }
    }

    timer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (a[i][j] && group[i][j] == -1) {
                GroupDfs(i, j);
                timer++;
            }
        }
    }

    groups_count = timer;
    timer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (a[i][j] && team[i][j] == -1) {
                TeamDfs(i, j);
                timer++;
            }
        }
    }

    teams_count = timer;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (a[i][j]) {
                group_pts[group[i][j]].push_back({i, j});
            }
        }
    }

    unordered_set<int> free;
    for (int i = 0; i < groups_count; ++i) {
        free.insert(i);
    }
    int an = 0;
    vector<pair<int, int>> ans_pts;

    while (!free.empty()) {
        int ma = -1, g = 0, cost = 0, value = 0;
        for (auto cur_g : free) {
            vector<pair<int, int>> pts;
            for (auto [x, y] : group_pts[cur_g]) {
                for (int d = 0; d < 4; ++d) {
                    int nx = x + dx1[d], ny = y + dy1[d];
                    if (Valid(nx, ny) && a[nx][ny] == 0) {
                        a[nx][ny] = 2;
                        pts.push_back({nx, ny});
                    }
                }
            }
            if (F(sz(pts), sz(group_pts[cur_g])) > ma) {
                ma = F(sz(pts), sz(group_pts[cur_g]));
                g = cur_g;
                cost = sz(pts);
                value = sz(group_pts[cur_g]);
            }
            for (auto [nx, ny] : pts) {
                a[nx][ny] = 0;
            }
        }

        free.erase(g);
        if (cost > k) {
            continue;
        }
        an += value;
        k -= cost;
        for (auto [x, y] : group_pts[g]) {
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx1[d], ny = y + dy1[d];
                if (Valid(nx, ny) && a[nx][ny] == 0) {
                    a[nx][ny] = 2;
                    ans_pts.push_back({nx, ny});
                }
            }
        }
    }
    cout << an << ' ' << sz(ans_pts) << '\n';
    for (auto [x, y] : ans_pts) {
        cout << x + 1 << ' ' << y + 1 << '\n';
    }

}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int qq = 1;
    freopen("/home/makson95/go/go-small/small", "r", stdin);
    freopen ("output.txt", "w", stdout);
    cin >> qq;
    rep (i, qq) {
        Solve();
//        stress();
    }
}