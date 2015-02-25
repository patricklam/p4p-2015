struct sendfile_struct * sendfile (char * fn, int socket) {
    struct sendfile_struct * s = malloc(sizeof(sendfile_struct));
    setNonblocking(socket);
    s.fd = open(fn, O_RDONLY);
    s.socket = socket;
    s.buf_len = s.buf_used = 0;
    s.state = SENDING;
}

int handle_io(struct sendfile_struct s) {
    int nread, nwrite;

    if (s.state == IDLE) return 2;

    if (s.buf_used >= s.buf_len) {
        s.buf_len = read(s.fd, s.buf, BUF_SIZE);
        if (s.buf_len == 0) {
            close(s.fd); close (s.socket);
            s.state = IDLE;
            return 1;
        }

        s.buf_used = 0;
    }
    
    nwrite = write(s.socket, s.buf + s.buf_used, s.buf_len - s.buf_used);
    s.buf_used += nwrite;
    return 0;
}
