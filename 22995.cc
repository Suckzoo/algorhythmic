#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <list>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

int T;
int K;
list<int> generateSequence(int n, int nextNum) {
    list<int> seq;
    int lg;
    for(lg=0; n >= (1<<(lg + 1)) - 1;lg++);
    debug("lg: %d, 2^lg-1: %d, n: %d", lg, (1<<lg) - 1, n);
    for(int i=0;i<lg;i++) {
        seq.push_back(nextNum++);
    }
    n -= ((1 << lg) - 1);
    while(n) {
        debug("remaining n: %d", n);
        for(lg=0; n >= (1<<(lg + 1));lg++);
        debug("lg: %d, 2^lg+1: %d, n: %d", lg, (1<<lg), n);
        auto it = seq.begin();
        for(int i = 0; i < lg; i++, it++);
        seq.insert(it, nextNum++);
        n -= (1 << lg);
    }
    return seq;
}
int main() {
    scanf("%d",&T);
    while(T--) {
        scanf("%d",&K);
        auto seq = generateSequence(K, 1);
        printf("%lu\n",seq.size());
        for(int x: seq) {
            printf("%d ", x);
        }
        printf("\n");
        seq.clear();
    }
    return 0;
}
