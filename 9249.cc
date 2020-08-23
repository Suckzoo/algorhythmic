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

char s[200010],t[200020],q[400040];
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
    int ns;
    vector<int> sa[20];
    vector<int> finalsa;
    vector<int> rev_pos;
    vector<int> lcp;
    vector<int> skip;
    vector<int> ranksa[20];
    vector<vector<Suffix>> a;
    vector<vector<Suffix>> b;

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
            bin.clear();
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
            bin.clear();
        }
    }

public:
    SuffixArray(char *str, int ns) {
        this->ns = ns;
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
        a.resize(2*n);
        b.resize(2*n);
        for(int i=1;i<=depth;i++) {
            debug("heyhey");
            radixbuild(i, sa[i-1]);
            // debugSuffixArray(i);
            debug("heyhey");
        }
        rev_pos.resize(n+1);
        lcp.resize(n+1);
        skip.resize(n+1);
        for(int i = 0; i < n; i++) {
            finalsa.push_back(sa[depth][i]);
            rev_pos[sa[depth][i]] = i;
        }
        buildLCPArray();
    }

    void buildLCPArray() {
        const int n = s.size();
        for(int i=0;i<n;i++) {
            int pos = rev_pos[i];
            int skip_val = skip[pos];
            debug("pos: %d", pos);
            int k = finalsa[pos];
            for (int j = skip_val; j < n; j++) {
                if (charAt(finalsa[pos-1] + j) != charAt(finalsa[pos] + j)) {
                    lcp[pos] = j; // common prefix between finalsa[pos-1] and finalsa[pos]
                    break;
                }
            }
            if (lcp[pos] > 1) {
                int skip_start = rev_pos[finalsa[pos-1] + 1];
                int skip_end = rev_pos[finalsa[pos] + 1];
                if (skip_start > skip_end) swap(skip_start, skip_end);
                for(int j = skip_start + 1 ; j <= skip_end; j++) {
                    if (skip[j] < lcp[pos] - 1) skip[j] = lcp[pos] - 1;
                }
            }
        }
        for(int i=1;i<n;i++) {
            debug("%d", lcp[i]);
        }
    }

    string maximalCommonPrefix() {
        const int n = s.size();
        int max = 0;
        int ptr = 0;
        for(int i=1;i<n;i++) {
            if ((finalsa[i] <= ns && finalsa[i-1] <= ns)
            ||(finalsa[i] > ns && finalsa[i-1] > ns)) continue;
            if (max < lcp[i]) {
                ptr = i;
                max = lcp[i];
            }
        }
        string res;
        int start_point = finalsa[ptr];
        for(int i=0;i<max;i++) {
            res.push_back(charAt(start_point + i));
        }
        return res;
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
    scanf("%s",t);
    sprintf(q, "%s+%s",s,t);
    debug("%s",q);
    auto sa = new SuffixArray(q, strlen(s));
    string res = sa->maximalCommonPrefix();
    printf("%lu\n%s\n", res.size(), res.c_str());
    return 0;
}