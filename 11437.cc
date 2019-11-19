#include <cstdio>
#include <vector>
#include <memory.h>
#include <queue>
#include <algorithm>
using namespace std;

int n, m;
int parent[50050][16];
int height[50050];
int check[50050];
vector<int> e[50050];
queue<int> que;

inline int Log(int x) {
    int k = 0;
    while(true) {
        x = x >> 1;
        if (!x) break;
        k++;
    }
    return k;
}

int find_common_ancestor(int x, int y) {
    int upper = x, lower = y;
    // upper should have lower height (i.e. higher in diagram but actually low height)
    if (height[upper] > height[lower]) swap(upper, lower);
    //printf("Let's find LCA of (%d, %d)!\n", upper, lower);
    //printf("Height of two nodes: %d, %d\n", height[upper], height[lower]);
    // adjust height!
    while (height[lower] != height[upper]) {
        int diff = height[lower] - height[upper];
        int portal = Log(diff & (~(diff - 1)));
        lower = parent[lower][portal];
    }
    //printf("Adjusted height! Let's find LCA of (%d, %d)!\n", upper, lower);
    //printf("Height of two nodes: %d, %d\n", height[upper], height[lower]);
    // upper could be ancestor of lower.
    // now both node have same height
    int round = 0;
    while(upper != lower) {
        round++;
        //printf("Round %d: (%d, %d)\n", round, upper, lower);
        for(int k=0;k<16;k++) {
            if (parent[lower][k] == -1) {
                //printf("No common ancestor found until root node. Climbing up...\n");
                // candidate: root~parent[lower][k-1]
                lower = parent[lower][k-1];
                upper = parent[upper][k-1];
                break;
            }
            if (parent[lower][k] == parent[upper][k]) {
                //printf("2^{%d} th ancestor is the first same ancestor! Climbing up to 2^{%d} th ancestor...\n", k, k-1);
                // candidate: parent[lower][k]~parent[lower][k-1]
                if (k == 0) {
                    //printf("Our parent must be LCA!\n");
                    upper = lower = parent[lower][k];
                } else {
                    //printf("Climbing up...\n");
                    lower = parent[lower][k-1];
                    upper = parent[upper][k-1];
                }
                break;
            }
        }
    }
    return upper;
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
    que.push(1);
    check[1] = 1;
    int head;
    while(!que.empty()) {
        head = que.front();
        que.pop();
        for(int tail: e[head]) {
            if (check[tail]) continue;
            check[tail] = 1;
            height[tail] = height[head] + 1;
            parent[tail][0] = head;
            int upper = head;
            int k = 1;
            while(height[tail] >= (1<<k)) {
                parent[tail][k] = parent[upper][k-1];
                upper = parent[upper][k-1];
                k++;
            }
            que.push(tail);
        }
    }
    scanf("%d",&m);
    for(i=0;i<m;i++) {
        scanf("%d%d",&x,&y);
        int common_ancestor = find_common_ancestor(x,y);
        printf("%d\n",common_ancestor);
    }
    return 0;
}