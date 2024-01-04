#include <bits/stdc++.h>
#define fr first
#define sc second
#define pb push_back
#define szof(s) (int)s.size()
#define all(s) s.begin(), s.end()
#define pii pair<int, int>
#define all(s) s.begin(), s.end()
#define prev myrza4321
#define y1 myrza1234
#define OK puts("OK")
using namespace std;
 
const int N = (int)1e5 + 5;
const int INF = 0x3f3f3f3f;
 
int col[N], sz[N], n, good;
vector <int> g[N];
int tiktak = 1, tin[N], tout[N], d[N];
int pr[N][25];
 
vector <pii> par[N], son[N];
set <pii> black[N];
vector <int> g1[N];
int root = -1;
bool used[N];
int nn = 0;
 
void dfs(int v, int p, int len = 0) {
  tin[v] = tiktak++;
  pr[v][0] = p;
  d[v] = len;
  for (int i = 1; i <= 20; i++) {
    pr[v][i] = pr[pr[v][i - 1]][i - 1];
  }
  for (int to : g[v]) {
    if (to == p) {
      continue;
    }
    dfs(to, v, len + 1);
  }
  tout[v] = tiktak++;
}
 
bool father(int a, int b) {
  return tin[a] <= tin[b] && tout[a] >= tout[b];
}
 
int lca(int a, int b) {
  if (father(a, b)) {
    return a;
  }
  if (father(b, a)) {
    return b;
  }
  for (int i = 20; i >= 0; i--) {
    if (!father(pr[a][i], b)) {
      a = pr[a][i];
    }
  }
  return pr[a][0];
}
 
void precalc() {
  dfs(1, 1);
}
 
 
int getD(int a, int b) {
  return d[a] + d[b] - 2 * d[lca(a, b)];
}
void calc(int v, int p = -1) {
  sz[v] = 1;
  for (int to : g[v]) {
    if (used[to] || to == p) {
      continue;
    }
    calc(to, v);
    sz[v] += sz[to];
  }
}
 
int find_Centroid(int v, int p = -1) {
  for (int to : g[v]) {
    if (used[to] || to == p) {
      continue;
    }
    if (sz[to] >= nn / 2) {
      return find_Centroid(to, v);
    }
  }
  return v;
}
 
 
void build(int v, int level = 0, int last = -1) {
  calc(v);
  nn = sz[v];
  int centroid = find_Centroid(v);
  used[centroid] = 1;
 
  if (root == -1) {
    root = centroid;
  }
 
  if (level > 0) {
    g1[centroid].pb(last);
    g1[last].pb(centroid);
  }
  for (int to : g[centroid]) {
    if (used[to] != 1) {
      build(to, level + 1, centroid);
    }
  }
}
 
 
void get(int v, int p = -1, vector <int> path = {}) {
  for (int i = szof(path) - 1; i >= 0; i--) {
    int to = path[i];
    par[v].pb({getD(v, to), to}), son[to].pb({getD(v, to), v});
  }
  par[v].pb({0, v});
  son[v].pb({0, v});
 
  path.pb(v);
  for (int to : g1[v]) {
    if (to == p) {
      continue;
    }
    get(to, v, path);
  }
}
 
 
void change(int v) {
  if (col[v] == 0) {
    good++;
    col[v] = 1;
    for (pii to : par[v]) {
      black[to.sc].insert({getD(v, to.sc), v});
    }
  } else {
    good--;
    col[v] = 0;
    for (pii to : par[v]) {
      black[to.sc].erase({getD(v, to.sc), v});
    }
  }
}
 
int get_ans(int v) {
  int mn = INF;
  for (pii to : par[v]) {
    if (black[to.sc].empty()) {
      continue;
    }
    pii cur = *black[to.sc].begin();
    mn = min(mn, to.fr + cur.fr);
  }
  return mn;
}
 
main() {
  cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v;
    scanf("%d %d", &u, &v);
    g[u].pb(v);
    g[v].pb(u);
  }
 
  precalc();
  build(1);
  get(root);
 
  int Q;
  cin >> Q;
 
  while (Q--) {
    int tp, v;
    scanf("%d", &tp);
    if (tp == 0) {
      scanf("%d", &v);
      change(v);
    } else {
      scanf("%d", &v);
      if (!good) {
        puts("-1");
        continue;
      }
      printf("%d\n", get_ans(v));
    }
  }
}
