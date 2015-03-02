// ECE 459 Lecture 14 live coding
// (corrected) example from midterm about fine-grained locking
// experiment with and without -DFINE_GRAINED_LOCKING
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct arg
{
    int i;
};

struct node
{
    struct node *left, *right;
    int key;
    int * data;
    pthread_mutex_t lock;
};

struct node * root;
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int find_and_increment(int key)
{
    struct node * n = root;
    while (n != NULL) {
        if (key == n->key) {
#ifdef FINE_GRAINED_LOCKING
            pthread_mutex_lock(&n->lock);
#else
            pthread_mutex_lock(&global_lock);
#endif
            (*n->data)++;
            int d = *n->data;
#ifdef FINE_GRAINED_LOCKING
            pthread_mutex_unlock(&n->lock);
#else
            pthread_mutex_unlock(&global_lock);
#endif
            return d;
        }
        if (key < n->key) 
            n = n->left;
        else if (key > n->key)
            n = n->right;
    }
    return -1;
}

struct node * make_new_node(int key) 
{
    struct node * newNode = malloc(sizeof (struct node));
    newNode->left = NULL; newNode->right = NULL;
    newNode->key = key;
    newNode->data = malloc(sizeof(int)); if (!newNode->data) perror("malloc");
    *newNode->data = 0;
    pthread_mutex_init(&newNode->lock, NULL);
    return newNode;
}

// not threadsafe
void add(int key)
{
    struct node * nprev = root, *n = root;
    while (n != NULL) {
        if (key == n->key) {
            return;
        }
        if (key < n->key) 
            { nprev = n; n = n->left; }
        else if (key > n->key)
            { nprev = n; n = n->right; }
    }
    //printf("adding key %d nprev->key %d at %xd, root %xd\n", key, nprev->key, nprev, root);
    if (key < nprev->key) {
        nprev->left = make_new_node(key);
    } else if (key > nprev->key) {
        nprev->right = make_new_node(key);
    }
}

void * do_searches(void * k)
{
    int key = ((struct arg *) k)->i;
    for (int i = 0; i < 500000; i++) {
        find_and_increment(key);
    }
}

void print_preorder(struct node * n) {
    printf("[%d", *n->data);
    if (n->left != NULL)
        { printf(" "); print_preorder(n->left); }
    if (n->right != NULL)
        { printf(" "); print_preorder(n->right); }
    printf("]");
}

int main(int argc, char * argv[])
{
    struct arg valid_key[4];
    pthread_t tids[4];

    root = malloc(sizeof (struct node));
    root->key = 50000; root->data = malloc(sizeof(struct node)); *root->data = 0;
    root->left = NULL; root->right = NULL;

    srand(1749L);
    for (int i = 0; i < 10000; i++) {
        valid_key[i%4].i = rand() % 100000;
        add(valid_key[i%4].i);
    }

    for (int i = 0; i < 4; i++) {
        pthread_create(&tids[i], NULL, do_searches, &valid_key[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(tids[i], NULL);
    }

    if (argc > 1) {
        print_preorder(root);
        printf("\n");
    }
}

#if 0
Note that the fine-grained locking version is almost 4x as fast.
You can also see that it has far fewer context switches.
The CPU utilization is slightly higher for the fine-grained locking case.
Indeed, it may be the case that my CPUs didn't even throttle up for the 
fine-grained locking.

plam@polya-wireless:~/459$ gcc -std=c99 -pthread fine-grained-locking.c -g -o fine-grained-locking
plam@polya-wireless:~/459$ perf stat ./fine-grained-locking

 Performance counter stats for './fine-grained-locking':

       1191.478341 task-clock                #    3.076 CPUs utilized          
            32,722 context-switches          #    0.027 M/sec                  
                23 cpu-migrations            #    0.019 K/sec                  
               419 page-faults               #    0.352 K/sec                  
     2,621,246,340 cycles                    #    2.200 GHz                     [82.81%]
     1,943,533,005 stalled-cycles-frontend   #   74.15% frontend cycles idle    [82.80%]
     1,220,096,424 stalled-cycles-backend    #   46.55% backend  cycles idle    [66.72%]
     1,440,802,841 instructions              #    0.55  insns per cycle        
                                             #    1.35  stalled cycles per insn [82.82%]
       312,537,230 branches                  #  262.310 M/sec                   [84.35%]
         3,362,841 branch-misses             #    1.08% of all branches         [83.52%]

       0.387305712 seconds time elapsed

plam@polya-wireless:~/459$ gcc -std=c99 -pthread fine-grained-locking.c -DFINE_GRAINED_LOCKING -o fine-grained-locking
plam@polya-wireless:~/459$ perf stat ./fine-grained-locking

 Performance counter stats for './fine-grained-locking':

        347.004351 task-clock                #    3.283 CPUs utilized          
                23 context-switches          #    0.066 K/sec                  
                 6 cpu-migrations            #    0.017 K/sec                  
               420 page-faults               #    0.001 M/sec                  
       694,466,395 cycles                    #    2.001 GHz                     [81.64%]
       503,785,406 stalled-cycles-frontend   #   72.54% frontend cycles idle    [83.86%]
       125,025,438 stalled-cycles-backend    #   18.00% backend  cycles idle    [67.24%]
       701,103,408 instructions              #    1.01  insns per cycle        
                                             #    0.72  stalled cycles per insn [84.00%]
       179,383,034 branches                  #  516.948 M/sec                   [83.86%]
            47,601 branch-misses             #    0.03% of all branches         [83.84%]

       0.105693026 seconds time elapsed
#endif
