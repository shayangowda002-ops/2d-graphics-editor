/*
 * 2D Graphics Editor in C
 * Uses a 2D character array as canvas
 * Supports: Circle, Rectangle, Line, Triangle
 * Operations: Add, Delete, Display
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS 30
#define COLS 80
#define MAX_OBJECTS 50

/* ─── Canvas ─────────────────────────────────────────── */
char canvas[ROWS][COLS];

/* ─── Object Types ───────────────────────────────────── */
typedef enum {
    OBJ_CIRCLE,
    OBJ_RECTANGLE,
    OBJ_LINE,
    OBJ_TRIANGLE
} ObjectType;

/* ─── Object Storage ─────────────────────────────────── */
typedef struct {
    int  id;
    int  active;
    ObjectType type;
    /* Parameters (meaning depends on type) */
    int x1, y1, x2, y2, x3, y3, radius;
    char ch;   /* drawing character */
} Object;

Object objects[MAX_OBJECTS];
int    obj_count = 0;
int    next_id   = 1;

/* ════════════════════════════════════════════════════════
 *  CANVAS HELPERS
 * ════════════════════════════════════════════════════════ */

void init_canvas(void) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            canvas[r][c] = ' ';
}

void display_canvas(void) {
    /* Top border */
    printf("+");
    for (int c = 0; c < COLS; c++) printf("-");
    printf("+\n");

    for (int r = 0; r < ROWS; r++) {
        printf("|");
        for (int c = 0; c < COLS; c++)
            putchar(canvas[r][c]);
        printf("|\n");
    }

    /* Bottom border */
    printf("+");
    for (int c = 0; c < COLS; c++) printf("-");
    printf("+\n");
}

static inline void plot(int r, int c, char ch) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = ch;
}

/* ════════════════════════════════════════════════════════
 *  DRAWING PRIMITIVES
 * ════════════════════════════════════════════════════════ */

/* Bresenham line */
void draw_line(int r0, int c0, int r1, int c1, char ch) {
    int dr = abs(r1 - r0), dc = abs(c1 - c0);
    int sr = (r0 < r1) ? 1 : -1;
    int sc = (c0 < c1) ? 1 : -1;
    int err = dr - dc;

    while (1) {
        plot(r0, c0, ch);
        if (r0 == r1 && c0 == c1) break;
        int e2 = 2 * err;
        if (e2 > -dc) { err -= dc; r0 += sr; }
        if (e2 <  dr) { err += dr; c0 += sc; }
    }
}

/* Midpoint circle */
void draw_circle(int cx, int cy, int radius, char ch) {
    int x = 0, y = radius;
    int d = 1 - radius;

    while (x <= y) {
        plot(cx + x, cy + y, ch);
        plot(cx - x, cy + y, ch);
        plot(cx + x, cy - y, ch);
        plot(cx - x, cy - y, ch);
        plot(cx + y, cy + x, ch);
        plot(cx - y, cy + x, ch);
        plot(cx + y, cy - x, ch);
        plot(cx - y, cy - x, ch);

        if (d < 0)      d += 2 * x + 3;
        else          { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

/* Rectangle (hollow) */
void draw_rectangle(int r1, int c1, int r2, int c2, char ch) {
    for (int c = c1; c <= c2; c++) { plot(r1, c, ch); plot(r2, c, ch); }
    for (int r = r1; r <= r2; r++) { plot(r, c1, ch); plot(r, c2, ch); }
}

/* Triangle given three vertices */
void draw_triangle(int r1, int c1, int r2, int c2, int r3, int c3, char ch) {
    draw_line(r1, c1, r2, c2, ch);
    draw_line(r2, c2, r3, c3, ch);
    draw_line(r3, c3, r1, c1, ch);
}

/* ════════════════════════════════════════════════════════
 *  ERASE: redraw canvas from scratch (delete support)
 * ════════════════════════════════════════════════════════ */

void redraw_all(void) {
    init_canvas();
    for (int i = 0; i < obj_count; i++) {
        if (!objects[i].active) continue;
        Object *o = &objects[i];
        switch (o->type) {
            case OBJ_CIRCLE:
                draw_circle(o->x1, o->y1, o->radius, o->ch); break;
            case OBJ_RECTANGLE:
                draw_rectangle(o->x1, o->y1, o->x2, o->y2, o->ch); break;
            case OBJ_LINE:
                draw_line(o->x1, o->y1, o->x2, o->y2, o->ch); break;
            case OBJ_TRIANGLE:
                draw_triangle(o->x1, o->y1, o->x2, o->y2,
                              o->x3, o->y3, o->ch); break;
        }
    }
}

/* ════════════════════════════════════════════════════════
 *  OBJECT MANAGEMENT
 * ════════════════════════════════════════════════════════ */

int add_object(Object obj) {
    if (obj_count >= MAX_OBJECTS) {
        printf("Canvas full! Max %d objects.\n", MAX_OBJECTS);
        return -1;
    }
    obj.id     = next_id++;
    obj.active = 1;
    objects[obj_count++] = obj;
    return obj.id;
}

void delete_object(int id) {
    for (int i = 0; i < obj_count; i++) {
        if (objects[i].id == id && objects[i].active) {
            objects[i].active = 0;
            redraw_all();
            printf("Object #%d deleted.\n", id);
            return;
        }
    }
    printf("Object #%d not found.\n", id);
}

void list_objects(void) {
    int found = 0;
    printf("\n%-5s %-12s %s\n", "ID", "Type", "Parameters");
    printf("%-5s %-12s %s\n", "---", "----------", "----------");
    for (int i = 0; i < obj_count; i++) {
        if (!objects[i].active) continue;
        found = 1;
        Object *o = &objects[i];
        printf("%-5d ", o->id);
        switch (o->type) {
            case OBJ_CIRCLE:
                printf("%-12s center(%d,%d) r=%d\n",
                       "Circle", o->x1, o->y1, o->radius); break;
            case OBJ_RECTANGLE:
                printf("%-12s (%d,%d)-(%d,%d)\n",
                       "Rectangle", o->x1, o->y1, o->x2, o->y2); break;
            case OBJ_LINE:
                printf("%-12s (%d,%d)->(%d,%d)\n",
                       "Line", o->x1, o->y1, o->x2, o->y2); break;
            case OBJ_TRIANGLE:
                printf("%-12s (%d,%d)(%d,%d)(%d,%d)\n",
                       "Triangle", o->x1, o->y1,
                       o->x2, o->y2, o->x3, o->y3); break;
        }
    }
    if (!found) printf("  (no objects)\n");
    printf("\n");
}

/* ════════════════════════════════════════════════════════
 *  INPUT HELPERS
 * ════════════════════════════════════════════════════════ */

void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int(const char *prompt) {
    int v;
    printf("%s", prompt);
    while (scanf("%d", &v) != 1) { flush_stdin(); printf("  Invalid. %s", prompt); }
    flush_stdin();
    return v;
}

char get_char(const char *prompt) {
    char ch;
    printf("%s", prompt);
    scanf(" %c", &ch);
    flush_stdin();
    return ch;
}

/* ════════════════════════════════════════════════════════
 *  ADD MENUS
 * ════════════════════════════════════════════════════════ */

void add_circle(void) {
    Object o = {0};
    o.type = OBJ_CIRCLE;
    printf("\n  -- Add Circle --\n");
    printf("  Canvas size: %d rows x %d cols\n", ROWS, COLS);
    o.x1     = get_int("  Center row   : ");
    o.y1     = get_int("  Center col   : ");
    o.radius = get_int("  Radius       : ");
    o.ch     = get_char("  Draw char (*/_/o): ");
    int id = add_object(o);
    if (id > 0) {
        draw_circle(o.x1, o.y1, o.radius, o.ch);
        printf("  Circle added as object #%d\n", id);
    }
}

void add_rectangle(void) {
    Object o = {0};
    o.type = OBJ_RECTANGLE;
    printf("\n  -- Add Rectangle --\n");
    o.x1 = get_int("  Top-left  row: ");
    o.y1 = get_int("  Top-left  col: ");
    o.x2 = get_int("  Bot-right row: ");
    o.y2 = get_int("  Bot-right col: ");
    o.ch = get_char("  Draw char (*/_): ");
    int id = add_object(o);
    if (id > 0) {
        draw_rectangle(o.x1, o.y1, o.x2, o.y2, o.ch);
        printf("  Rectangle added as object #%d\n", id);
    }
}

void add_line(void) {
    Object o = {0};
    o.type = OBJ_LINE;
    printf("\n  -- Add Line --\n");
    o.x1 = get_int("  Start row: ");
    o.y1 = get_int("  Start col: ");
    o.x2 = get_int("  End   row: ");
    o.y2 = get_int("  End   col: ");
    o.ch = get_char("  Draw char (*/_): ");
    int id = add_object(o);
    if (id > 0) {
        draw_line(o.x1, o.y1, o.x2, o.y2, o.ch);
        printf("  Line added as object #%d\n", id);
    }
}

void add_triangle(void) {
    Object o = {0};
    o.type = OBJ_TRIANGLE;
    printf("\n  -- Add Triangle --\n");
    o.x1 = get_int("  Vertex 1 row: ");
    o.y1 = get_int("  Vertex 1 col: ");
    o.x2 = get_int("  Vertex 2 row: ");
    o.y2 = get_int("  Vertex 2 col: ");
    o.x3 = get_int("  Vertex 3 row: ");
    o.y3 = get_int("  Vertex 3 col: ");
    o.ch = get_char("  Draw char (*/_): ");
    int id = add_object(o);
    if (id > 0) {
        draw_triangle(o.x1, o.y1, o.x2, o.y2, o.x3, o.y3, o.ch);
        printf("  Triangle added as object #%d\n", id);
    }
}

/* ════════════════════════════════════════════════════════
 *  MENUS
 * ════════════════════════════════════════════════════════ */

void add_menu(void) {
    int choice;
    printf("\n");
    printf("  ╔══════════════════════╗\n");
    printf("  ║     ADD OBJECT       ║\n");
    printf("  ╠══════════════════════╣\n");
    printf("  ║  1. Circle           ║\n");
    printf("  ║  2. Rectangle        ║\n");
    printf("  ║  3. Line             ║\n");
    printf("  ║  4. Triangle         ║\n");
    printf("  ║  0. Back             ║\n");
    printf("  ╚══════════════════════╝\n");
    choice = get_int("  Choice: ");
    switch (choice) {
        case 1: add_circle();    break;
        case 2: add_rectangle(); break;
        case 3: add_line();      break;
        case 4: add_triangle();  break;
        case 0: break;
        default: printf("  Invalid choice.\n");
    }
}

void delete_menu(void) {
    list_objects();
    if (obj_count == 0) return;
    int id = get_int("  Enter object ID to delete (0=cancel): ");
    if (id != 0) delete_object(id);
}

void main_menu(void) {
    int choice;
    do {
        printf("\n");
        printf("  ╔══════════════════════════╗\n");
        printf("  ║   2D GRAPHICS EDITOR     ║\n");
        printf("  ╠══════════════════════════╣\n");
        printf("  ║  1. Add Object           ║\n");
        printf("  ║  2. Delete Object        ║\n");
        printf("  ║  3. Display Canvas       ║\n");
        printf("  ║  4. List Objects         ║\n");
        printf("  ║  5. Clear Canvas         ║\n");
        printf("  ║  0. Exit                 ║\n");
        printf("  ╚══════════════════════════╝\n");
        choice = get_int("  Choice: ");
        switch (choice) {
            case 1: add_menu();    break;
            case 2: delete_menu(); break;
            case 3:
                printf("\n");
                display_canvas();
                break;
            case 4: list_objects(); break;
            case 5:
                init_canvas();
                obj_count = 0;
                next_id   = 1;
                printf("  Canvas cleared.\n");
                break;
            case 0:
                printf("  Goodbye!\n");
                break;
            default:
                printf("  Invalid choice.\n");
        }
    } while (choice != 0);
}

/* ════════════════════════════════════════════════════════
 *  MAIN
 * ════════════════════════════════════════════════════════ */

int main(void) {
    init_canvas();
    printf("\n  Welcome to 2D Graphics Editor!\n");
    printf("  Canvas: %d rows x %d cols\n", ROWS, COLS);
    main_menu();
    return 0;
}
