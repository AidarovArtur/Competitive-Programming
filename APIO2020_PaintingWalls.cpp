#include <bits/stdc++.h>
#include "paint.h"
//#include "grader.cpp"
#define chkmin(a, b) a = min(a, b)
#define chkmax(a, b) a = max(a, b)
#define pb push_back
#define all(s) s.begin(), s.end()
#define szof(s) (int)s.size()
using namespace std;
 
const int MAXN = (int)1e5 + 5;
const int INF = 1e9 + 7;
 
int pos[MAXN];
int dp[MAXN];
int pref[MAXN];
 
struct Segment_Tree {
	vector <int> tree;
	vector <int> lazy;
	Segment_Tree (int n) {
		tree.resize(n * 4, INF);
		lazy.resize(n * 4, INF);
	}
	void push(int v, int tl, int tr) {
		if (lazy[v] == INF) {
			return;
		}
		if (tl != tr) {
			chkmin(lazy[v + v], lazy[v]);
			chkmin(lazy[v + v + 1], lazy[v]);
		}
		chkmin(tree[v], lazy[v]);
		lazy[v] = INF;
	}
	void upd(int l, int r, int val, int v, int tl, int tr) {
		push(v, tl, tr);
		if (l > tr || tl > r) {
			return;
		}
		if (l <= tl && tr <= r) {
			lazy[v] = val;
			push(v, tl, tr);
			return;
		}
		int mid = (tl + tr) >> 1;
		upd(l, r, val, v + v, tl, mid);
		upd(l, r, val, v + v + 1, mid + 1, tr);
		tree[v] = min(tree[v + v], tree[v + v + 1]);
	}
	int get(int l, int r, int v, int tl, int tr) {
		push(v, tl, tr);
		if (l > tr || tl > r) {
			return INF;
		}
		if (l <= tl && tr <= r) {
			return tree[v];
		}
		int mid = (tl + tr) >> 1;
		return min(get(l, r, v + v, tl, mid), get(l, r, v + v + 1, mid + 1, tr));
	}
};
 
vector <int> color[MAXN];
 
int used[MAXN], used_id = 1;
 
int minimumInstructions(int N, int M, int K, vector<int> C, vector<int> A, vector<vector<int>> B) {
	for (int i = 0; i < M; i++) {
		for (int el : B[i]) {
			color[el].pb(i);
		}
	}
	vector <vector<int>> Q;
	vector <int> MAXQ(N + 1, 0);
	Q.resize(2);
	Q[0].resize(N + 1, 0);
	Q[1].resize(N + 1, 0);
	int type = 0;
	memset(used, -1, sizeof(used));
	for (int i = N - 1; i >= 0; i--) {
		for (int p : color[C[i]]) {
			if (i == N - 1) {
				Q[type][p] = 1;
				used[p] = i;
			} else {
				if (used[(p + 1) % M] == i + 1) {
					Q[type][p] = 1 + Q[type ^ 1][(p + 1) % M];
				} else {
					Q[type][p] = 1;
				}
			}
			MAXQ[i] = max(MAXQ[i], Q[type][p]);
		}
		for (int p : color[C[i]]) {
			used[p] = i;
		}
		type ^= 1;
	}
	
	auto can = [&](int l, int r) {
		return l + MAXQ[l] - 1 >= r;
	};
	fill(dp, dp + MAXN, INF);
	Segment_Tree T(N);
	if (can(0, M - 1)) {
		T.upd(0, M - 1, 1, 1, 0, N - 1);
	}
	for (int i = M; i < N; i++) {
		if (can(i - M + 1, i)) {
			int best = T.get(i - M, i - 1, 1, 0, N - 1);
			T.upd(i - M + 1, i, best + 1, 1, 0, N - 1);
		}
	}
	if (T.get(N - 1, N - 1, 1, 0, N - 1) < INF) {
		return T.get(N - 1, N - 1, 1, 0, N - 1);
	} else {
		return -1;
	}
}
 
/*
5 3 10
0 2 1 4 3
2 0 4
2 3 2
1 1
*/
