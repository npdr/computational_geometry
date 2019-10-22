#include <stdio.h>
#include <math.h>

void bresenham(int x1, int y1, int x2, int y2) {
    int dx, dy, p, x, y;

    /* Vertical line */
    if(x1 == x2) {
        x = x1;
        y = y1;

        if(y2 >= y1) {
            while(y <= y2) {
                printf("(%d,%d)\n", x, y);
                y++;
            }
        }

        if(y2 < y1)
        while(y >= y2) {
            printf("(%d,%d)\n", x, y);
            y--;
        }
        return;
    }

    /* Slope less than or equal to 1 */
    if(fabs( (y2 - y1) / (x2 - x1) ) <= 1) {
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
            return;
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
            return;
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
            return;
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
            return;
        }
        return;
    }

    /* Slope bigger than 1 */
    if(fabs( (y2 - y1) / (x2 - x1) ) > 1) {
        int aux = x1;
        x1 = y1;
        y1 = aux;
        aux = x2;
        x2 = y2;
        y2 = aux;

        dx = x2 - x1;
        dy = y2 - y1;

        x = x1;
        y = y1;

        if (x2 >= x1 && y2 >= y1) {
            p = 2 * dy - dx;
            while (x <= x2) {
                printf("21(%d,%d)\n", y, x);
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
                printf("22(%d,%d)\n", y, x);
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
                printf("23(%d,%d)\n", y, x);
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
                printf("24(%d,%d)\n", y, x);
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
}

int main() {
    //int x1 = 1, y1 = 1, x2 = 3, y2 = 6;
    //int x1 = 3, y1 = 6, x2 = 1, y2 = 1;
    //int x1 = 3, y1 = 1, x2 = 1, y2 = 6;
    //int x1 = 1, y1 = 6, x2 = 3, y2 = 1;
    //int x1 = 8, y1 = 3, x2 = 2, y2 = 3;  
    int x1 = 7, y1 = 5, x2 = 8, y2 = 8;
    bresenham(x1, y1, x2, y2);
    return 0;
}
