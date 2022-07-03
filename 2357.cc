#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <limits>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define MIN(p) (p).first
#define MAX(p) (p).second

typedef pair<int, int> pii;
typedef long long llint;

int n,lgn,m;
pii a[18][100010];

inline int lg2(int x) {
    int p = 0;
    for(; x > (1<<p); p++);
    return p;
}

void input() {
    cin >> n >> m;
    lgn = lg2(n);
    for(int i=0;i<n;i++) {
        int x;
        cin >> x;
        a[0][i] = {x, x};
    }
    for(int j=1;j<lgn;j++) {
        for(int i=0;i<n;i++) {
            if (i + (1<<j) > n) {
                break;
            }
            int k = i + (1<<(j-1));
            MIN(a[j][i]) = MIN(a[j-1][i]) < MIN(a[j-1][k]) ? MIN(a[j-1][i]) : MIN(a[j-1][k]);
            MAX(a[j][i]) = MAX(a[j-1][i]) > MAX(a[j-1][k]) ? MAX(a[j-1][i]) : MAX(a[j-1][k]);
        }
    }
}
pii query(int s, int e) {
    int l = e-s+1, i = s, k = 0;
    pii q = {numeric_limits<int>().max(), numeric_limits<int>().min()};
    while(l) {
        if (l%2) {
            const pii &r = a[k][i];
            MIN(q) = MIN(q) < MIN(r) ? MIN(q) : MIN(r);
            MAX(q) = MAX(q) > MAX(r) ? MAX(q) : MAX(r);
            i += 1<<k;
        }
        l >>= 1;
        k++;
    }
    return q;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    input();
    while(m--) {
        int s, e;
        cin >> s >> e;
        s--;
        e--;
        pii ans = query(s, e);
        cout << MIN(ans) << " " << MAX(ans) << "\n";
    }
    return 0;
}
