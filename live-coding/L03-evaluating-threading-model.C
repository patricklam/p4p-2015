// compile with gcc -lpthread -o evaluating-threading-model -std=gnu99 evaluating-threading-model.c
// also run with perf stat -B
// and with taskset 0x01 to limit to one core

#include <stdlib.h>
#include <stdio.h>
#include <thread>

#define STATE_SIZE 64

void thename()
{
  int c = 0;
  int return_value;

  struct random_data* qq = static_cast<struct random_data*>(calloc(1,sizeof(struct random_data)));

  char* buf = static_cast<char *>(calloc(STATE_SIZE,sizeof(char)));
  initstate_r(69L,buf,STATE_SIZE,qq);

  for(int i = 0 ; i < (1<<28); i++)
  {
    random_r(qq,&return_value);
    c+= return_value;
  }
  printf("Thread returns c = %d\n",c);
}

int main()
{
  int c = 0;
  int return_value;

  struct random_data* qq = static_cast<struct random_data*>(calloc(1,sizeof(struct random_data)));

  char* buf = static_cast<char *>(calloc(STATE_SIZE,sizeof(char)));
  initstate_r(69L,buf,STATE_SIZE,qq);

  std::thread first(thename);

  for(int i = 0 ; i < (1<<28); i++)
  {
    random_r(qq,&return_value);
    c+= return_value;
  }
  printf("c is %d\n",c);

  first.join();

  return 0;
}
