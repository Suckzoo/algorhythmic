#include <cstdio>

typedef unsigned long long uint64t;

int n, K;
int a[160];
int s[160];
int S[160][160];

class bitset {
    uint64t x[7900];

public:
    uint64t get_segment(int s) { // get bit segments starting from s
        uint64t s_radix = s / 64;
        uint64t s_denom = s % 64;
        if (!s_denom) {
            return x[s_radix];
        }
        uint64t jump = 64 - s_denom;
        uint64t segment = 0;
        uint64t first = x[s_radix] >> s_denom;
        uint64t second = x[s_radix+1] & ((1ull << s_denom) - 1ull);
        return (second << jump) + first; 
    }

    void dump_one_segment(uint64t segment) {
        for(int j = 0; j < 64; j++) {
            printf("%llu", segment & 1);
            segment >>= 1;
        }
    }

    void update_segment(int s, uint64t segment) {
        uint64t s_radix = s / 64;
        uint64t s_denom = s % 64;
        if (!s_denom) {
            x[s_radix] = x[s_radix] | segment;
            return;
        }
        uint64t jump = 64 - s_denom;
        uint64t first = segment & ((1ull << jump) - 1ull);
        x[s_radix] = x[s_radix] | (first << s_denom);
        uint64t second = segment - first;
        x[s_radix + 1] = x[s_radix + 1] | (second >> jump);
    }

    bool get_bit(int s) {
        uint64t s_radix = s / 64;
        uint64t s_denom = s % 64;
        return !!(x[s_radix] & (1ull << s_denom));
    }

    void set_bit(int s, bool bit) {
        uint64t s_radix = s / 64;
        uint64t s_denom = s % 64;
        uint64t flag = !!bit;
        if (flag) {
            x[s_radix] = x[s_radix] | (flag << s_denom);
        } else {
            x[s_radix] = x[s_radix] & (flag << s_denom);
        }
    }

    void dump(int until) {
        int limit = (until / 64);
        for(int i = 0; i <= limit; i++) {
            dump_one_segment(x[i]);
            if (i != limit) printf(" | ");
        }
        printf("\n");
    }
    
    int get_first_zero_greater_than(int s) {
        int s_radix = s / 64;
        int s_denom = s % 64;
        int index = (s / 64) * 64;
        uint64t flag = x[s_radix]; 
        flag |= ((1ull << s_denom) - 1);
        while(flag == (uint64t)-1) {
            flag = x[++s_radix];
            index += 64;
        }
        while(flag % 2) {
            flag /= 2;
            index++;
        }
        return index;
    }

};
bitset dy[160];

int main() {
    scanf("%d", &n);
    int i;
    for(i=1;i<=n;i++) {
        scanf("%d",&a[i]);
        s[i] = s[i-1] + a[i];
    }
    for(i=1;i<=n;i++) {
        S[i][i] = a[i];
        for(int j=i+1;j<=n;j++) {
            S[i][j] = S[i][j-1] + s[j] - s[i-1];
        }
    }
    scanf("%d",&K);
    if (S[1][n] <= K) {
        printf("%d\n", K + (S[1][n] == K));
        return 0;
    }
    dy[0].set_bit(0, true);
    dy[1].set_bit(0, true);
    dy[1].set_bit(S[1][1], true);
    for(i=2;i<=n;i++) {
        dy[i].set_bit(S[1][i], true);
        for(int k=0;k<=S[1][i-1];k+=64) {
            uint64t prev_segment = dy[i-1].get_segment(k);
            dy[i].update_segment(k, prev_segment);
        }
        for(int j=0;j<i-1;j++) {
            int score = S[j+2][i];
            for(int k=0;k<=S[1][i];k+=64) {
                uint64t prev_segment = 0;
                if (k < score && score - k < 64) {
                    prev_segment = dy[j].get_segment(0);
                    //예를 들어 -1부터 64개 가져오세요 라는 쿼리를 받는다면?
                    //0~63까지 -> 0~62까지를 -> 1~63으로 바꿔서
                    prev_segment = prev_segment << (uint64t)(score - k);
                } else if (k >= score) {
                    prev_segment = dy[j].get_segment(k - score);
                }
                dy[i].update_segment(k, prev_segment);
            }
        }
    }
    int x = dy[n].get_first_zero_greater_than(K);
    printf("%d\n",x);
    return 0;
}
