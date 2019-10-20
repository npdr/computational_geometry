
#include <stdio.h>

void bresenham(int x1, int y1, int x2, int y2) {
    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;

    x = x1;
    y = y1;

    p = 2 * dy - dx;

    while (x <= x2) {
        printf("(%d,%d)\n", x, y);
        x++;
        if (p < 0) {
            p = p + 2 * dy;
        } else {
            p = p + 2 * dy - 2 * dx;
            y++;
        }
    }
}

int main() {
    int x1 = 4, y1 = 2, x2 = 4, y2 = 8;
    bresenham(x1, y1, x2, y2);
    return 0;
}
