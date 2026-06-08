# 2D Graphics Editor in C

A menu-driven 2D graphics editor written in C that uses a 2D character array as a canvas, drawing shapes using `*` and `_` characters.

## Features

- Draw **Circle**, **Rectangle**, **Line**, and **Triangle**
- **Add** objects to the canvas
- **Delete** objects by ID (canvas redraws cleanly)
- **List** all active objects with their parameters
- **Clear** the entire canvas
- 30×80 character canvas displayed with borders

## Build & Run

```bash
gcc -o graphics_editor graphics_editor.c -lm
./graphics_editor
```

Requires a C compiler (gcc/clang) and the standard math library.

## How It Works

| Component | Details |
|---|---|
| Canvas | `char canvas[30][80]` — 2D array of characters |
| Circle | Midpoint circle algorithm |
| Line | Bresenham's line algorithm |
| Rectangle | Hollow rectangle via 4 edge draws |
| Triangle | Three lines connecting three vertices |
| Delete | Marks object inactive → redraws all remaining objects |

## Usage Example

```
  ╔══════════════════════════╗
  ║   2D GRAPHICS EDITOR     ║
  ╠══════════════════════════╣
  ║  1. Add Object           ║
  ║  2. Delete Object        ║
  ║  3. Display Canvas       ║
  ║  4. List Objects         ║
  ║  5. Clear Canvas         ║
  ║  0. Exit                 ║
  ╚══════════════════════════╝
```

Choose **1** to add a shape, enter its parameters, then **3** to display the canvas.

## File Structure

```
graphics_editor.c   — single-file implementation
README.md
```

## Assignment

Menu-driven 2D graphics editor in C using `*` and `_` characters.  
Course submission — Daily commits maintained as required.
