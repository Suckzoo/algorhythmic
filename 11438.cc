#include <cstdio>
#include <vector>
#include <memory.h>
#include <algorithm>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", \
__FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

int n, m;
int parent[100050][21];
int height[100050];
int check[100050];
vector<int> e[100050];

int find_common_ancestor(int x, int y) {
    int upper = x, lower = y;
    // upper should have lower height (i.e. higher in diagram but actually low height)
    if (height[upper] > height[lower]) swap(upper, lower);
    //printf("Let's find LCA of (%d, %d)!\n", upper, lower);
    //printf("Height of two nodes: %d, %d\n", height[upper], height[lower]);
    // adjust height!
    if (height[upper] < height[lower]) {
        for(int k=20;k>=0;k--) {
            if (height[lower] - height[upper] >= (1<<k)) {
                lower = parent[lower][k];
            }
        }
    }
    debug("%d %d %d %d", lower, height[lower], upper, height[upper]);
    if (upper == lower) return upper;
    for(int k=20;k>=0;k--) {
        debug("%d %d %d %d", lower, height[lower], upper, height[upper]);
        if (parent[upper][k] == -1 || parent[lower][k] == -1) continue;
        if (parent[upper][k] != parent[lower][k]) {
            upper = parent[upper][k];
            lower = parent[lower][k];
            debug("%d %d",upper, lower);
        }
    }
    return parent[upper][0];
}

void dfs(int v) {
    check[v] = 1;
    int i;
    for(int x: e[v]) {
        if (!check[x]) {
            height[x] = height[v] + 1;
            parent[x][0] = v;
            int h = height[x];
            int k = 0, q = v;
            while(parent[q][k] != -1) {
                parent[x][k+1] = parent[q][k];
                q = parent[q][k];
                k++;
            }
            dfs(x);
        }
    }
}

int main() {
    scanf("%d",&n);
    int i;
    int x,y;
    memset(parent, -1, sizeof(parent));
    for(i=0;i<n-1;i++) {
        scanf("%d%d",&x,&y);
        e[x].push_back(y);
        e[y].push_back(x);
    }

    height[1] = 0;
    dfs(1);
    
    scanf("%d",&m);
    for(i=0;i<m;i++) {
        scanf("%d%d",&x,&y);
        int common_ancestor = find_common_ancestor(x,y);
        printf("%d\n",common_ancestor);
    }
    return 0;
}