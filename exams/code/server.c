void respond(int fd, int count, char * payload) {
    char * response = malloc(count);
    memcpy(response, payload, count);
    write(fd, response, count);
}
