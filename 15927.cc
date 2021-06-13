#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

char s[500050];
int main() {
    scanf("%s",s);
    int i;
    int n = strlen(s);
    for(i=1;i<n;i++) {
        if (s[i] != s[0]) break;
    }
    if (i >= n) {
        printf("-1\n");
        return 0;
    }
    for(i=0;i<n/2;i++) {
        if (s[i] != s[n - i - 1]) {
            printf("%d\n", n);
            return 0;
        }
    }
    printf("%d\n", n-1);
    return 0;
}
