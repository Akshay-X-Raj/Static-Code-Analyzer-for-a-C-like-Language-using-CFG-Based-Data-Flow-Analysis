int x;
int y;
int z;
int p;
x = 0;
y = 10;

while (x < 5) {

    if (x < 3) {
        y = y + x;
    } else {
        y = y - x;
    }

    if (y > 5) {
        z = y + x;
    } else {
        z = y - x;
    }

    x = x + 1;
}

printf("%d", y);