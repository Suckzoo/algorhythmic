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

typedef pair<int, int> pii;
typedef long long llint;
typedef int Clause;
typedef int Vertex;

int n, m;
vector<int> e[2020], rev[2020];

inline Vertex toVertex(Clause c) {
    if (c < 0) return n - c;
    return c;
}
inline Clause toClause(Vertex v) {
    if (v > n) return n - v;
    return v;
}
inline int fromClauseToIndex(Clause c) {
    return c > 0 ? c : -c;
}
inline int fromVertexToIndex(Vertex v) {
    return fromClauseToIndex(toClause(v));
}
inline Clause NEG(Clause clause) {
    return -clause;
}

void connect(Clause c1, Clause c2) {
    Vertex v1 = toVertex(c1), v2 = toVertex(c2);
    debug("connect %d -> %d", c1, c2);
    e[v1].push_back(v2);
    rev[v2].push_back(v1);
}

int input() {
    cin >> n >> m;
    if (cin.eof()) return -1;
    // v1 === v1 || v1 === -v1 -> v1
    connect(NEG(1), 1);
    for(int i=0;i<m;i++) {
        int v1, v2;
        cin >> v1 >> v2;
        // p: (v1, v2) === !(!v1 && !v2) === (v1 || v2) === (!v1 -> v2) === (!v2 -> v1)
        connect(NEG(v1), v2);
        connect(NEG(v2), v1);
    }
    return 0;
}

int check[2020], dfn[2020], currentDFN;
Vertex vertexByDFN[2020];
int SCCMembership[2020];

void DFS(Vertex v) {
    check[v] = 1;
    for(auto &i: e[v]) {
        if (!check[i]) DFS(i);
    }
    dfn[v] = currentDFN;
    vertexByDFN[currentDFN] = v;
    currentDFN--;
}

void tagDFN() {
    Vertex i;
    currentDFN = 2 * n - 1;
    for(i=1;i<=2*n;i++) {
        if (!check[i]) DFS(i);
    }
#ifdef DEBUG
    for(i=0;i<2*n;i++) {
        debug("vertexByDFN[%d] = %d", i, toClause(vertexByDFN[i]));
    }
#endif
}

void revDFS(Vertex v, int SCCNum) {
    debug("revDFS(%d, %d)", toClause(v), SCCNum);
    SCCMembership[v] = SCCNum;
    check[v] = 2;
    for(auto &u: rev[v]) {
        if (check[u] != 2) {
            revDFS(u, SCCNum);
        }
    }
}

bool getSCC() {
    tagDFN();
    int SCCNum = 1;
    for(int i=0;i<2*n;i++) {
        Vertex v = vertexByDFN[i];
        if (check[v] == 2) continue;
        revDFS(v, SCCNum++);
    }
    for(Clause i=1;i<=n;i++) {
        Vertex v = toVertex(i);
        Vertex negV = toVertex(NEG(i));
        if (SCCMembership[v] == SCCMembership[negV]) {
            return false;
        }
    }
    return true;
}

void clear() {
    for(auto &v: e) v.clear();
    for(auto &v: rev) v.clear();
    fill(begin(check), end(check), 0);
    fill(begin(dfn), end(dfn), -1);
    fill(begin(vertexByDFN), end(vertexByDFN), 0);
    fill(begin(SCCMembership), end(SCCMembership), 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    while(!input()) {
        if (getSCC()) cout << "yes" << "\n";
        else cout << "no" << "\n";
        clear();
    }
    return 0;
}
