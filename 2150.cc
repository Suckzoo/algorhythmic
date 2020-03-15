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
int n,m;
vector <int> e[10010];
vector <int> e_rev[10010];
vector <vector <int>> scc;

int check[10010];
int nth_vertex[10010];
int current_dfn;

void first_dfs(int v) {
    check[v] = 1;
    int i;
    int k = e[v].size();
    for(i=0;i<k;i++) {
        int u = e[v][i];
        if (!check[u]) first_dfs(u);
    }
    nth_vertex[current_dfn--] = v;
}

void second_dfs(vector <int> &scc_vector, int v) {
    check[v] = 2;
    scc_vector.push_back(v);
    int i;
    int k = e_rev[v].size();
    debug("%d rev_k was %d", v, k);
    for(i=0;i<k;i++) {
        int u = e_rev[v][i];
        if (check[u] == 1) second_dfs(scc_vector, u);
    }
}

bool scc_comp(const vector<int> &vx, const vector<int> &vy) {
    return vx[0] < vy[0];
}

int main() {
    scanf("%d%d",&n,&m);
    current_dfn = n-1;
    int i, x, y;
    for(i=0;i<m;i++) {
        scanf("%d%d",&x,&y);
        e[x].push_back(y);
        e_rev[y].push_back(x);
    }
    for(i=1;i<=n;i++) {
        if (!check[i]) first_dfs(i);
    }
    for(i=0;i<n;i++) {
        int v = nth_vertex[i];
        debug("%dth vertex was: %d", i, v);
        if (check[v] == 1) {
            vector <int> scc_vector;
            second_dfs(scc_vector, v);
            sort(scc_vector.begin(), scc_vector.end());
            scc.push_back(scc_vector);
        }
    }
    int scc_n = scc.size();
    printf("%d\n", scc_n);
    sort(scc.begin(), scc.end(), scc_comp);
    for(i=0;i<scc_n;i++) {
        int comp_n = scc[i].size();
        for(int j=0;j<comp_n;j++) {
            printf("%d ", scc[i][j]);
        }
        printf("-1\n");
    }
    return 0;
}
