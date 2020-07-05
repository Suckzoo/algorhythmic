#include <cstdio>
#include <cstring>
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

int n;
int lr[50], ll[50];
int board[100][100];
int m;

void dfs(int x, int y, int cnt) {
    if (x == n) {
        if (cnt > m) m = cnt;
        return;
    }
    if (y == n) {
        dfs(x+1, 0, cnt);
        return;
    }
    if (!lr[x+y] && !ll[n+x-y]) {
        lr[x+y] = ll[n+x-y] = 1;
        board[x][y] = 1;
        dfs(x, y+1, cnt+1);
        board[x][y] = 0;
        lr[x+y] = ll[n+x-y] = 0;
    }
    dfs(x, y+1, cnt);
}
char num[100];
int inum[100];

int main() {
    // for(n=1;n<=8;n++) {
    //     dfs(0,0,0);
    //     printf("n: %d / max: %d\n", n, m);
    //     m = 0;
    // }
    scanf("%s", num);
    if (!strcmp(num, "1")) {
        printf("1\n");
        return 0;
    }
    int l = strlen(num);
    int i;
    for(i = 0; i < l; i++) {
        inum[i] = num[l-i-1] - '0';
    }
    int neg_i = 0;
    while(--inum[neg_i] < 0) {
        inum[neg_i] += 10;
        neg_i++;
    }
    for(i=0;i<l;i++) {
        inum[i] *= 2;
    }
    for(i=0;i<l;i++) {
        if (inum[i] > 9) {
            inum[i+1] += (inum[i] / 10);
            inum[i] = inum[i] % 10;
        }
    }
    int begin = 0;
    for(i=99;i>=0;i--) {
        if (inum[i]) begin = 1;
        if (begin) printf("%d",inum[i]);
    }
    printf("\n");
    return 0;
}
