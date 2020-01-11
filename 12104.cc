#include <cstdio>
#include <cstring>

char a[200010];
char b[100010];
int pi[100010];
int n;

int main() {
    scanf("%s",b);
    scanf("%s",a);
    n = strlen(a);
    int i;
    for(i=0;i<n;i++) {
        a[i+n] = a[i];
    }
    int k = 0;
    for(i=1;i<n;i++) {
        if(b[i] == b[k]) {
            pi[i] = ++k;
        } else {
            while (k > 0) {
                k = pi[k-1];
                if (b[i] == b[k]) break;
            }
            if (b[i] != b[k]) {
                k = 0;
                pi[i] = 0;
            } else {
                k++;
                pi[i] = k;
            }
        }
    }

    int ans = 0;
    k = 0;
    for(i=0;i<2*n-1;i++) {
        if(a[i] != b[k]) {
            while(k > 0) {
                k = pi[k-1];
                if (a[i] == b[k]) break;
            }
            if(a[i] != b[k]) k = -1;
        }
        k++;
        if (k == n) {
            k--;
            ans++;
            while(k > 0) {
                k = pi[k-1];
                if (a[i] == b[k]) break;
            }
            if (a[i] != b[k]) k = -1;
            k++;
        }
    }
    printf("%d\n",ans);
    return 0;
}