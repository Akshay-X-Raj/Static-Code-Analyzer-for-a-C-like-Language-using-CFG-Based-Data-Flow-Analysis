int x;
int y;

x = 0;
y = 0;

while (x < 5) {

    if (x < 3) {
        y = y + x;
    } else {
        y = y - x;
    }

    x = x + 1;
}