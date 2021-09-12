#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <memory.h>
#include <queue>
#include <set>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;
int n,m;
int a[110][110];
int meshInfo[110][110];
int meshes=1;
int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
vector<int> e[10010];
set<int> s[10010];
queue<pii> que;
queue<int> ique;
int dist[10010];
int minRadius=9999999;


void bfsForMesh(int x, int y) {
    que.push({x, y});
    meshInfo[x][y] = meshes;
    while(!que.empty()) {
        auto c = que.front();
        que.pop();
        for(int i=0;i<4;i++) {
            int X = c.first + dir[i][0];
            int Y = c.second + dir[i][1];
            if (a[X][Y] != -1 && meshInfo[X][Y] == 0 && a[X][Y] == a[x][y]) {
                meshInfo[X][Y] = meshes;
                que.push({X, Y});
            }
        }
    }
}
void bfsForRadii(int i) {
    dist[i] = 0;
    ique.push(i);
    int maxDist = 0;
    while(!ique.empty()) {
        int c = ique.front();
        ique.pop();
        for(int x: e[c]) {
            if (dist[x] == -1) {
                dist[x] = dist[c] + 1;
                if (maxDist < dist[x]) maxDist = dist[x];
                ique.push(x);
            }
        }
    }
    if (minRadius > maxDist) minRadius = maxDist;
}
int main() {
    memset(a, -1, sizeof(a));
    scanf("%d%d",&n,&m);
    int i,j;
    for(i=1;i<=n;i++) {
        for(j=1;j<=m;j++) {
            scanf("%d",&a[i][j]);
        }
    }
    for(i=1;i<=n;i++) {
        for(j=1;j<=m;j++) {
            if (!meshInfo[i][j]) {
                bfsForMesh(i,j);
                meshes++;
            }
        }
    }
    for(i=1;i<=n;i++) {
        for(j=1;j<=m;j++) {
            for(int k=0;k<4;k++) {
                int X = i + dir[k][0];
                int Y = j + dir[k][1];
                if (a[X][Y] != -1 && meshInfo[X][Y] != meshInfo[i][j]) {
                    s[meshInfo[i][j]].insert(meshInfo[X][Y]);
                }
            }
        }
    }
    for(i=1;i<meshes;i++) {
        for(auto x: s[i]) {
            e[i].push_back(x);
            debug("%d %d", i, x);
        }
    }
    for(i=1;i<meshes;i++) {
        memset(dist, -1, sizeof(dist));
        bfsForRadii(i);
    }
    printf("%d\n", minRadius);
    return 0;
}
