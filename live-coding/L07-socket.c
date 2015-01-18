// ECE 459 live coding: level- vs edge-triggered epoll.
// Change the definition of ev.events to observe different behaviour.
// Use nc -l -p 1701 to set up a server, type there, watch here.
// Thanks Andrew Tinits.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netdb.h>

#define MAX_EVENTS 10

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serv_addr;
  char * buf = malloc(1048576);

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    abort();
  serv_addr.sin_port = htons(1701);

  connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
  struct epoll_event ev;
   // ************** use or don't use EPOLLET here **********************
  ev.events = EPOLLIN | EPOLLET;
  ev.data.ptr = NULL;
  int epfd = epoll_create1(0);
  epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

  struct epoll_event* events =
    calloc(sizeof(struct epoll_event), MAX_EVENTS);

  while (1) {
    epoll_wait(epfd, events, MAX_EVENTS, -1);
    sleep(1);
    int n = read(sockfd, buf, 5);
    printf("n = %d\n", n);
    printf("buf = %5s\n", buf);
  }
}
