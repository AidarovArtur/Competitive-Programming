#include "highway.h"
//#include "grader.cpp"
#include <bits/stdc++.h>
#define pii pair<int, int>
#define fr first
#define sc second
#define szof(s) (int)s.size()
#define all(s) s.begin(), s.end()
#define mk make_pair
#define pb push_back
#define ll long long
using namespace std;
 
const int MAXN = 130005;
const ll INF = 0x3f3f3f3f3f3f3f3f;
 
int n, m;
vector <pair<int, int>> G[MAXN]; // обычный граф
vector <pair<int, int>> tree[MAXN]; // дерево
int d[MAXN];
int tin[MAXN];
int tout[MAXN];
int tiktak = 0;
int edge[MAXN][2];
bool used[MAXN];
int ROOT, SUB_S;
vector <int> BFS_EDGES;
vector <int> bfs_edges[3]; // already sorted by id
map <pii, int> id;
 
ll costa, costb;
ll diametr;
ll min_path, max_path;
 
ll get_min_path() {
    vector <int> w(m);
    for (int i = 0; i < m; i++) {
        w[i] = 0;
    }
    ll cur_cost = ask(w);
    return cur_cost;
}
 
int find_root() {
    vector <int> w(m);
    for (int i = 0; i < m; i++) {
        w[i] = 0;
    }
    w[0] = 1;
    ll cur_cost;
    int l = -1, r = m - 1;
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        for (int i = 0; i < m; i++) {
            w[i] = 0;
        }
        for (int i = 0; i <= mid; i++) {
            w[i] = 1;
        }
        cur_cost = ask(w);
        if (cur_cost == min_path) {
            l = mid;
        } else {
            r = mid;
        }
    }
    ROOT = edge[r][0];
    SUB_S = edge[r][1];
    return edge[r][0];
}
 
void create_tree(int root) {
    BFS_EDGES.clear();
    memset(used, 0, sizeof(used));
    for (int i = 0; i < MAXN; i++) {
        tree[i].clear();
    }
    // найти ребро на пути S -> T
    queue <int> q;
    vector <int> dist(MAXN, -1);
    dist[root] = 0;
    q.push(root);
    
    assert(id.count(mk(root, SUB_S)));
    
    for (int i = 1; i < szof(G[root]); i++) {
        if (G[root][i].fr == SUB_S) {
            swap(G[root][0], G[root][i]);
            break;
        }
    }
    
    // почему ? надо разобраться
    //int sub_id = id[mk(root, SUB_S)];
    //used[SUB_S] = true;
    //BFS_EDGES.pb(sub_id);
    //tree[root].pb({SUB_S, sub_id});
    //tree[sub_id].pb({SUB_S, root});
    //dist[SUB_S] = 1;
    //q.push(SUB_S);
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (pii to : G[v]) {
            if (dist[to.fr] == -1) {
                used[to.sc] = true;
                BFS_EDGES.pb(to.sc);
                tree[v].pb({to.fr, to.sc});
                tree[to.fr].pb({v, to.sc});
                dist[to.fr] = dist[v] + 1;
                q.push(to.fr);
            }
        }
    }
}
void calc(int v, int par, int len) {
    tin[v] = ++tiktak;
    d[v] = len;
    for (pii to : tree[v]) {
        if (to.fr != par) {
            calc(to.fr, v, len + 1);            
        }
    }
    tout[v] = tiktak;
}
bool father(int a, int b) {
    return tin[a] <= tin[b] && tout[b] <= tout[a];
}
vector <int> subtree[3];
void dfs(int v, int par) {
    if (father(SUB_S, v)) {
        subtree[1].pb(v);
    } else {
        subtree[2].pb(v);
    }
    for (pii to : tree[v]) {
        if (to.fr != par) {
            dfs(to.fr, v);
        }
    }
}
 
int find_ans(int cur_root, vector <int> cur_bfs_edges) {
    if (cur_bfs_edges.empty()) {
        return cur_root;
    }
    int l = -1, r = szof(cur_bfs_edges);
    vector <int> w(m);
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        for (int i = 0; i < m; i++) {
            w[i] = 1;
        }
        for (int el : BFS_EDGES) {
            w[el] = 0;
        }
        for (int i = mid; i < szof(cur_bfs_edges); i++) {
            w[cur_bfs_edges[i]] = 1;
        }
        ll cur_cost = ask(w);
        if (cur_cost == min_path) {
            r = mid;
        } else {
            l = mid;
        }
    }
    if (l == -1) {
        return cur_root;
    }
    int x = edge[cur_bfs_edges[l]][0];
    int y = edge[cur_bfs_edges[l]][1];
    return (d[x] > d[y] ? x : y);
}
 
void find_pair(int N, vector<int> U, vector<int> V, int AA, int BB) {
    m = szof(U);
    n = N;
    costa = AA;
    costb = BB;
    min_path = get_min_path(); // 1 q
    diametr = min_path / costa;
    max_path = diametr * costb;
    for (int i = 0; i < m; i++) {
        int x, y;
        x = U[i];
        y = V[i];
        id[{x, y}] = i;
        id[{y, x}] = i;
        edge[i][0] = x;
        edge[i][1] = y;
        G[x].pb({y, i});
        G[y].pb({x, i});
    }
    int s, t;
    int root = find_root(); // log(n)
    create_tree(root);
    calc(root, -1, 0);
    dfs(root, -1);
    sort(all(subtree[1]));
    sort(all(subtree[2]));
    auto exist = [&](vector <int> &vec, int x) {
        bool ok = binary_search(all(vec), x);
        return ok;
    };
    for (int el : BFS_EDGES) {
        int x = edge[el][0];
        int y = edge[el][1];
        if ((x == ROOT && y == SUB_S) || (x == SUB_S && y == ROOT)) {
            continue;
        }
        if (exist(subtree[1], x)) {
            bfs_edges[1].pb({id[{x, y}]});
        } else {
            bfs_edges[2].pb({id[{x, y}]});
        }
    }
    s = find_ans(SUB_S, bfs_edges[1]); // log(n) - 1
    t = find_ans(ROOT, bfs_edges[2]); // log(n) - 1
    answer(s, t);
}
