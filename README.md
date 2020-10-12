# Graph-c

A graphing tool written in C.

## Build

It requires c99 and link with `math` lib.

```bash
gcc graph.c -lm -o graph
```

## Usage

Currently the only way to change the function is to change the source codes `f` function.

There are also some command lines available:
```
$ graph --help
Usage: ./graph [OPTIONS]

OPTIONS:
    --no-color                           -    Print graph without color.
    --scale or -s <NUMBER>               -    Scale/zoom the graph.
    --height or -h <INTEGER>             -    The height of the graph.
    --wigth or -w <INTEGER>              -    The width of the graph.
    --intercept-char or -i <CHARACTER>   -    The intercept char on the graph, default is '@'
    --help or -?                         -    Prints this usage text.
```

## Examples 

Function `f(x) = x`:

![`f(x) = x`](./assets/f_x.png)

Function `f(x) = x * x`:

![`f(x) = x * x`](./assets/f_x_x.png)

Function `f(x) = sin(x)`:

![`f(x) = sin(x)`](./assets/f_sin_x.png)

Function `f(x) = tan(x)`:

![`f(x) = tan(x)`](./assets/f_tan_x.png)

## TODO

 - The ability to change functions dynamically. (This requires lexer, parser, ast and executor).
 - The ability to change the colors of both coords and function.
 - ...

