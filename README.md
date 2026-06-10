# 2D Graphics Editor in C

A menu-driven 2D graphics editor written in C that uses a 2D character array as a canvas, drawing shapes using `*` and `_` characters.

## Problem Statement

Write a menu driven program for a 2D graphics editor in C using `*` and `_`.
- Draw objects: Circle, Rectangle, Line, Triangle
- Add objects to the picture
- Delete objects from the picture
- Use a 2D array of characters to store the picture
- Display the picture

## How to Compile and Run

```bash
gcc -o graphics_editor graphics_editor.c -lm
./graphics_editor
```

## Menu Options

```
2D Graphics Editor
Canvas size: 80 x 24
1. Add object
2. Delete object
3. Modify object
4. Display picture
5. List objects
0. Exit
```

## Shapes Supported

| Shape | Input Required |
|---|---|
| Line | x1 y1 x2 y2 |
| Rectangle | top-left x y, bottom-right x y |
| Circle | center x y, radius |
| Triangle | x1 y1 x2 y2 x3 y3 |

## How It Works

| Component | Details |
|---|---|
| Canvas | `char picture[24][80]` — 2D array of characters |
| Empty cell | `_` underscore |
| Drawn pixel | `*` asterisk |
| Line | Bresenham's Line Algorithm |
| Circle | Midpoint Circle Algorithm |
| Rectangle | Four lines connecting corners |
| Triangle | Three lines connecting vertices |
| Delete | Marks object inactive, redraws all remaining |

## Sample Output

### Line
```
________________________________________________________________________________
________________________________________________________________________________
__**____________________________________________________________________________
____***_________________________________________________________________________
_______***______________________________________________________________________
__________***___________________________________________________________________
```

### Rectangle
```
_____*********************______________________________________________________
_____*___________________*______________________________________________________
_____*___________________*______________________________________________________
_____*********************______________________________________________________
```

### Circle
```
______________________________________*****_____________________________________
_____________________________________*_____*____________________________________
____________________________________*_______*___________________________________
___________________________________*_________*__________________________________
______________________________________*****_____________________________________
```

### Triangle
```
____________________*___________________________________________________________
___________________*_*__________________________________________________________
__________________*___*_________________________________________________________
__________*********************_________________________________________________
```

## AI Tool Used

This project was developed using **Claude (claude.ai)** by Anthropic.
All prompts and responses are documented in `prompt.txt`.

## Files

```
graphics_editor.c   — Main C source code
README.md           — Project documentation
prompt.txt          — AI interaction log
```
