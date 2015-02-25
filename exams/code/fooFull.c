#include <stdlib.h>
#include <stdio.h>

struct elem {
    int data;
    struct elem * next;
};

int getFifth(struct elem * e) {
    for (int i = 0; i < 5; i++) {
        if (e == NULL) return -1;
        e = e->next;
    }
    if (e != NULL) return e->data; else return -1;
}

void visit(struct elem * root) {
    printf("5th element is %d.\n", getFifth(root));
    printf("5th element is still %d.\n", getFifth(root));
}

int main() {
    struct elem * r = malloc(sizeof(struct elem));
    struct elem * e = r;
    for (int i = 0; i < 6; i++) {
        e->next = malloc(sizeof(struct elem));
        e->data = i;
        e = e->next;
    }
    visit(r);
}
