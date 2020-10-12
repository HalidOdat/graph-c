/**
*
*    Copyright (c) 2020 Haled Odat <halidodat@gmail.com, haledodat@gmail.com>
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy of this
*    software and associated documentation files (the "Software"), to deal in the Software
*    without restriction, including without limitation the rights to use, copy, modify, merge,
*    publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
*    to whom the Software is furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all copies or
*    substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
*    BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
*    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
**/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef uint32_t u32;
typedef int32_t i32;
typedef uint32_t u64;
typedef int32_t i64;
typedef double f64;
typedef size_t usize;

// The function to be graphed.
//
// Return `NAN` if there is a hole in the function.
f64 f(f64 x) {
	return x * x;
}

// The cli options.
typedef struct Opts {
	bool no_color;
	f64 scale;
	u32 height;
	u32 width;
	char intercept_char;
} Opts;

void print_usage() {
    printf(
        "Usage: ./graph [OPTIONS]\n"
        "\n"
        "OPTIONS:\n"
        "    --no-color                           -    Print graph without color.\n"
        "    --scale or -s <NUMBER>               -    Scale/zoom the graph.\n"
        "    --height or -h <INTEGER>             -    The height of the graph.\n"
        "    --wigth or -w <INTEGER>              -    The width of the graph.\n"
        "    --intercept-char or -i <CHARACTER>   -    The intercept char on the graph, default is '@'\n"
        "    --help or -?                         -    Prints this usage text.\n"
        "\n"
    );
}

// Parse the cli options from `argv`.
Opts opts_from(const char** argv, const int argc) {
	Opts opts = { true, 4.5, 30, 60, '@'};
	if (argc == 1) {
		return opts;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
			print_usage();
			exit(0);
		} else if (strcmp(argv[i], "--no-color") == 0) {
			opts.no_color = false;
		} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--scale") == 0) {
			if (argc - 1 >= i + 1) {
				const char* value = argv[++i];
				const f64 number = strtod(value, NULL);
				opts.scale = number;
			} else {
				fprintf(stderr, "error: '--scale' expected number argument\n");
				exit(-1);
			}
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) {
			if (argc - 1 >= i + 1) {
				const char* value = argv[++i];
				const i32 number = atoi(value);
				if (number < 20) {
					fprintf(stderr, "error: `--height` argument must be greater than 20\n");
					exit(-1);
				}
				opts.height = number;
			} else {
				fprintf(stderr, "error: '--height' expected integer argument\n");
				exit(-1);
			}
		} else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
			if (argc - 1 >= i + 1) {
				const char* value = argv[++i];
				const i32 number = atoi(value);
				if (number < 20) {
					fprintf(stderr, "error: '--width' argument must be greater than 20\n");
					exit(-1);
				} 
				opts.width = number;
			} else {
				fprintf(stderr, "error: '--width' expected integer argument\n");
				exit(-1);
			}
		} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--intercept-char") == 0) {
			if (argc - 1 >= i + 1) {
				const char* value = argv[++i];
				if (strlen(value) == 0) {
					opts.intercept_char = '@';
				} else {
					opts.intercept_char = value[0];
				}
			} else {
				fprintf(stderr, "error: '--intercept-char' expected character argument\n");
				exit(-1);
			}
		} else {
			fprintf(stderr, "Unexpected argument: %s\n", argv[i]);
			exit(-1);
		}
	}
	return opts;
}

int main(const int argc, const char** argv) {
	Opts opts = opts_from(argv, argc);

	// Height of the graph.
	const u32 HEIGHT = opts.height;
	// Width of the graph.
	const u32 WIDTH = opts.width;

	const u32 SIZE = WIDTH * HEIGHT;

	const i32 X_MAX = WIDTH / 2;
	const i32 X_MIN = -X_MAX;
	
	const i32 Y_MAX = HEIGHT / 2;
	const i32 Y_MIN = -Y_MAX;

	const f64 STEP = 0.001;

	// The domain this function will be graphed against.
	const f64 DOMAIN = fmax(HEIGHT, WIDTH);

	// The scale/zoom of the graph.
	const f64 SCALE = opts.scale;

	const char INTERCEPT_CHAR = opts.intercept_char;

	// The actual graph data.
	char graph[SIZE];

	// Initialize the graph with whitespace.
	for (usize i = 0; i < SIZE; i++) {
		graph[i] = ' ';
	}

    // Plot the points on the graph.
	f64 x = -DOMAIN;
	while (x < DOMAIN) {
		const f64 y = f(x / SCALE) * SCALE;

		if (isnan(y) || x >= X_MAX || x <= X_MIN || y >= Y_MAX || y <= Y_MIN) {
			x += STEP;
			continue;
		}

		const i64 index = (round(-y) - Y_MIN) * WIDTH + (round(x) - X_MIN);
        if (!(index < 0 || index >= (i64)SIZE)) {
            graph[index] = '*';
        }

		x += STEP;
	}

	// Draw y coordinate
	for (i32 y = Y_MIN; y < Y_MAX; y++) {
        const i32 index = (y - Y_MIN) * WIDTH + (-X_MIN);
        assert(index > 0 && index < (i32)SIZE);
        if (graph[index] == '*') {
        	graph[index] = INTERCEPT_CHAR;
        } else {
        	graph[index] = '#';
        }
    }

    // Draw x coordinate
    for (i32 x = X_MIN; x < X_MAX; x++) {
        const i32 index = -Y_MIN * WIDTH + (x - X_MIN);
        assert(index > 0 && index < (i32)SIZE);
        if (graph[index] == '*' || graph[index] == INTERCEPT_CHAR) {
        	graph[index] = INTERCEPT_CHAR;
        } else {
        	graph[index] = '#';
        }
    }

    // Print graph to stdio.
    for (char* it = graph; it != graph + SIZE; it += WIDTH) {
		for (usize j = 0; j < WIDTH ; j++) {
			const char c = it[j];
			if (opts.no_color) {
				switch (c) {
					case '#':
						printf("\033[31m");
						break;
					case '*':
						printf("\033[92m");
						break;
					case ' ':
						break;
					default:
						if (c == INTERCEPT_CHAR) {
							printf("\033[96m");
							break;
						}
						assert(0);
						break;
				}
				printf("%c\033[0m", c);
			} else {
				putchar(c);
			}
		}
		putchar('\n');
	}

	return 0;
}
