#include <bits/stdc++.h>
#define fr first
#define sc second
#define pii pair<int, int>
#define pb push_back
#define szof(s) (int)s.size()
#define all(s) s.begin(), s.end()
#define fastInp ios_base::sync_with_stdio(0); cin.tie(0);
#define int long long
using namespace std;
 
const int MAXN = 30005;
const int SHIFT = 50000;
const int S = 95;
const int INF = 1e9;
 
int pos[MAXN], p[MAXN];
int n, m;
 
struct node {
	int v, pw, cost;
	node () {
		
	}
	node (int v_, int pw_, int cost_) {
		v = v_;
		pw = pw_;
		cost = cost_;
	}
};
 
struct Compare_node {
	bool operator () (node const &p1, node const &p2) {
		if (p1.cost != p2.cost) {
			return p1.cost < p2.cost;			
		}
		if (p1.v != p2.v) {
			return p1.v < p2.v;
		}
		return p1.pw < p2.pw;
	}
};
 
vector <int> big[MAXN];
vector <int> small[MAXN];
vector <node> g[MAXN][S];
int dist[MAXN][S];
 
bool in(int pos) {
	return 0 <= pos && pos < n;
}
 
signed main() {
	fastInp;
	cin >> n >> m;
	set <int> all_small;
	for (int i = 0; i < m; i++) {
		cin >> pos[i] >> p[i];
		if (p[i] >= S) {
			big[pos[i]].pb(p[i]);
		} else {
			small[pos[i]].pb(p[i]);
			all_small.insert(p[i]);
		}
	}
	for (int i = 0; i < n; i++) {
		sort(all(small[i]));
		small[i].erase(unique(all(small[i])), small[i].end());
		for (int val : small[i]) {
			g[i][0].pb(node(i, val, 0));
		}
	}
	set <node, Compare_node> pq;
	vector <bool> used(MAXN, false);
	memset(dist, -1, sizeof(dist));
	pq.insert(node(pos[0], 0, 0));
	dist[pos[0]][0] = 0;
	while (!pq.empty()) {
		int v = pq.begin()->v;
		int pw = pq.begin()->pw;
		int cost = pq.begin()->cost;
		pq.erase(pq.begin());
		if (cost > dist[v][pw]) {
			continue;
		}
		if (!used[v]) { // впервые добрались до v
			used[v] = 1;
			for (int el : big[v]) {
				for (int to = v % el; to < n; to += el) {
					int c = abs(v - to) / el;
					if (dist[to][0] == -1 || dist[to][0] > cost + c) {
						dist[to][0] = cost + c;
						pq.insert(node(to, 0, dist[to][0]));
					}
				}
			}
		}
		// change our pw
		if (dist[v][0] == -1 || dist[v][0] > dist[v][pw]) {
			dist[v][0] = dist[v][pw];
			pq.insert(node(v, 0, dist[v][pw]));
		}
		if (pw == 0) {
			for (int el : small[v]) {
				int new_v = v;
				int new_pw = el;
				if (!in(new_v)) {
					continue;
				}			
				int c = 0;
				if (dist[new_v][new_pw] == -1 || dist[new_v][new_pw] > dist[v][pw] + c) {
					dist[new_v][new_pw] = dist[v][pw] + c;
					pq.insert(node(new_v, new_pw, dist[new_v][new_pw]));
				}		
			}
		}
		for (int type = -1; type <= 1; type++) {
			if (type == 0) {
				continue;
			}
			int new_v = v + type * pw;
			int new_pw = pw;
			if (!in(new_v)) {
				continue;
			}			
			int c = 1;
			if (dist[new_v][new_pw] == -1 || dist[new_v][new_pw] > dist[v][pw] + c) {
				dist[new_v][new_pw] = dist[v][pw] + c;
				pq.insert(node(new_v, new_pw, dist[new_v][new_pw]));
			}		
		}
	}
	cout << dist[pos[1]][0] << endl;
}
