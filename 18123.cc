#include <cstdio>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <utility>
#include <memory.h>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef pair<int, unsigned long long> pulli;
typedef long long llint;
typedef unsigned long long ullint;

int N;
struct Tree {
    list<int> e[50];
    int nodes;
    int check[50];
    int root;
    int diameter;
    ullint tree_hash;
    void build_from_input() {
        scanf("%d", &nodes);
        if (nodes == 1) {
            diameter = 0;
            root = 0;
            return;
        }
        int i;
        int x, y;
        for(i=0;i<nodes-1;i++) {
            scanf("%d%d",&x,&y);
            e[x].push_back(y);
            e[y].push_back(x);
        }
        memset(check, 0, sizeof(check));
        auto [_, p] = furthest(0, 0, 1);
        auto [dia, q] = furthest(0, p, 2);
        diameter = dia;
        auto [center, __] = centroid(p, q, 0);
        root = center;
    }
    void invalidate(int x, int y) {
        for(auto it = e[x].begin(); it != e[x].end(); it++) {
            if (*it == y) {
                e[x].erase(it);
                break;
            }
        }
    }
    void invalidate_centroids(int x, int y) {
        invalidate(x, y);
        invalidate(y, x);
    }
    pii centroid(int r1, int r2, int dep) {
        if (r1 == r2) {
            return make_pair(-1, 1);
        }
        check[r1] = 3;
        for (auto x: e[r1]) {
            if (check[x] != 3) {
                auto [c, reached] = centroid(x, r2, dep + 1);
                if (reached) {
                    if (dep == diameter / 2) {
                        if (diameter % 2) {
                            int k = nodes++;
                            e[x].push_back(k);
                            e[k].push_back(x);
                            e[r1].push_back(k);
                            e[k].push_back(r1);
                            invalidate_centroids(x, r1);
                            return make_pair(k, 1);
                        } else {
                            return make_pair(r1, 1);
                        }
                    } else {
                        return make_pair(c, 1);
                    }
                }
            }
        }
        return make_pair(-1, 0);
    }
    pii furthest(int dep, int v, int marker) {
        check[v] = marker;
        int max_dep = dep, max_v = v;
        for(auto x: e[v]) {
            if (check[x] != marker) {
                auto [depth, vertex] = furthest(dep+1, x, marker);
                if (depth > max_dep) {
                    max_dep = depth;
                    max_v = vertex;
                }
            }
        }
        return make_pair(max_dep, max_v);
    }
    void clear_check() {
        memset(check, 0, sizeof(check));
    }
    ullint interpret(int v) {
        check[v] = 1;
        vector<ullint> tree_hash_vals;
        for(auto x: e[v]) {
            if (!check[x]) {
                ullint hash_val = interpret(x);
                tree_hash_vals.push_back(hash_val);
            }
        }
        sort(tree_hash_vals.begin(), tree_hash_vals.end());
        ullint hash_val = 1;
        for (auto hv: tree_hash_vals) {
            int length = 0;
            ullint hvac = hv;
            for(; hvac != 0; hvac >>= 1, length++);
            hash_val <<= length;
            hash_val |= hv;
        }
        hash_val <<= 1;
        return hash_val;
    }
    pulli perform_interpretation() {
        memset(check, 0, sizeof(check));
        tree_hash = interpret(root);
        return make_pair(diameter, tree_hash);
    }
};
set<pulli> se;

int main() {
    scanf("%d",&N);
    int i;
    for(i=0;i<N;i++) {
        Tree a;
        a.build_from_input();
        se.insert(a.perform_interpretation());
        debug("%d, %lld", a.diameter, a.tree_hash);
    }
    printf("%lu\n",se.size());
    return 0;
}
