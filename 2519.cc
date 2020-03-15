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
#define POS(t) 2*(t)
#define NEG(t) 2*(t) + 1
#define POSV(v) (v)/2*2
#define NEGV(v) (v)/2*2+1
#define COMPV(v) (v)/2*2 + !((v)%2)
#define IS_POS(v) (v)%2 == 0
#define IS_NEG(v) (v)%2 == 1

const int VISITED_ONCE = 1, VISITED_TWICE = 2;

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
vector <vector <int>> sccs;
int current_order;
int nth_v[6060];
int check[6060];
int scc_label[6060];
int truth[6060];

void first_dfs(int v) {
    check[v] = VISITED_ONCE;
    int k = e[v].size();
    for(int i=0;i<k;i++) {
        int u = e[v][i];
        if (check[u] != VISITED_ONCE) {
            first_dfs(u);
        }
    }
    nth_v[current_order--] = v;
}

void second_dfs(vector<int> &scc, int label, int v) {
    check[v] = VISITED_TWICE;
    int k = e_rev[v].size();
    for(int i=0;i<k;i++) {
        int u = e_rev[v][i];
        if (check[u] != VISITED_TWICE) {
            second_dfs(scc, label, u);
        }
    }
    scc.push_back(v);
    scc_label[v] = label;
    debug("%d has label %d", v, label);
}

int main() {
    scanf("%d",&n);
    N = 3*n;
    int V = 2 * N;
    int i,j;
    for(i=0;i<N;i++) {
        scanf("%d%d%d%d", &x(a[i].s), &y(a[i].s), &x(a[i].e), &y(a[i].e));
        a[i].group = i / 3;
        node[POS(i)].tval = true;
        node[POS(i)].idx = i;
        node[NEG(i)].tval = false;
        node[NEG(i)].idx = i;
        if (i % 3 == 2) {
            // Condition 1. if a line in a group is excluded, other lines must be included.
            for(j = i - 2; j <= i; j++) {
                for(int k = i - 2; k <= i; k++) {
                    if (j == k) continue;
                    e[NEG(j)].push_back(POS(k));
                    e_rev[POS(k)].push_back(NEG(j));
                }
            }
        }
    }
    for(i=0;i<N;i++) {
        for(j=i+1;j<N;j++) {
            if (xross(a[i], a[j])) {
                // Condition 2. if a line X and a line Y are crossing each other and X is included, Y must be excluded.
                e[POS(i)].push_back(NEG(j));
                e_rev[NEG(j)].push_back(POS(i));
                e[POS(j)].push_back(NEG(i));
                e_rev[NEG(i)].push_back(POS(j));
            }
        }
    }

    for(i=0;i<V;i++) {
        debug("endpoint from %d", i);
        for(j=0;j<e[i].size();j++) {
            debug("%d ", e[i][j]);
        }
    }
    current_order = V-1;
    for(i=0;i<V;i++) {
        if (check[i] != VISITED_ONCE) first_dfs(i);
    }
    int num_scc = 0;
    for(i=0;i<V;i++) {
        int u = nth_v[i];
        if (check[u] != VISITED_TWICE) {
            vector<int> scc;
            second_dfs(scc, num_scc, u);
            sccs.push_back(scc);
            num_scc++;
        }
    }
    for(i=0;i<V;i++) {
        debug("%d(%d): %d", i, i/2, scc_label[i]);
    }
    bool possible = true;
    for(i=0;i<N;i++) {
        if (scc_label[NEG(i)] == scc_label[POS(i)]) {
            debug("%d(%d, %d): %d, %d", i, POS(i), NEG(i), scc_label[POS(i)], scc_label[NEG(i)]);
            printf("-1\n");
            possible = false;
            break;
        }
    }
    if (possible) {
        for(i=0;i<num_scc;i++) {
            int k = sccs[i].size();
            for(j=0;j<k;j++) {
                int v = sccs[i][j];
                if (!truth[v]) {
                    truth[v] = -1;
                    truth[COMPV(v)] = 1;
                }
            }
        }
        vector <int> excluded;
        for(i=0;i<N;i++) {
            if (truth[NEG(i)] == 1) {
                excluded.push_back(i+1);
            }
        }
        int en = excluded.size();
        printf("%d\n",en);
        for(i=0;i<en;i++) {
            printf("%d ", excluded[i]);
        }
    }
    return 0;
}