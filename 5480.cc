#include <cstdio>
#include <utility>
#include <algorithm>
#include <vector>
#include <memory.h>
#include <cstdlib>
#define m(p) (p).first
#define M(p) (p).second

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", \
__FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

using namespace std;
typedef pair<int, int> pii;

template <typename T>
class segment_tree {
private:
    struct segment_node {
        int s;
        int e;
        int p;
        T v;
        segment_node() {
            s = e = p = 0;
            v = invalid;
        }
        segment_node(int s, int e, T v): s(s), e(e), v(v) {
            p = s + (e - s) / 2;
        }
    };
    segment_node tree[1200000];
    T _init(int idx, int s, int e, T *init_array) {
        if (s != e) {
            int p = s + (e - s) / 2;
            segment_node lnode(s, p, invalid);
            tree[idx * 2 + 1] = lnode;
            T lval = _init(idx * 2 + 1, s, p, init_array);
            segment_node rnode(p+1, e, invalid);
            tree[idx * 2 + 2] = rnode;
            T rval = _init(idx * 2 + 2, p + 1, e, init_array);
            tree[idx].v = min(lval, rval);
        } else {
            if (init_array) {
                tree[idx].v = init_array[s];
            }
        }
        debug("node %d, covers segment (%d, %d), represents value %d.",  idx, s, e, tree[idx].v);
        return tree[idx].v;
    }
    T _find(int idx, int s, int e) {
        auto &node = tree[idx];
        debug("node covers (%d, %d) with represent value %d, querying (%d, %d)...", node.s, node.e, node.v, s, e);
        if (s == node.s && e == node.e) {
            return node.v;
        } else if (e <= node.p) {
            return _find(2 * idx + 1, s, e);
        } else if (node.p < s) {
            return _find(2 * idx + 2, s, e);
        } else {
            T lval = _find(2 * idx + 1, s, node.p);
            T rval = _find(2 * idx + 2, node.p + 1, e);
            return min(lval, rval);
        }
    }
public:
    static const T invalid = -1;
    segment_tree() {}
    void init(int N, T *init_array=nullptr) {
        tree[0] = segment_node(0, N-1, invalid);
        _init(0, 0, N-1, init_array);
    }
    static T min(T x, T y) {
        if (x == invalid) {
            return y;
        }
        if (y == invalid) {
            return x;
        }
        return x < y ? x : y;
    }
    void clear() {
        memset(tree, 0, sizeof(tree));
    }
    T find(int s, int e) {
        T ret = _find(0, s, e);
        debug("find %d, %d: %d", s, e, ret);
        return ret;
    }
};

segment_tree<int> xtree, ytree;

struct ship {
    pii x, y;
    int w;
};
struct laser {
    int x;
    int b;
};
ship a[100010];
laser l[100010];
vector<int> xvec, yvec;
int xinit[300010], yinit[300010];

int n, m;
int max_w[100010];

void init_case() {
    memset(xinit, -1, sizeof(xinit));
    memset(yinit, -1, sizeof(yinit));
    memset(max_w, 0, sizeof(max_w));
    xvec.clear();
    yvec.clear();
    xtree.clear();
    ytree.clear();
}

void compress(vector <int> &v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}

int main() {
    int T;
    int dummy;
    scanf("%d",&T);
    while(T--) {
        init_case();
        scanf("%d%d%d",&dummy,&n,&m);
        int i;
        for(i=0;i<n;i++) {
            scanf("%d%d%d%d%d", &m(a[i].x), &m(a[i].y), &M(a[i].x), &M(a[i].y), &a[i].w);
            if (m(a[i].x) > M(a[i].x)) swap(m(a[i].x), M(a[i].x));
            if (m(a[i].y) > M(a[i].y)) swap(m(a[i].y), M(a[i].y));
            xvec.push_back(m(a[i].x));
            xvec.push_back(M(a[i].x));
            yvec.push_back(m(a[i].y));
            yvec.push_back(M(a[i].y));
        }
        for(i=0;i<m;i++) {
            scanf("%d%d", &l[i].x, &l[i].b);
            if (l[i].b) {
                xvec.push_back(l[i].x);
            } else {
                yvec.push_back(l[i].x);
            }
        }
        compress(xvec);
        compress(yvec);

        for (auto &ship: a) {
            m(ship.x) = lower_bound(xvec.begin(), xvec.end(), m(ship.x)) - xvec.begin();
            M(ship.x) = lower_bound(xvec.begin(), xvec.end(), M(ship.x)) - xvec.begin();
            m(ship.y) = lower_bound(yvec.begin(), yvec.end(), m(ship.y)) - yvec.begin();
            M(ship.y) = lower_bound(yvec.begin(), yvec.end(), M(ship.y)) - yvec.begin();
        }

        for (i = 0; i < m; i++) {
            if (l[i].b) {
                debug("compressing x... originally, %d", l[i].x);
                l[i].x = lower_bound(xvec.begin(), xvec.end(), l[i].x) - xvec.begin();
                xinit[l[i].x] = segment_tree<int>::min(xinit[l[i].x], i);
                debug("compressed as %d, xinit is %d", l[i].x, xinit[l[i].x]);
            } else {
                debug("compressing y... originally, %d", l[i].x);
                l[i].x = lower_bound(yvec.begin(), yvec.end(), l[i].x) - yvec.begin();
                yinit[l[i].x] = segment_tree<int>::min(yinit[l[i].x], i);
                debug("compressed as %d, yinit is %d", l[i].x, yinit[l[i].x]);
            }
        }

        xtree.init(xvec.size(), xinit);
        ytree.init(yvec.size(), yinit);

        for(i=0;i<n;i++) {
            int lx = xtree.find(m(a[i].x), M(a[i].x));
            int ly = ytree.find(m(a[i].y), M(a[i].y));
            int minl = segment_tree<int>::min(lx, ly);
            if (minl != segment_tree<int>::invalid) {
                if (max_w[minl] < a[i].w) max_w[minl] = a[i].w;
            }
        }
        for(i=0;i<m;i++) {
            printf("%d\n", max_w[i]);
        }
    }
    return 0;
}