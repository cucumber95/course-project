#include <iostream>
#include <vector>
#include <set>

#define sz(x) (int)(x).size()
#define unq(x) { sort(x.begin(), x.end()); x.resize(unique (x.begin(), x.end()) - x.begin()); }

using namespace std;

const int N = 100 + 5;
const int INF = 1e9;

int a[N][N];
int n, k, group[N][N], team[N][N], was[N][N], timer = 0, groups_count = 0, teams_count = 0;
int dx1[4] = {1, -1, 0, 0};
int dy1[4] = {0, 0, 1, -1};
int dx2[8] = {0, 1, 2, 1, 0, -1, -2, -1};
int dy2[8] = {2, 1, 0, -1, -2, -1, 0, 1};
vector<int> team_list[N * N], team_alive_list[N * N];
vector<pair<int, int>> group_pts[N * N];
set<pair<pair<int, int>, int>> team_options_set[N * N];
vector<pair<pair<int, int>, int>> team_options[N * N];

bool IsValid(int i, int j) {
    return (i >= 0 && i < n && j >= 0 && j < n);
}

void GroupDfs(int i, int j) {
    group[i][j] = timer;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (IsValid(ni, nj) && a[ni][nj] && group[ni][nj] == -1) {
            GroupDfs(ni, nj);
        }
    }
}

void TeamDfs(int i, int j) {
    team[i][j] = timer;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (IsValid(ni, nj) && a[ni][nj] && team[ni][nj] == -1) {
            TeamDfs(ni, nj);
        }
    }
    for (int d = 0; d < 8; ++d) {
        int ni = i + dx2[d], nj = j + dy2[d];
        if (IsValid(ni, nj) && a[ni][nj] && team[ni][nj] == -1) {
            TeamDfs(ni, nj);
        }
    }
}

bool DfsIsDead(int i, int j, int c) {
    was[i][j] = timer;
    bool dead = true;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx1[d], nj = j + dy1[d];
        if (IsValid(ni, nj) && a[ni][nj] == c && was[ni][nj] != timer) {
            dead &= DfsIsDead(ni, nj, c);
        }
        if (IsValid(ni, nj) && !a[ni][nj]) {
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
        if (IsValid(ni, nj) && a[ni][nj] == c && was[ni][nj] != timer) {
            DfsDeleteGroup(ni, nj, c);
        }
    }
}

void SolveMask() {
    for (int i = 0; i < teams_count; ++i) {
        for (int mask = 0; mask < (1 << sz(team_alive_list[i])); ++mask) {
            vector<pair<int, int>> pts;
            int cost = 0;
            for (int j = 0; j < sz(team_alive_list[i]); ++j) {
                if (mask & (1 << j)) {
                    for (auto [x, y]: group_pts[team_alive_list[i][j]]) {
                        for (int d = 0; d < 4; ++d) {
                            int nx = x + dx1[d], ny = y + dy1[d];
                            if (IsValid(nx, ny) && !a[nx][ny]) {
                                a[nx][ny] = 2;
                                cost++;
                                pts.emplace_back(nx, ny);
                            }
                        }
                    }
                }
            }

            timer++;
            for (auto [x, y]: pts) {
                if (was[x][y] != timer) {
                    bool fl = DfsIsDead(x, y, 2);
                    if (fl) {
                        timer++;
                        DfsDeleteGroup(x, y, 2);
                    }
                }
            }

            ti  mer++;
            int value = 0;
            for (int j = 0; j < sz(team_alive_list[i]); ++j) {
                if (mask & (1 << j)) {
                    value += sz(group_pts[team_alive_list[i][j]]) * DfsIsDead(group_pts[team_alive_list[i][j]][0].first,
                                                                              group_pts[team_alive_list[i][j]][0].second,
                                                                              1);
                }
            }

            for (auto [x, y]: pts) {
                a[x][y] = 0;
            }

            team_options_set[i].insert({{value, cost}, mask});
        }
    }

    for (int i = 0; i < teams_count; ++i) {
        int last_value = -1;
        for (auto [x, mask]: team_options_set[i]) {
            if (last_value != x.first) {
                while (!team_options[i].empty() && team_options[i].back().first.second > x.second) {
                    team_options[i].pop_back();
                }
                team_options[i].emplace_back(x, mask);
                last_value = x.first;
            }
        }
    }

    int dp[teams_count][k + 1], p[teams_count][k + 1];
    for (int i = 0; i < teams_count; ++i) {
        for (int j = 0; j <= k; j++) {
            dp[i][j] = -1;
        }
    }

    for (int i = 0; i < teams_count; ++i) {
        if (!i) {
            for (int j = 0; j < sz(team_options[i]); ++j) {
                if (team_options[i][j].first.second > k) {
                    break;
                }
                dp[i][team_options[i][j].first.second] = team_options[i][j].first.first;
                p[i][team_options[i][j].first.second] = j;
            }
            continue;
        }
        for (int last_cost = 0; last_cost <= k; ++last_cost) {
            if (dp[i - 1][last_cost] == -1) {
                continue;
            }
            for (int j = 0; j < sz(team_options[i]); ++j) {
                int value = team_options[i][j].first.first, cost = team_options[i][j].first.second;
                if (last_cost + cost > k) {
                    break;
                }
                if (dp[i][last_cost + cost] < dp[i - 1][last_cost] + value) {
                    dp[i][last_cost + cost] = dp[i - 1][last_cost] + value;
                    p[i][last_cost + cost] = j;
                }
            }
        }
    }

    int max_value = -1, cur_i = teams_count - 1, cur_k = 0;
    for (int i = 0; i <= k; ++i) {
        if (max_value < dp[teams_count - 1][i]) {
            max_value = dp[teams_count - 1][i];
            cur_k = i;
        }
    }

    cout << cur_k << '\n';
    vector<int> ans_groups;
    while (cur_i >= 0) {
        int j = p[cur_i][cur_k];
        int mask = team_options[cur_i][j].second;
        for (int i = 0; i < sz(team_alive_list[cur_i]); ++i) {
            if (mask & (1 << i)) {
                ans_groups.push_back(team_alive_list[cur_i][i]);
            }
        }
        cur_k -= team_options[cur_i][j].first.second;
        cur_i--;
    }

    for (int g : ans_groups) {
        for (auto [x, y] : group_pts[g]) {
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx1[d], ny = y + dy1[d];
                if (IsValid(nx, ny) && !a[nx][ny]) {
                    a[nx][ny] = 2;
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (a[i][j] == 2) {
                cout << i + 1 << ' ' << j + 1 << '\n';
            }
        }
    }
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
            team[i][j] = group[i][j] = -1;
        }
    }

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
                team_list[team[i][j]].push_back(group[i][j]);
            }
        }
    }

    for (auto &list: team_list) {
        unq(list);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (a[i][j]) {
                group_pts[group[i][j]].emplace_back(i, j);
            }
        }
    }

    int max_comp = 0;
    timer = 0;
    for (int i = 0; i < teams_count; ++i) {
        for (int j: team_list[i]) {
            bool fl = true;
            vector <pair<int, int>> temp_pts;
            for (auto [x, y]: group_pts[j]) {
                for (int d = 0; d < 4; ++d) {
                    int nx = x + dx1[d], ny = y + dy1[d];
                    if (IsValid(nx, ny) && !a[nx][ny]) {
                        temp_pts.emplace_back(nx, ny);
                        a[nx][ny] = 2;
                    }
                }
            }

            timer++;
            for (auto [x, y]: temp_pts) {
                if (was[x][y] != timer) {
                    if (DfsIsDead(x, y, 2)) {
                        fl = false;
                    }
                }
            }

            for (auto [x, y]: temp_pts) {
                a[x][y] = 0;
            }

            if (fl) {
                team_alive_list[i].push_back(j);
            }
        }
        max_comp = max(max_comp, sz(team_alive_list[i]));
    }

    if (max_comp <= 20) {
        SolveMask();
    } else {
        cout << "0\n";
    }
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    freopen("/home/makson95/go/go-large/large", "r", stdin);
    freopen ("output.txt", "w", stdout);
    int test_count = 1;
    cin >> test_count;
    for (int i = 0; i < test_count; ++i) {
        Solve();
    }
}