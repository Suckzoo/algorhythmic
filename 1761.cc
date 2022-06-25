#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define V(p) (p).first
#define W(p) (p).second

typedef pair<int, int> pii;
typedef long long llint;
int n, lgn;
vector<pii> e[40040];
int dist[40040];
pii parent[40040][17];

void dumpNode(int u) {
    debug("u: %d, h: %d", u, dist[u]);
}
int lg2(int x) {
    int p = 0;
    for(; x > (1<<p); p++);
    return p;
}
void input() {
    cin >> n;
    lgn = lg2(n);
    for(int i=0;i<n-1;i++) {
        int u, v, w;
        cin >> u >> v >> w;
        e[u].push_back({v, w});
        e[v].push_back({u, w});
    }
}
void bfs() {
    fill(begin(dist), end(dist), -1);
    queue<int> que;
    que.push(1);
    dist[1] = 0;
    while(!que.empty()) {
        int u = que.front();
        dumpNode(u);
        que.pop();
        for(auto &p: e[u]) {
            if(dist[V(p)] == -1) {
                dist[V(p)] = dist[u] + 1;
                V(parent[V(p)][0]) = u;
                W(parent[V(p)][0]) = W(p);
                que.push(V(p));
            }
        }
    }
}
void constructHierarchy() {
    for(int j=1;j<17;j++) {
        for(int i=1;i<=n;i++) {
            int p = V(parent[i][j-1]);
            debug("(%d, %d), p and pp: %d, %d", i, j, p, V(parent[p][j-1]));
            if (!V(parent[p][j-1])) continue;
            V(parent[i][j]) = V(parent[p][j-1]);
            W(parent[i][j]) = W(parent[p][j-1]) + W(parent[i][j-1]);
        }
    }
#ifdef DEBUG
    for(int i=1;i<=n;i++) {
        for(int j=0;j<17;j++) {
            debug("parent[%d][%d]: (%d, %d)", i, j, V(parent[i][j]), W(parent[i][j]));
        }
    }
#endif
}
pii climb(int u, int h) {
    int k = 0;
    int weightSum = 0;
    while(h) {
        int base = 1<<k;
        if (h & base) {
            weightSum += W(parent[u][k]);
            u = V(parent[u][k]);
            h -= base;
        }
        k++;
    }
    return {u, weightSum};
}
void query() {
    int m;
    cin >> m;
    while(m--) {
        int p, q;
        cin >> p >> q;
        debug("Start query (%d, %d)", p, q);
        int weightSum = 0;
        if (dist[p] > dist[q]) swap(p, q);
        int hdiff = dist[q] - dist[p];
        // 1. adjust height
        tie(q, weightSum) = climb(q, hdiff);
        debug("1. Adjusting heights");
        dumpNode(p);
        dumpNode(q);
        debug("weight: %d", weightSum);
        // 2. climb until common ancestor
        debug("2. Climb");
        for(int k = lgn; p != q && k >= 0; k--) {
            if (!V(parent[p][k])) continue;
            if (V(parent[p][k]) == V(parent[q][k])) continue;
            int w1, w2;
            tie(p, w1) = parent[p][k];
            tie(q, w2) = parent[q][k];
            weightSum += (w1 + w2);
            debug("Still climbing...");
            dumpNode(p);
            dumpNode(q);
            debug("weight: %d", weightSum);
        }
        // 3. get common ancestor for real
        debug("3. Final adjustment");
        if (p != q) {
            int w1, w2;
            tie(p, w1) = parent[p][0];
            tie(q, w2) = parent[q][0];
            weightSum += (w1 + w2);
            dumpNode(p);
            dumpNode(q);
            debug("weight: %d", weightSum);
        }
        debug("4. Common ancestor reached");
        cout << weightSum << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    input();
    bfs();
    constructHierarchy();
    query();
    return 0;
}
