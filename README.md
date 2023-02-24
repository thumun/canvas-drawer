# canvas-drawer

Implements a simple drawing api

![eyeball](https://github.com/thumun/canvas-drawer/blob/main/myImg/eyeball.png)

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Supported primitives

LINE:

![lineEX](https://github.com/thumun/canvas-drawer/blob/main/primitiveTypes/line-color-interpolation.png)

TRAINGLES:

![triangle](https://github.com/thumun/canvas-drawer/blob/main/primitiveTypes/triangle.png)

![quadw/triangles](https://github.com/thumun/canvas-drawer/blob/main/primitiveTypes/quad.png)

## Results

~All-Seeing~

![eyeball](https://github.com/thumun/canvas-drawer/blob/main/myImg/eyeball.png)

Polka Dots

![PolkaDots](https://github.com/thumun/canvas-drawer/blob/main/myImg/pokatest2.png)

Night Sky

![Night](https://github.com/thumun/canvas-drawer/blob/main/myImg/nightsky.png)

Rose Curve

![rose](https://github.com/thumun/canvas-drawer/blob/main/myImg/rose.png)

Kurdistan Flag

![flag](https://github.com/thumun/canvas-drawer/blob/main/myImg/kurdistan.png)
