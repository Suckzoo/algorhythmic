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

int n;
llint W;
llint a[20];
llint maxW;

void back(int day, llint num_coin, llint cash) {
    if (day == n) {
        if (cash > maxW) {
            maxW = cash;
        }
        return;
    }
    // 풀매수
    back(day+1, num_coin + (cash / a[day]), cash % a[day]);
    // 풀매도
    back(day+1, 0, cash + num_coin * a[day]);
    // skip
    back(day+1, num_coin, cash);
}

int main() {
    scanf("%d%lld",&n,&W);
    int i;
    for(i=0;i<n;i++) {
        scanf("%lld",&a[i]);
    }
    back(0, 0, W);
    printf("%lld\n", maxW);
    return 0;
}
