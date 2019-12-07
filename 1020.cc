#include <cstdio>
#include <cstring>

typedef long long ll;
const int cost[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 5};
ll exp10[16] = {1, };
char digit[20];

int total_cost = 0;
int ndigit;
ll num;

ll next_int(int remain_cost, ll ref, int target_digit, ll current) {
    if (target_digit == -1) {
        if (!remain_cost && current > ref) return current;
        return -1ll;
    }
    
    if (current + exp10[target_digit + 1] - 1 <= ref) return -1;
    if (remain_cost > 7 * (target_digit + 1)) return -1;

    for(ll i=0;i<10;i++) {
        if (remain_cost >= cost[i]) {
            ll next_int_cand = next_int(remain_cost - cost[i], ref, target_digit - 1, current + i * exp10[target_digit]); 
            if (next_int_cand != -1ll) return next_int_cand;
        }
    }
    return -1ll;
}

int main() {
    int i;
    for(i=1;i<16;i++) {
        exp10[i] = exp10[i-1] * 10;
    }

    scanf("%s", digit);
    ndigit = strlen(digit);
    for(i=0;i<ndigit;i++) {
        num *= (ll)10;
        num += (ll)(digit[i] - '0');
        total_cost += cost[digit[i] - '0'];
    }
    ll target = next_int(total_cost, num, ndigit - 1, 0ll);
    ll diff = 0ll;
    if (target >= 0) {
        diff = target - num;
    } else {
        target = next_int(total_cost, 0ll, ndigit - 1, 0ll);
        diff = exp10[ndigit] - num + target;
    }
    printf("%lld\n", diff);
    return 0;
}
