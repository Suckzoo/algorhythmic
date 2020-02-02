#include <cstdio>
#include <memory.h>
#define LEFT 0
#define RIGHT 1
#define abs(x) ((x) > 0 ? (x) : -(x))

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", \
__FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

using namespace std;

typedef long long llint;

int T;
int n, s;
llint dy[110][110][2];
llint x[110];
void clear() {
    for(int i=0;i<110;i++) for(int j=0;j<110;j++) for(int k=0;k<2;k++) dy[i][j][k] = 1e18;
    memset(x, 0, sizeof(x));
}
llint dist(int s, int e) {
    return abs(x[s] - x[e]);
}
llint dynamic(int x, int y, int pos) {
    if (y < s || x > s) return 1e18;
    if (x < 0 || y >= n) return 1e18;
    if (x > y) return 1e18;
    if (dy[x][y][pos] < 1e18) return dy[x][y][pos];
    debug("%d %d %d",x,y,pos);
    
    llint k = n - y + x;
    if (pos == LEFT) {
        llint cand1, cand2;
        cand1 = dynamic(x+1, y, LEFT) + k * dist(x+1, x);
        cand2 = dynamic(x+1, y, RIGHT) + k * dist(y, x);
        dy[x][y][pos] = cand1 < cand2 ? cand1 : cand2;
    } else {
        llint cand1, cand2;
        cand1 = dynamic(x, y-1, LEFT) + k * dist(x, y);
        cand2 = dynamic(x, y-1, RIGHT) + k * dist(y-1, y);
        dy[x][y][pos] = cand1 < cand2 ? cand1 : cand2;
    }
    debug("%d %d %d %lld", x, y, pos, dy[x][y][pos]);
    return dy[x][y][pos];
}
int main() {
    scanf("%d",&T);
    while(T--) {
        clear();
        scanf("%d%d",&n,&s);
        s--;
        int i, delta;
        for(i=1;i<n;i++) {
            scanf("%d",&delta);
            x[i] = x[i-1] + delta;
        }
        dy[s][s][LEFT] = 0ll;
        dy[s][s][RIGHT] = 0ll;
        llint l = dynamic(0, n-1, LEFT);
        llint r = dynamic(0, n-1, RIGHT);
        printf("%lld\n",l < r ? l : r);
        debug();
    }
    return 0;
}