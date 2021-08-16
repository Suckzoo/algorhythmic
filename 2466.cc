#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <memory.h>
#include <deque>
#include <queue>
#include <cassert>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

int n;
struct Book {
    int w;
    int h;
    int i;
};
Book a[100100];
int valid[100100];
int dy[100100];
int n2dy[100100];
int ws[100100];
struct BookDP {
    int dy;
    int h;
    int k;
    int i;
    bool operator < (const BookDP &x) const {
        return dy + h > x.dy + x.h;
    }
};
deque<BookDP> deq;
priority_queue<BookDP> que;

int possible(int target) {
    memset(dy, 0, sizeof(dy));
    memset(valid, -1, sizeof(valid));
    deq.clear();
    while(!que.empty()) que.pop();
    int k = 0;
    for(int i = 1; i <= n; i++) {
        debug("Inserting book %d", i);
        while (ws[i] - ws[k] > target) {
            k++;
        }
        while (!deq.empty()) {
            auto front = deq.front();
            if (front.i <= k) {
                deq.pop_front();
                valid[front.k] = -1;
            } else if (front.k < k) {
                valid[front.k] = -1;
                BookDP bdp = {
                    .dy = dy[k],
                    .h = front.h,
                    .k = k,
                    .i = front.i,
                };
                valid[k] = dy[k] + front.h;
                deq.pop_front();
                deq.push_front(bdp);
                que.push(bdp);
                break;
            } else {
                break;
            }
            // if (ws[i] - ws[front.i - 1] > target) {
            //     deq.pop_front();
            // } else break;
        }
        while(!deq.empty()) {
            auto back = deq.back();
            if (back.h <= a[i].h) {
                valid[back.k] = -1;
                deq.pop_back();
            } else break;
        }
        BookDP x;
        if (deq.empty()) {
            x = {
                .dy = dy[k],
                .h = a[i].h,
                .k = k,
                .i = i
            };
            valid[k] = dy[k] + a[i].h;
        } else {
            x = {
                .dy = dy[deq.back().i],
                .h = a[i].h,
                .k = deq.back().i,
                .i = i
            };
            valid[deq.back().i] = dy[deq.back().i] + a[i].h;
        }
        que.push(x);
        deq.push_back(x);
        while(!que.empty()) {
            auto bdp = que.top();
            debug("dy: %d, h: %d, i: %d, k: %d, valid[k]: %d", bdp.dy, bdp.h, bdp.i, bdp.k, valid[bdp.k]);
            if (bdp.dy + bdp.h == valid[bdp.k]) {
                dy[i] = bdp.dy + bdp.h;
                break;
            } else {
                que.pop();
            }
        }
    }
#ifdef DEBUG
    n2dy[1] = a[1].h;
    for(int i=2;i<=n;i++) {
        int cand = 2000000000;
        int maxH = -1;
        for(int j = i-1; j >= 0; j--) {
            maxH = maxH < a[j+1].h ? a[j+1].h : maxH;
            if (ws[i] - ws[j] > target) break;
            if (n2dy[j] + maxH < cand) cand = n2dy[j] + maxH;
        }
        n2dy[i] = cand;
    }
    for(int i=0;i<=n;i++) {
        debug("%d %d", dy[i], n2dy[i]);
    }
    for(int i=0;i<=n;i++) {
        if (dy[n] != n2dy[n]) {
            printf("WRONG!!!!\n");
            break;
        }
    }
#endif
    if (dy[n] <= target) return 1;
    return 0;
}

int main() {
    scanf("%d",&n);
    int i;
    int hMax = 0, wMax = 0;
    for(i=1;i<=n;i++) {
        scanf("%d%d",&a[i].w,&a[i].h);
        a[i].i = i;
        ws[i] = ws[i-1] + a[i].w;
        wMax = a[i].w > wMax ? a[i].w : wMax;
        hMax = a[i].h > hMax ? a[i].h : hMax;
    }
    int s = wMax, e = ws[n] > hMax ? ws[n] : hMax;
    int ans = e;
    while(s <= e) {
        int p = s + (e - s) / 2;
        debug("Testing target %d", p);
        if (possible(p)) {
            ans = p;
            e = p - 1;
        } else {
            s = p + 1;
        }
    }
    printf("%d\n", ans);
    return 0;
}
