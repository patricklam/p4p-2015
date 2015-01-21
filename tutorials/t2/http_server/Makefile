CXXFLAGS?=-std=c++11 -D_GNU_SOURCE -Wall -MP -MMD -g -pthread
LDFLAGS?=-pthread
default: all

TARGETS = server_sync server_select server_poll server_epoll server_epollet server_multithread
OBJ_SYNC = http_server_base.o http_server_sync.o driver_sync.o
OBJ_SELECT = http_server_base.o http_server_select.o driver_select.o
OBJ_POLL = http_server_base.o http_server_poll.o driver_poll.o
OBJ_EPOLL = http_server_base.o http_server_epoll.o driver_epoll.o
OBJ_EPOLLET = http_server_base.o http_server_epollet.o driver_epoll.o
OBJ_MT = http_server_base.o http_server_multithread.o driver_multithread.o
OBJECTS = $(OBJ_SYNC) $(OBJ_SELECT) $(OBJ_POLL) $(OBJ_EPOLL) $(OBJ_MT) $(OBJ_EPOLLET)

all: $(TARGETS)

driver_sync.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DSYNC -c http_server_driver.cc -o $@

driver_select.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DSELECT -c http_server_driver.cc -o $@

driver_poll.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DPOLL -c http_server_driver.cc -o $@

driver_epoll.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DEPOLL -c http_server_driver.cc -o $@

driver_multithread.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DMT -c http_server_driver.cc -o $@


server_sync: $(OBJ_SYNC)
	$(CXX) $(LDFLAGS) -DSYNC $^ -o $@

server_select: $(OBJ_SELECT)
	$(CXX) $(LDFLAGS) -DSELECT $^ -o $@

server_poll: $(OBJ_POLL)
	$(CXX) $(LDFLAGS) -DPOLL $^ -o $@

server_epoll: $(OBJ_EPOLL)
	$(CXX) $(LDFLAGS) -DEPOLL $^ -o $@

server_epollet: $(OBJ_EPOLLET)
	$(CXX) $(LDFLAGS) -DEPOLL $^ -o $@

server_multithread: $(OBJ_MT)
	$(CXX) $(LDFLAGS) -DMT $^ -o $@


-include ${OBJECTS:%.o=%.d}

clean:
	rm *.o *.d $(TARGETS)
