#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif
#define x(p) (p).first
#define y(p) (p).second

typedef pair<int, int> pii;

struct line {
    int group;
    pii s, e;
};
struct truth_node {
    bool tval;
    int idx;
};

int ccw(line l, pii p) {
    int t1 = x(l.e) - x(l.s);
    int t2 = y(l.e) - y(l.s);
    int t3 = x(p) - x(l.s);
    int t4 = y(p) - y(l.s);
    int r = t1 * t4 - t2 * t3;
    if (r < 0) return -1;
    if (r > 0) return 1;
    return 0;
}
bool xross(line l1, line l2) {
    int c1 = ccw(l1, l2.s);
    int c2 = ccw(l1, l2.e);
    int c3 = ccw(l2, l1.s);
    int c4 = ccw(l2, l1.e);
    return c1 * c2 < 0 && c3 * c4 < 0;
}
int n, N;
line a[3030];
truth_node node[6060];
vector <int> e[6060];
vector <int> e_rev[6060];
int dfn[6060];

int main() {
    scanf("%d",&n);
    N = 3*n;
    int i,j;
    for(i=0;i<N;i++) {
        scanf("%d%d%d%d", &x(a[i].s), &y(a[i].s), &x(a[i].e), &y(a[i].e));
        a[i].group = i / 3;
        node[2*i].tval = true;
        node[2*i].idx = i;
        node[2*i+1].tval = false;
        node[2*i+1].idx = i;
        if (i % 3 == 2) {
            int base = 2*(i-2);
            for(int k1 = 0; k1 < 3; k1++) {
                for(int k2 = 0; k2 < 3; k2++) {
                    if (k1 == k2) continue;
                    // Condition 1. if a line in a group is excluded, other lines must be included.
                    e[base + 2*k1+1].push_back(base + 2 * k2);
                    e_rev[base + 2 * k2].push_back(base + 2*k1+1);
                }
            }
        }
    }
    for(i=0;i<N;i++) {
        for(j=i+1;j<N;j++) {
            if (xross(a[i], a[j])) {
                // Condition 2. if a line X and a line Y are crossing each other and X is included, Y must be excluded.
                e[2*i].push_back(2*j+1);
                e_rev[2*j+1].push_back(2*i);
                e[2*j].push_back(2*i+1);
                e_rev[2*i+1].push_back(2*j);
            }
        }
    }
    for(i=0;i<2*N;i++) {
        if (!dfn[i]) {
            dfs(i);
        }
    }
    return 0;
}