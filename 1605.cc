#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_map>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;
const llint MOD = 1e9 + 7;

int L;
char a[200020];

int sliding_hash(int k) {
    unordered_map<llint, vector<int>> hashmap;
    int i;
    llint hashval = 0;
    debug("k is %d", k);
    for(i=0;i<k;i++) {
        hashval *= 2;
        hashval += a[i];
        hashval = hashval % MOD;
    }
    debug("initial hash value is %lld", hashval);
    vector<int> v;
    v.push_back(0);
    hashmap[hashval] = v;
    llint premodulo = 1;
    for(i=0;i<k;i++) {
        premodulo *= 2;
        premodulo = premodulo % MOD;
    }
    for(i=k;i<L;i++) {
        hashval *= 2;
        hashval = (hashval - (premodulo * a[i-k]) + MOD) % MOD;
        hashval += a[i];
        hashval = hashval % MOD;
        debug("next hash value is %lld", hashval);

        if (hashmap.find(hashval) == hashmap.end()) {
            vector<int> v;
            v.push_back(i - k + 1);
            hashmap[hashval] = v;
        } else {
            debug("Collision detected.");
            for (int x: hashmap[hashval]) {
                debug("Candidates: (%d, %d)", x, i - k + 1);
                int j;
                for (j = 0; j < k; j++) {
                    if (a[x + j] != a[i - k + 1 + j]) break;
                }
                if (j == k) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int parametric_search() {
    int s = 1, e = L-1;
    int ans = 0;
    while (s <= e) {
        int p = s + (e - s) / 2;
        if (sliding_hash(p)) {
            ans = p;
            s = p + 1;
        } else {
            e = p - 1;
        }
    }
    return ans;
}


int main() {
    scanf("%d",&L);
    scanf("%s",a);
    int ans = parametric_search();
    printf("%d\n", ans);
    return 0;
}
