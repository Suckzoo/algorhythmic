#include <cstdio>
#include <cstring>
#include <string>
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
int TCoeff[61];
int W[61][61];
int a[61];
int t[61];
int check[61];
int ans=99999999;
void back(int leftDryer, int tempIndex, int cost) {
    debug("%d %d %d", leftDryer, tempIndex, cost);
    if (leftDryer == 1) {
        int i;
        int maxCost = 0;
        for(i=0;i<n;i++) {
            if (!check[i]) {
                if (maxCost < W[tempIndex][i])
                    maxCost = W[tempIndex][i];
            }
        }
        if (cost + maxCost < ans) {
            ans = cost + maxCost;
        }
        return;
    }
    int i,j;
    for(i=tempIndex;i<n;i++) {
        //3600^2 나와도 괜찮을듯!!!
        if (check[i]) continue;
        int Q = W[tempIndex][i];
        int mini = -1;
        for(j=tempIndex;j<n;j++) {
            if (!check[j] && W[tempIndex][j] <= Q) {
                check[j] = leftDryer;
            } else if (!check[j] && mini == -1) {
                mini = j;
            }
        }
        if (mini == -1) {
            back(1, 0, cost + Q);
        } else {
            back(leftDryer - 1, mini, cost + Q);
        }
        for(j=tempIndex;j<n;j++) {
            if (check[j] == leftDryer) {
                check[j] = 0;
            }
        }
    }
}
int main() {
    scanf("%d%d",&n,&K);
    int i,j;
    int x,y;
    int minT = 999;
    for(i=0;i<n;i++) {
        scanf("%d%d",&x,&y);
        if (minT > x) minT = x;
        if(TCoeff[x - 40] < y) TCoeff[x-40] = y;
    }
    int n = 0;
    for(i=0;i<61;i++) {
        if (TCoeff[i]) {
            a[n] = TCoeff[i];
            t[n] = i + 40;
            n++;
        }
    }
    debug("%d", n);
    for(i=0;i<n;i++) {
        for(j=i;j<n;j++) {
            W[i][j] = 30 + a[j] * (t[j] - t[i]);
            // fprintf(stderr, "%d ", W[i][j]);
        }
        // fprintf(stderr, "\n");
    }
    back(K, 0, 0);
    printf("%d\n",ans);
    return 0;
}
