/*
ребра как вершины //
*/
#include "garden.h"
#include "gardenlib.h"
#include <bits/stdc++.h>
//#include "grader.cpp"
#define pii pair<int, int>
#define fr first
#define sc second
#define pb push_back
#define mk make_pair
#define szof(s) (int)s.size()
#define all(s) s.begin(), s.end()
using namespace std;
 
const int MAXN = 300005;
int n, m, q, p;
 
//__________________
vector <pii> edges;
vector <pii> g[MAXN];
vector <int> eg[MAXN]; // edge graph
vector <int> reg[MAXN];
pii from_to[MAXN];
int mn1, mn2;
int pr[MAXN], siz[MAXN];
int lst[MAXN];
bool cycle[MAXN];
vector <int> used(MAXN, false);
int tiktak = 0;
int tin[MAXN], tout[MAXN];
int cycle_root[MAXN];
int dist[MAXN];
int cycle_size[MAXN];
vector <vector<int>> mn_dist;
//__________________
 
int findp(int v) {
  if (v == pr[v]) {
    return v;
  }
  return pr[v] = findp(pr[v]);
}
void unite(int a, int b) {
  a = findp(a);
  b = findp(b);
  if (a == b) {
    return;
  }
  if (siz[a] > siz[b]) {
    swap(a, b);
  }
  pr[a] = b;
  siz[b] += siz[a];
}
bool find_cycle(int v) {
  used[v] = 1;
  for (int to : eg[v]) {
    if (!used[to]) {
      lst[to] = v;
      bool f = find_cycle(to);
      if (f) {
        return true;          
      }
    }
    if (used[to] == 1) {
      int fn = v;
      while (fn != to) {
        cycle[fn] = true;
        fn = lst[fn];
      }
      cycle[to] = true;
      return true;
    }
  }
  used[v] = 2;
  return false;
}
void rev_dfs(int v, int root, int len = 0) {
  tin[v] = ++tiktak;
  cycle_root[v] = root;
  dist[v] = len;
  for (int to : reg[v]) {
    if (!cycle[to]) {
      rev_dfs(to, root, len + 1);
    }
  }
  tout[v] = tiktak;
}
void cycle_dfs(int v, int type, int len = 0) {
  mn_dist[type][v] = len;
  for (int to : eg[v]) {
    if (mn_dist[type][to] == -1) {
      cycle_dfs(to, type, len + 1);
    }
  }
}
bool father(int a, int b) {
  return (tin[a] <= tin[b] && tout[b] <= tout[a]);
}
void calc(vector <int> comp) {
  if (pr[comp[0]] != pr[mn1] && pr[comp[2]] != pr[mn2]) {
    return;
  }
  find_cycle(comp[0]);
  int CYCLE_SIZE = 0;
  for (int el : comp) {
    if (cycle[el]) {
      rev_dfs(el, el);
      CYCLE_SIZE++;
    }
  }
  for (int el : comp) {
    cycle_size[el] = CYCLE_SIZE;
  }
  cycle_dfs(mn1, 1);
  cycle_dfs(mn2, 2);
  for (int type = 1; type <= 2; type++) {
    for (int el : comp) {
      if (cycle[el]) {
        mn_dist[type][el] = (cycle_size[el] - mn_dist[type][el]) % cycle_size[el];
      }
    }
  }
}
void precalc(int sz) {
  mn1 = g[p][0].sc;
  if (szof(g[p]) > 1) {
    mn2 = g[p][1].sc;
  } else {
    mn2 = mn1;
  }
  mn_dist.resize(3);
  for (int i = 1; i <= 2; i++) {
    mn_dist[i].resize(sz, -1);
  }
  for (int i = 0; i < sz; i++) {
    pr[i] = i;
    siz[i] = 1;
    lst[i] = -1;
  }
  
  
  for (int i = 0; i < sz; i++) {
    for (int to : eg[i]) {
      unite(i, to);
      reg[to].pb(i);
    }
  }
  vector <int> comp[MAXN];
  for (int i = 0; i < sz; i++) {
    comp[findp(i)].pb(i);
  }
  for (int i = 0; i < sz; i++) {
    if (!comp[i].empty()) {
      calc(comp[i]);      
    }
  }
}
 
int tp;
bool check(int v, int fn, int k) {
  if (pr[v] != pr[fn]) {
    return 0;
  }
  if (cycle[v] && !cycle[fn]) {
    return 0;
  }
  
  if (!cycle[fn]) {
    if (tin[fn] <= tin[v] && tout[v] <= tout[fn] && dist[v] - dist[fn] == k) {
      return 1;
    }
  } else {
    tp = (fn == mn1 ? 1 : 2);
    assert(cycle_size[cycle_root[v]] > 0);
    if (k - dist[v] - mn_dist[tp][cycle_root[v]] >= 0 &&
    (k - dist[v] - mn_dist[tp][cycle_root[v]]) % cycle_size[cycle_root[v]] == 0) {
      return 1;
    }
  }
  return 0;
}
 
void count_routes(int N, int M, int P, int R[][2], int Q, int G[]) {
  n = N;
  m = M;
  q = Q;
  p = P;
  
  for (int i = 0; i < m; i++) {
    int x = R[i][0];
    int y = R[i][1];
    edges.pb({x, y});
    if (szof(g[x]) < 2) {
      g[x].pb({y, i});
    }
    if (szof(g[y]) < 2) {
      g[y].pb({x, i});
    }
  }
  int cnt = 0;
  for (int v = 0; v < n; v++) {
    for (auto &edge : g[v]) {
      int to = edge.fr, ind = edge.sc;
      from_to[cnt] = {v, to};
      edge.sc = cnt;
      cnt++;
    }
  }
  int sz = 0;
  for (int v = 0; v < n; v++) {
    for (auto edge : g[v]) {
      sz++;
      int to = edge.fr, ind = edge.sc;
      if (szof(g[to]) == 1) {
        eg[ind].pb(g[to][0].sc);
        continue;
      }
      int cur = edge.sc;
      int mn1 = g[to][0].sc;
      int mn2 = g[to][1].sc; 
      int x = from_to[cur].fr, y = from_to[cur].sc;
      int xx = from_to[mn1].fr, yy = from_to[mn1].sc;
      if (x > y) {
        swap(x, y);
      } 
      if (xx > yy) {
        swap(xx, yy);
      }
      if (!(x == xx && y == yy)) {
        eg[ind].pb(mn1);
      } else {
        eg[ind].pb(mn2);
      }
    }
  }
  precalc(sz);
  int ans;
  
  int best[MAXN];
  for (int i = 0; i < n; i++) {
    best[i] = g[i][0].sc;
  }
  
  for (int i = 0; i < q; i++) {
    ans = 0;
    bool ok;
    for (int v = 0; v < n; v++) {
      ok = check(best[v], mn1, G[i]);
      ok |= check(best[v], mn2, G[i]);
      ans += ok;
    }
    answer(ans);
  }
}
