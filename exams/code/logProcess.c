#include <stdlib.h>

struct logEntry {
    int zipcode;
};

struct histogramEntry {
    int zipcode;
    int num;
};

struct histogramEntry * countZips(struct logEntry * l, int start, int count) {
    struct histogramEntry * results = malloc(100000*sizeof(struct histogramEntry));
    for (int i = start; i < count; i++) {
        results[l[i].zipcode].num++;
    }
    return results;
}
