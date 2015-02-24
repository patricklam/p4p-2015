// http://www.kegel.com/dkftpbench/nonblocking.html
#include <fcntl.h>

#define BUFSIZE 1024

void sendFile(const char *filename, int socket) {
    int fd, nread, nwrite, i;
    char buf[BUFSIZE];

    fd = open(filename, O_RDONLY);
    /* Send the file, one chunk at a time */
    do {
        /* Get one chunk of the file from disk */
        nread = read(fd, buf, BUFSIZE);
        if (nread == 0) {
            /* All done; close the file and the socket. */
            close(fd); close(socket);
            break;
        }
        /* Send the chunk */
        for (i=0; i<nread; i += nwrite) {
            /* write might not take it all in one call,
             * so we have to try until it's all written */
            nwrite = write(socket, buf + i, nread - i);
        }
    } while (1);
}
