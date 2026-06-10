#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 24
#define EMPTY '_'
#define PIXEL '*'
#define MAX_OBJECTS 50

char picture[HEIGHT][WIDTH];

/* ── Object storage ── */
typedef struct {
    int active;
    int type;          /* 1=line 2=rect 3=circle 4=triangle */
    int x1,y1,x2,y2,x3,y3,radius;
} Object;

Object objects[MAX_OBJECTS];
int obj_count = 0;

/* ══════════════════════════════════════════
   CORE FUNCTIONS
══════════════════════════════════════════ */

void clearPicture() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            picture[y][x] = EMPTY;
}

void displayPicture() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            putchar(picture[y][x]);
        putchar('\n');
    }
}

void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        picture[y][x] = PIXEL;
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2-x1), dy = abs(y2-y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (1) {
        setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y2, x2, y2);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
}

void drawCircle(int cx, int cy, int radius) {
    int x = 0, y = radius;
    int d = 1 - radius;
    while (x <= y) {
        setPixel(cx+x, cy+y); setPixel(cx-x, cy+y);
        setPixel(cx+x, cy-y); setPixel(cx-x, cy-y);
        setPixel(cx+y, cy+x); setPixel(cx-y, cy+x);
        setPixel(cx+y, cy-x); setPixel(cx-y, cy-x);
        if (d < 0)     d += 2*x + 3;
        else         { d += 2*(x-y) + 5; y--; }
        x++;
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* ── Redraw all active objects ── */
void redrawAll() {
    clearPicture();
    for (int i = 0; i < obj_count; i++) {
        if (!objects[i].active) continue;
        Object *o = &objects[i];
        switch (o->type) {
            case 1: drawLine(o->x1,o->y1,o->x2,o->y2); break;
            case 2: drawRectangle(o->x1,o->y1,o->x2,o->y2); break;
            case 3: drawCircle(o->x1,o->y1,o->radius); break;
            case 4: drawTriangle(o->x1,o->y1,o->x2,o->y2,o->x3,o->y3); break;
        }
    }
}

/* ══════════════════════════════════════════
   MAIN
══════════════════════════════════════════ */

int main() {
    int choice;
    clearPicture();

    while (1) {
        printf("2D Graphics Editor\n");
        printf("Canvas size: %d x %d\n", WIDTH, HEIGHT);
        printf("1. Add object\n");
        printf("2. Delete object\n");
        printf("3. Modify object\n");
        printf("4. Display picture\n");
        printf("5. List objects\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int type;
            printf("Choose shape type:\n");
            printf("1. Line\n2. Rectangle\n3. Circle\n4. Triangle\n");
            printf("Enter shape type: ");
            scanf("%d", &type);

            Object o = {0};
            o.active = 1;
            o.type = type;

            if (type == 1) {
                printf("Enter x1 y1 x2 y2: ");
                scanf("%d %d %d %d", &o.x1,&o.y1,&o.x2,&o.y2);
                drawLine(o.x1,o.y1,o.x2,o.y2);
            } else if (type == 2) {
                printf("Enter top-left x y and bottom-right x y: ");
                scanf("%d %d %d %d", &o.x1,&o.y1,&o.x2,&o.y2);
                drawRectangle(o.x1,o.y1,o.x2,o.y2);
            } else if (type == 3) {
                printf("Enter center x y and radius: ");
                scanf("%d %d %d", &o.x1,&o.y1,&o.radius);
                drawCircle(o.x1,o.y1,o.radius);
            } else if (type == 4) {
                printf("Enter x1 y1 x2 y2 x3 y3: ");
                scanf("%d %d %d %d %d %d",
                      &o.x1,&o.y1,&o.x2,&o.y2,&o.x3,&o.y3);
                drawTriangle(o.x1,o.y1,o.x2,o.y2,o.x3,o.y3);
            }
            objects[obj_count] = o;
            printf("Object added with index %d.\n", obj_count);
            obj_count++;

        } else if (choice == 2) {
            int idx;
            printf("Enter object index to delete: ");
            scanf("%d", &idx);
            if (idx >= 0 && idx < obj_count && objects[idx].active) {
                objects[idx].active = 0;
                redrawAll();
                printf("Object %d deleted.\n", idx);
            } else {
                printf("Invalid index.\n");
            }

        } else if (choice == 3) {
            int idx, type;
            printf("Enter object index to modify: ");
            scanf("%d", &idx);
            if (idx >= 0 && idx < obj_count && objects[idx].active) {
                printf("Choose new shape type:\n");
                printf("1. Line\n2. Rectangle\n3. Circle\n4. Triangle\n");
                printf("Enter shape type: ");
                scanf("%d", &type);
                Object o = {0};
                o.active = 1;
                o.type = type;
                if (type == 1) {
                    printf("Enter x1 y1 x2 y2: ");
                    scanf("%d %d %d %d", &o.x1,&o.y1,&o.x2,&o.y2);
                } else if (type == 2) {
                    printf("Enter top-left x y and bottom-right x y: ");
                    scanf("%d %d %d %d", &o.x1,&o.y1,&o.x2,&o.y2);
                } else if (type == 3) {
                    printf("Enter center x y and radius: ");
                    scanf("%d %d %d", &o.x1,&o.y1,&o.radius);
                } else if (type == 4) {
                    printf("Enter x1 y1 x2 y2 x3 y3: ");
                    scanf("%d %d %d %d %d %d",
                          &o.x1,&o.y1,&o.x2,&o.y2,&o.x3,&o.y3);
                }
                objects[idx] = o;
                redrawAll();
                printf("Object %d modified.\n", idx);
            } else {
                printf("Invalid index.\n");
            }

        } else if (choice == 4) {
            displayPicture();

        } else if (choice == 5) {
            printf("Objects:\n");
            for (int i = 0; i < obj_count; i++) {
                if (!objects[i].active) continue;
                Object *o = &objects[i];
                printf("  [%d] ", i);
                switch(o->type) {
                    case 1: printf("Line (%d,%d)->(%d,%d)\n",o->x1,o->y1,o->x2,o->y2); break;
                    case 2: printf("Rectangle (%d,%d)-(%d,%d)\n",o->x1,o->y1,o->x2,o->y2); break;
                    case 3: printf("Circle center(%d,%d) r=%d\n",o->x1,o->y1,o->radius); break;
                    case 4: printf("Triangle (%d,%d)(%d,%d)(%d,%d)\n",o->x1,o->y1,o->x2,o->y2,o->x3,o->y3); break;
                }
            }

        } else if (choice == 0) {
            printf("Not a good choice bye.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
