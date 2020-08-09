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

const llint THAT_NUMBER = 16769023;

llint exp2(int n) {
    if (n==0) return 1;
    llint tmp = exp2(n/2);
    tmp = tmp * tmp % THAT_NUMBER;
    if (n%2) {
        tmp = 2 * tmp % THAT_NUMBER;
    }
    return tmp;
}

int main() {
    int k;
    scanf("%d",&k);
    printf("%lld\n", exp2((k+1)/2));
    return 0;
}
