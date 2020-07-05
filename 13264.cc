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

char s[400010];
int n;

struct Suffix{
    int i;
    int f;
    int s;
};
class SuffixArray {
private:
    string s;
    int depth;
    vector<int> sa[20];
    vector<int> ranksa[20];

    void radixbuildLevelZero(vector<int> &targetArray) {
        vector<int> radixBucket[128];
        for(auto target: targetArray) {
            radixBucket[(int)charAt(target)].push_back(target);
        }
        int k = 0;
        int rank = 0;
        for(int i=0;i<128;i++) {
            for(auto t: radixBucket[i]) {
                sa[0][k++] = t;
                ranksa[0][t] = rank;
            }
            if (radixBucket[i].size()) rank++;
            radixBucket[i].clear();
        }
    }

    void radixbuild(int level, vector<int> &targetArray) {
        int n = this->s.size();
        vector<vector<Suffix>> a(2*n);
        vector<vector<Suffix>> b(2*n);
        debug("Pre AB Sorted");
        for(auto target: targetArray) {
            int half = 1 << (level-1);
            Suffix x;
            x.i = target;
            x.f = ranksa[level-1][target];
            x.s = ranksa[level-1][(target + half) % n];
            debug("%d %d %d", x.i, x.f, x.s);
            b[x.s].push_back(x);
        }
        debug("B Sorted");
        for(auto &bin: b) {
            for(auto x: bin) {
                a[x.f].push_back(x);
            }
        }
        debug("A Sorted");
        int rank = 0;
        Suffix prevx;
        for(auto &bin: a) {
            for(auto x: bin) {
                if (prevx.f != x.f || prevx.s != x.s) {
                    rank++;
                    prevx = x;
                }
                ranksa[level][x.i] = rank;
                sa[level].push_back(x.i);
            }
        }
    }

public:
    SuffixArray(char *str) {
        s = string(str);
        s.push_back('$');
        int n = s.size();
        int k = 1;
        for(depth = 0; k < n; depth++, k*=2);
        debug("depth is %d", depth);
        for(int i=0;i<=depth;i++) {
            ranksa[i].resize(n);
        }
        for(int j=0;j<n;j++) {
            sa[0].push_back(j);
        }
        debug("heyhey");
        radixbuildLevelZero(sa[0]);
        debugSuffixArray(0);
        for(int i=1;i<=depth;i++) {
            debug("heyhey");
            radixbuild(i, sa[i-1]);
            // debugSuffixArray(i);
            debug("heyhey");
        }
    }

    char charAt(int x) {
        return s[x % s.size()];
    }

    void debugSuffixArray(int d) {
        debug("Level %d suffix array is", d);
        int i;
        int n = s.size();
        for(i=0;i<n;i++) {
            debug("%d", sa[d][i]);
        }
    }

    void printSuffixArray() {
        int i;
        int n = s.size();
        for(i=1;i<n;i++) {
            printf("%d ", sa[depth][i]);
        }
    }
};

int main() {
    scanf("%s",s);
    auto sa = new SuffixArray(s);
    sa->printSuffixArray();
    return 0;
}
