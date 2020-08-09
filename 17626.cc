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
int a[225];
int dy[50010];

int main() {
    int i, K;
    memset(dy, -1, sizeof(dy));
    for(i=1;i<=225;i++) {
        a[i-1] = i*i;
    }
    dy[0] = 0;
    for(K=0;K<=50000;K++) {
        for(i=0;i<225;i++) {
            if (K + a[i] > 50000) break;
            if (dy[K+a[i]] == -1 || dy[K+a[i]] > dy[K]+1) dy[K+a[i]] = dy[K]+1;
        }
    }
    int n;
    scanf("%d",&n);
    printf("%d\n",dy[n]);

    return 0;
}
