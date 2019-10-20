
#include <stdio.h>

void bresenham(int x1, int y1, int x2, int y2) {
    int dx, dy, p, x, y;

    /* Slope less than or equal to 1 */
    if(( (y2 > y1) / (x2 - x1) ) <= 1) {
        dx = x2 - x1;
        dy = y2 - y1;

        x = x1;
        y = y1;

        if (x2 >= x1 && y2 >= y1) {
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

        if (x2 >= x1 && y2 <= y1) {
            p = 2 * dy + dx;
            while (x <= x2) {
                printf("(%d,%d)\n", x, y);
                x++;
                if (p > 0) {
                    p = p + 2 * dy;
                } else {
                    p = p + 2 * dy + 2 * dx;
                    y--;
                }
            }
        }

        if (x2 <= x1 && y2 >= y1) {
            p = - 2 * dy - dx;
            while (x >= x2) {
                printf("(%d,%d)\n", x, y);
                x--;
                if (p > 0) {
                    p = p - 2 * dy;
                } else {
                    p = p - 2 * dy - 2 * dx;
                    y++;
                }
            }
        }

        if (x2 <= x1 && y2 <= y1) {
            p = dx - 2 * dy;
            while (x >= x2) {
                printf("(%d,%d)\n", x, y);
                x--;
                if (p < 0) {
                    p = p - 2 * dy;
                } else {
                    p = p - 2 * dy + 2 * dx;
                    y--;
                }
            }
        }
    }

    /* Slope bigger than 1 */
    if(( (y2 > y1) / (x2 - x1) ) > 1) {
        //
    }
}

int main() {
    int x1 = 7, y1 = 2, x2 = 2, y2 = 5;
    
    bresenham(x1, y1, x2, y2);
    return 0;
}
