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

char s[100010];
int n;

class SuffixArray {
private:
    string s;
    int depth;
    vector<int> sa[20];
    vector<int> ranksa[20];

    void buildForZeroLevel() {
        sort(sa[0].begin(), sa[0].end(), [this](int x, int y) {
            return this->charAt(x) < this->charAt(y);
        });
    }

    void build(int k) {
        sort(sa[k].begin(), sa[k].end(), [this, ranksa = this->ranksa, k, n = this->s.size()](int x, int y) {
            int half = 1 << (k-1);
            debug("Challenge at level %d!!!\n\nchallengers are %d and %d", k, x, y);
            debug("Half is %d", half);
            debug("prevsa[x]: %d, prevsa[y]: %d", ranksa[k-1][x], ranksa[k-1][y]);
            debug("prevsa[x+half]: %d, prevsa[y+half]: %d\n\n\n\n", ranksa[k-1][(x+half)%n], ranksa[k-1][(y+half)%n]);
            return ranksa[k-1][x] < ranksa[k-1][y] || (ranksa[k-1][x] == ranksa[k-1][y] && ranksa[k-1][(x + half) % n] < ranksa[k-1][(y + half) % n]);
        });
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
            sa[i].resize(n);
            ranksa[i].resize(n);
            for(int j=0;j<n;j++) {
                sa[i][j] = j;
            }
        }
        buildForZeroLevel();
        int rank = 1;
        ranksa[0][sa[0][0]] = 1;
        for(int i=1;i<n;i++) {
            if (s[sa[0][i-1]] != s[sa[0][i]]) rank++;
            ranksa[0][sa[0][i]] = rank;
            debug("Level 0: %d", sa[0][i]);
            debug("Rank is: %d", ranksa[0][sa[0][i]]);
        }
        for(int i=1;i<=depth;i++) {
            build(i);
            int rank = 1;
            ranksa[0][sa[i][0]] = rank;
            int half = 1 << (i-1);
            for(int j=1;j<n;j++) {
                int x = sa[i][j-1];
                int y = sa[i][j];
                if (ranksa[i-1][x] != ranksa[i-1][y] || ranksa[i-1][(x + half) % n] != ranksa[i-1][(y + half) % n]) rank++;
                ranksa[i][sa[i][j]] = rank;
                debug("Level %d: %d", i, sa[i][j]);
                debug("Rank is: %d", ranksa[i][sa[i][j]]);
            }
        }
    }

    char charAt(int x) {
        return s[x % s.size()];
    }

    void printSuffixArray() {
        int i;
        int n = s.size();
        for(i=1;i<n;i++) {
            printf("%d\n", sa[depth][i]);
        }
    }
};

int main() {
    scanf("%s",s);
    auto sa = new SuffixArray(s);
    sa->printSuffixArray();
    return 0;
}
