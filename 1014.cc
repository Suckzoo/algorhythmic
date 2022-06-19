#include <cstdio>
#include <iostream>
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

int n, m;
string a[10];
int dy[10][1<<10];

void input() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    fill_n(dy[0], 10 * (1<<10), -1);
}

bool seatsAvailable(int currentRowMask, int col) {
    return !(currentRowMask & (1<<col));
}

int markCurrentRowSeats(int currentRowMask, int col) {
    currentRowMask |= (1<<col);
    if (col < m - 1) {
        currentRowMask |= (1<<(col+1));
    }
    return currentRowMask;
}

int markNextRowSeats(int nextRowMask, int col) {
    if (col > 0) {
        nextRowMask |= (1<<(col-1));
    }
    if (col < m - 1) {
        nextRowMask |= (1<<(col+1));
    }
    return nextRowMask;
}

int maskRow(int row) {
    int rowMask = 0;
    for (int i = 0; i < m; i++) {
        if (a[row][i] == 'x') {
            rowMask |= (1<<i);
        }
    }
    return rowMask;
}

void dfs(int row, int col, int currentRowMask, int numStudents, int nextRowMask) {
    if (col == m) {
        if (numStudents > dy[row][nextRowMask]) {
            dy[row][nextRowMask] = numStudents;
        }
        return;
    }
    dfs(row, col+1, currentRowMask, numStudents, nextRowMask);
    if (seatsAvailable(currentRowMask, col)) {
        int currentRowMaskIfSeated = markCurrentRowSeats(currentRowMask, col);
        int nextRowMaskIfSeated = markNextRowSeats(nextRowMask, col);
        dfs(row, col+1, currentRowMaskIfSeated, numStudents + 1, nextRowMaskIfSeated);
    }
}

void seed() {
    int seedMask = maskRow(0);
    dfs(0, 0, seedMask, 0, 0);
}

void solve() {
    seed();
    for(int i = 1; i < n; i++) {
        for(int K = 0; K < (1<<m); K++) {
            if (dy[i-1][K] == -1) {
                continue;
            }
            int mask = K | maskRow(i);
            dfs(i, 0, mask, dy[i-1][K], 0);
        }
    }
    int maxStudents = -1;
    for(int K = 0; K < (1<<m); K++) {
        if (dy[n-1][K] > maxStudents) {
            maxStudents = dy[n-1][K];
        }
    }
    cout << maxStudents << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int T;
    cin >> T;
    while(T--) {
        input();
        solve();
    }
    return 0;
}
