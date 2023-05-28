#include "testlib.h"

using namespace std;

int n, k, a[100][100];

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);
    int t = inf.readInt();
    while (t--) {
        n = inf.readInt();
        k = inf.readInt();
        inf.readEoln();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                char c = inf.readChar();
                a[i][j] = (c == '*');
            }
            inf.readEoln();
        }
        int m = ouf.readInt(0, k, "m");
        for (int i = 0; i < m; ++i) {
            int x = ouf.readInt(1, n, "x"), y = ouf.readInt(1, n, "y");
            x--;
            y--;
            if (a[x][y] == 1) {
                quitf(_wa, "Incorrect output: the cell is not free");
            }
            if (a[x][y] == 2) {
                quitf(_wa, "Incorrect output: the cell is output twice");
            }
            a[x][y] = 2;
        }
    }
    quitf(_ok, "Correct output");
}
