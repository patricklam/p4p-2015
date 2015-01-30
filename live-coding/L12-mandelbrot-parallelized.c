// ECE 459 Live Coding demo (Patrick Lam)
// original version of mandelbrot example due to Jon Eyolfson

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

#define WIDTH 1400
#define HEIGHT 800

int * output_array;
int iterations = 1000;
struct arg{
    int offset, stride;
};

void inMandelbrot(int h, int w) {
    double x0, y0, x, xtemp, y;
    int iteration;
    /* scale i to [-2.5, 1.0] */
    x0 = 3.5*((double)w)/((double) WIDTH - 1.0) - 2.5;
    /* scale j to [-1.0, 1.0] */
    y0 = 2.0*((double)h)/((double)HEIGHT - 1.0) - 1.0;

    x = 0.0, y = 0.0;

    iteration = 0;
    while ((x*x + y*y < 4.0) && (iteration < iterations)) {
        xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        ++iteration;
    }
    if (iteration == iterations)
        output_array[h*WIDTH+w] = 1;
}

void * doThread(void * arg) {
    struct arg * a = (struct arg *) arg;
    for (int h = a->offset; h < HEIGHT; h += a->stride) {
        for (int w = 0; w < WIDTH; ++w) {
            inMandelbrot(h, w);
        }
    }
    return 0;
}

#define NUM_THREADS 4

int main(int argc, char *argv[])
{
    {
        int c;
        while ((c = getopt (argc, argv, "i:")) != -1) {
            switch (c) {
            case 'i':
                //iterations = atoi(optarg);
                break;
            default:
                return EXIT_FAILURE;
            }
        }
    }
    if (iterations <= 0) {
        printf("%s: option missing or requires an argument > 0 -- 'i'\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    output_array = (int *)calloc(HEIGHT * WIDTH, sizeof(int));

    pthread_t tids[NUM_THREADS];
    struct arg args[NUM_THREADS];
    for (int i = 0; i < 4; i++) {
        args[i].offset = i; args[i].stride = 4;
        pthread_create(&tids[i], NULL, doThread, &args[i]);
    }
    for (int i = 0; i < 4; i++)
        pthread_join(tids[i], NULL);

    FILE* pgmFile = fopen("output.pgm", "wb");
    if (pgmFile == NULL) {
        fprintf(stderr, "Failed to open pgm file\n");
        return EXIT_FAILURE;
    }
    fprintf(pgmFile, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int h = 0; h < HEIGHT; ++h) {
        for (int w = 0; w < WIDTH; ++w) {
            if (output_array[h*WIDTH+w]) {
                fputc(255, pgmFile);
            }
            else {
                fputc(0, pgmFile);
            }
        }
    }
    fclose(pgmFile);

    return EXIT_SUCCESS;
}
