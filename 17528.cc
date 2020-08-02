#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <memory.h>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

int n;
int a[300], b[300];
int dy[100000];
int asum, bsum;

int main() {
    int i;
    scanf("%d",&n);
    for(i=0;i<n;i++) {
        scanf("%d%d",&a[i],&b[i]);
        asum += a[i];
        bsum += b[i];
    }
    memset(dy, -1, sizeof(dy));
    dy[0] = bsum;
    for(i=0;i<n;i++) {
        for(int K=asum-a[i];K>=0;K--) {
            if (dy[K] != -1) {
                if (dy[K+a[i]] == -1 || dy[K+a[i]] > dy[K] - b[i]) {
                    dy[K+a[i]] = dy[K] - b[i];
                }
            }
        }
    }
    int minv = asum;
    for(i=0;i<=minv;i++) {
        if (dy[i] != -1) {
            int cand = i > dy[i] ? i : dy[i];
            if (minv > cand) minv = cand;
        }
    }
    printf("%d\n",minv);
    return 0;
}
