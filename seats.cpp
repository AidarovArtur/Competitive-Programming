/*
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░░░░░░░░░░░░░░░░▄▄███▄▄▄░▄▄██▄░░░░░░░
░░░░░░░░░██▀███████████████▀▀▄█░░░░░░
░░░░░░░░█▀▄▀▀▄██████████████▄░░█░░░░░
░░░░░░░█▀▀░▄██████████████▄█▀░░▀▄░░░░
░░░░░▄▀░░░▀▀▄████████████████▄░░░█░░░
░░░░░▀░░░░▄███▀░░███▄████░████░░░░▀▄░
░░░▄▀░░░░▄████░░▀▀░▀░░░░░░██░▀▄░░░░▀▄
░▄▀░░░░░▄▀▀██▀░░░░░▄░░▀▄░░██░░░▀▄░░░░
█░░░░░█▀░░░██▄░░░░░▀▀█▀░░░█░░░░░░█░░░
█░░░▄▀░░░░░░██░░░░░▀██▀░░█▀▄░░░░░░▀▀▀
▀▀▀▀░▄▄▄▄▄▄▀▀░█░░░░░░░░░▄█░░█▀▀▀▀▀█░░
░░░░█░░░▀▀░░░░░░▀▄░░░▄▄██░░░█░░░░░▀▄░
░░░░█░░░░░░░░░░░░█▄▀▀▀▀▀█░░░█░░░░░░█░
░░░░▀░░░░░░░░░░░░░▀░░░░▀░░░░▀░░░░░░░░
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ ░░░░░░░░
*/
#include "seats.h"
#include <bits/stdc++.h>
#define chkmax(a, b) a = max(a, (b))
#define chkmin(a, b) a = min(a, (b))
 
#define fr first
#define sc second
#define pii pair<int, int>
#define szof(s) (int)s.size()
#define all(s) s.begin(), s.end()
using namespace std;
 
const int N = (int)1e6 + 6;
const int INF = 0x3f3f3f3f;
 
struct mnmx {
  int mn, mx;
  mnmx() {
    mn = INF, mx = -INF;
  }
};
 
int get(mnmx a, mnmx b) {
  return (a.mx - a.mn + 1) * (b.mx - b.mn + 1);
}
 
struct subtask12 {
  vector <int> posy, posx;
  vector <int> id;
  int n, m;
  
  bool solution = false;
 
  void give_initial_chart(int H, int W, vector <int> R, vector <int> C) {
    posy.resize(H * W);
    posx.resize(H * W);
    n = H, m = W;
    for (int i = 0; i < H * W; i++) {
      posy[i] = R[i];
      posx[i] = C[i];
    }
  }
  int swap_seats(int a, int b) {
    swap(posy[a], posy[b]);
    swap(posx[a], posx[b]);
    mnmx R, C;
    int ans = 0;
    for (int i = 0; i < n * m; i++) {
      chkmin(R.mn, posy[i]);
      chkmax(R.mx, posy[i]);
      chkmin(C.mn, posx[i]);
      chkmax(C.mx, posx[i]);
      if (get(R, C) == i + 1) {
        ans++;
      }
    }
    return ans;
  }
} small;
 
struct subtask3 {
  /*
  создать дерево отрезков чтобы отвечать на запросы min max y/x
  */
  vector <int> y, x;
  struct node {
    int mn, mx;
    node(int x) {
      mn = mx = x;
    }
    node() {
      mn = INF, mx = -INF;
    }
    node operator + (const node& other) {
      node res;
      res.mn = min(mn, other.mn);
      res.mx = max(mx, other.mx);
      return res;
    }
  };
 
  struct Tree {
    node* tree;
    Tree (int Area) {
      tree = (node*)malloc(Area * sizeof (node));
    }
    Tree() {
      tree = nullptr;
    }
    void build(int v, int tl, int tr, vector <int> &arr) { // 0 индексация
      if (tl == tr) {
        tree[v] = node(arr[tl]);
        return;
      }
      int mid = (tl + tr) >> 1;
      build(v * 2 + 1, tl, mid, arr);
      build(v * 2 + 2, mid + 1, tr, arr);
      tree[v] = tree[v * 2 + 1] + tree[v * 2 + 2];
    }
    void upd(int pos, int val, int v, int tl, int tr) {
      if (tl == tr) {
        tree[v] = node(val);
        return;
      }
      int mid = (tl + tr) >> 1;
      if (pos <= mid) {
        upd(pos, val, v * 2 + 1, tl, mid);
      }
      else {
        upd(pos, val, v * 2 + 2, mid + 1, tr);
      }
      tree[v] = tree[v * 2 + 1] + tree[v * 2 + 2];
    }
    node get(int l, int r, int v, int tl, int tr) {
      if (l <= tl && tr <= r) {
        return tree[v];
      }
      if (l > tr || tl > r) {
        return node();
      }
      int mid = (tl + tr) >> 1;
      return get(l, r, v * 2 + 1, tl, mid) + get(l, r, v * 2 + 2, mid + 1, tr);
    }
  };
  bool solution = false;
  Tree row, col;
  int Area;
  
  int ourH, ourW;
 
  void give_initial_chart(int H, int W, vector <int> R, vector <int> C) {
    assert(H == 1000 && W == 1000);
    y.resize(H * W);
    x.resize(H * W);
    Area = H * W;
    ourH = H;
    ourW = W;
    for (int i = 0; i < Area; i++) {
      y[i] = R[i], x[i] = C[i];
    }
    row = Tree(Area * 3);
    col = Tree(Area * 3);
    row.build(0, 0, Area - 1, y);
    col.build(0, 0, Area - 1, x);
  }
  int swap_seats(int a, int b) {
    int pos = 0, ans = 0;
    swap(x[a], x[b]);
    swap(y[a], y[b]);
    row.upd(a, y[a], 0, 0, Area - 1);
    col.upd(a, x[a], 0, 0, Area - 1);
    row.upd(b, y[b], 0, 0, Area - 1);
    col.upd(b, x[b], 0, 0, Area - 1);
    while (pos < Area) { 
      node R = row.get(0, pos, 0, 0, Area - 1);
      node C = col.get(0, pos, 0, 0, Area - 1);
      int rectangle = (R.mx - R.mn + 1) * (C.mx - C.mn + 1);
      if (rectangle == pos + 1) {
        ans++, pos++;
      }
      else {
        pos = rectangle - 1;
      }
    }
    return ans;
  }
} a_plus_b;
 
struct subtask5 {
  bool solution = false;
  struct node {
    int mn, cnt;
    node(int x) {
      mn = x, cnt = 1;
    }
    node() {
      mn = INF, cnt = 0;
    }
    node operator + (const node& other) {
      node res;
      res.mn = min(mn, other.mn);
      res.cnt = 0;
      if (res.mn == mn) res.cnt += cnt;
      if (res.mn == other.mn) res.cnt += other.cnt;
      return res;
    }
  };
 
  node tree[4 * N];
  int add[4 * N];
  int pos[N], arr[N];
  int n;
  
  void push(int v, int tl, int tr) {
    if (add[v] == 0) {
      return;
    }
    tree[v].mn += add[v];
    if (tl != tr) {
      add[v + v + 1] += add[v];
      add[v + v + 2] += add[v];
    }
    add[v] = 0;
  }
 
  void build(int v, int tl, int tr) {
    if (tl == tr) {
      tree[v] = node(tl);
      return;
    }
    int mid = (tl + tr) >> 1;
    build(v + v + 1, tl, mid);
    build(v + v + 2, mid + 1, tr);
    tree[v] = tree[v + v + 1] + tree[v + v + 2];
  }
 
  void upd(int ql, int qr, int val, int v, int tl, int tr) {
    push(v, tl, tr);
    if (ql > tr || tl > qr) {
      return;
    }
    if (ql <= tl && tr <= qr) {
      add[v] += val;
      push(v, tl, tr);
      return;
    }
    int mid = (tl + tr) >> 1;
    upd(ql, qr, val, v + v + 1, tl, mid);
    upd(ql, qr, val, v + v + 2, mid + 1, tr);
    tree[v] = tree[v + v + 1] + tree[v + v + 2];
  }
  int get(int pos, int v, int tl, int tr) {
    push(v, tl, tr);
    if (tl == tr) {
      return tree[v].mn;
    }
    int mid = (tl + tr) >> 1;
    if (pos <= mid) {
      return get(pos, v + v + 1, tl, mid);
    }
    else {
      return get(pos, v + v + 2, mid + 1, tr);
    }
  }
  void give_initial_chart(int H, int W, vector <int> R, vector <int> C) {
    assert(H == 1);
    n = W;
 
    for (int i = 0; i < n; i++) {
      pos[i] = C[i];
    }
 
    for (int i = 0; i < n; i++) {
      arr[pos[i]] = i;
    }
 
    build(0, 0, n - 1);
 
    for (int i = 0; i + 1 < n; i++) {
      int mx = max(arr[i], arr[i + 1]);
      upd(mx, n - 1, -1, 0, 0, n - 1);
    }
  }
  int swap_seats(int a, int b) {
    set <pair<int, int>> go;
    if (pos[a] > 0) {
      go.insert({ pos[a] - 1, pos[a] });
    }
    if (pos[a] + 1 < n) {
      go.insert({ pos[a], pos[a] + 1 });
    }
    if (pos[b] > 0) {
      go.insert({ pos[b] - 1, pos[b] });
    }
    if (pos[b] + 1 < n) {
      go.insert({ pos[b], pos[b] + 1 });
    }
    for (pii pr : go) {
      int mx = max(arr[pr.fr], arr[pr.sc]);
      upd(mx, n - 1, 1, 0, 0, n - 1);
    }
    swap(arr[pos[a]], arr[pos[b]]);
    swap(pos[a], pos[b]);
    for (pii pr : go) {
      int mx = max(arr[pr.fr], arr[pr.sc]);
      upd(mx, n - 1, -1, 0, 0, n - 1);
    }
    return tree[0].cnt;
  }
} line;
 
void give_initial_chart(int H, int W, vector <int> R, vector <int> C) {
  if (H * W <= 10000) {
    small.solution = true;
    small.give_initial_chart(H, W, R, C);
  }
  else if (H == 1) {
    line.solution = true;
    line.give_initial_chart(H, W, R, C);
  }
  else if (H <= 1000 && W <= 1000) {
    a_plus_b.solution = true;
    a_plus_b.give_initial_chart(H, W, R, C);
  }
}
 
int swap_seats(int a, int b) {
  if (line.solution) {
    return line.swap_seats(a, b);
  }
  if (small.solution) {
    return small.swap_seats(a, b);
  }
  if (a_plus_b.solution) {
    return a_plus_b.swap_seats(a, b);
  }
  assert(false);
}
