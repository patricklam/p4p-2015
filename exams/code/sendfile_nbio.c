struct sendfile_state {
    int fd;            /* file being sent */
    int socket;        /* socket to send file over */
    char buf[BUFSIZE]; /* current chunk of file */
    int buf_len;       /* bytes in buffer; <= BUFSIZE */
    int buf_used;      /* bytes from buffer sent so far; <= m_buf_len */
    enum { IDLE, SENDING } state; /* what we're doing */
};
