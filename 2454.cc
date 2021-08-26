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

typedef pair<int, int> pii;
typedef long long llint;

int n, K;
vector<int> e[300030];
int dy[300030];
int L[300030];
enum VertexStatus {
    NEVER_VISITED,
    ON_EVALUATING,
    EVALUATED,
};
VertexStatus state[300030];
const int CLOSED = -1;

void dfs(int v) {
    state[v] = ON_EVALUATING;
    int frags = 0;
    int minBranch1 = -1, minBranch2 = -1;
    for(int x: e[v]) {
        if (state[x] == NEVER_VISITED) {
            dfs(x);
            frags += dy[x];
            debug("x: %d, L(x): %d", x, L[x]);
            debug("mb1: %d, mb2: %d", minBranch1, minBranch2);
            if (minBranch1 == -1 && L[x] != CLOSED) {
                minBranch1 = x;
            } else if (L[minBranch1] > L[x] && L[x] != CLOSED) {
                minBranch2 = minBranch1;
                minBranch1 = x;
            } else if (minBranch2 == -1 && L[x] != CLOSED) {
                minBranch2 = x;
            } else if (L[minBranch2] > L[x] && L[x] != CLOSED) {
                minBranch2 = x;
            }
        }
    }
    debug("v: %d, minB1, minB2: (%d, %d)", v, minBranch1, minBranch2);
    if (minBranch1 == -1 && minBranch2 == -1) {
        dy[v] = frags + 1;
        L[v] = 0;
    } else if (minBranch2 == -1) {
        dy[v] = frags;
        if (L[minBranch1] + 1 <= K) {
            L[v] = L[minBranch1] + 1;
        } else {
            dy[v] = frags + 1;
            L[v] = 0;
        }
    } else {
        dy[v] = frags;
        if (L[minBranch1] + L[minBranch2] + 2 <= K) {
            dy[v] = frags - 1;
            L[v] = -1;
        } else if (L[minBranch1] + 1 <= K) {
            L[v] = L[minBranch1] + 1;
        } else {
            dy[v] = frags + 1;
            L[v] = 0;
        }
    }
    state[v] = EVALUATED;
}

int main() {
    scanf("%d%d",&n,&K);
    int x, y;
    for(int i=0;i<n-1;i++) {
        scanf("%d%d",&x,&y);
        e[x].push_back(y);
        e[y].push_back(x);
    }
    dfs(1);
    printf("%d\n",dy[1]);
    for(int i=1;i<=n;i++) {
        debug("dy[%d]: %d", i, dy[i]);
    }
    return 0;
}
