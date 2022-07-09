#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define endl "\n"
#define L(p) ((p).first)
#define R(p) ((p).second)

typedef pair<int, int> pii;
typedef long long llint;

int n;
pii a[500050];
int seq[500050];

bool cmp(pii a, pii b) {
  return L(a) > L(b) || (L(a) == L(b) && R(a) < R(b));
}
void input() {
    cin >> n;
    int x;
    for(int i=0;i<n;i++) {
        cin >> x >> L(a[i]) >> R(a[i]);
    }
}
int lowerbound(int l, int x) {
    int s = 0, e = l-1;
    int pos = -1;
    while (s <= e) {
        int p = s + (e - s)/2;
        if (seq[p] <= x) {
            pos = p;
            s = p + 1;
        } else {
            e = p - 1;
        }
    }
    return pos;
}
void solve() {
    sort(a, a+n, cmp);
    int l = 1;
    seq[0] = R(a[0]);
    for(int i=1;i<n;i++) {
        if (a[i] == a[i-1]) continue;
        int p = lowerbound(l, R(a[i]));
        if (p == l - 1 && seq[p] <= R(a[i])) {
            seq[l++] = R(a[i]);
        } else {
            seq[p+1] = R(a[i]);
        }
    }
    cout << l << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    input();
    solve();
    return 0;
}
