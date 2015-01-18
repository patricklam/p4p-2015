CXXFLAGS?=-std=c++11 -D_GNU_SOURCE -Wall -MP -MMD -O2 -g

default: all

all: server_sync server_select server_poll server_epoll

OBJ_SYNC = http_server_base.o http_server_sync.o driver_sync.o
OBJ_SELECT = http_server_base.o http_server_select.o driver_select.o
OBJ_POLL = http_server_base.o http_server_poll.o driver_poll.o
OBJ_EPOLL = http_server_base.o http_server_epoll.o driver_epoll.o
OBJECTS = $(OBJ_SYNC) $(OBJ_SELECT) $(OBJ_POLL) $(OBJ_EPOLL)

driver_sync.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DSYNC -c $^ -o $@

driver_select.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DSELECT -c $^ -o $@

driver_poll.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DPOLL -c $^ -o $@

driver_epoll.o: http_server_driver.cc
	$(CXX) $(CXXFLAGS) -DEPOLL -c $^ -o $@


server_sync: $(OBJ_SYNC)
	$(CXX) -DSYNC $^ -o $@

server_select: $(OBJ_SELECT)
	$(CXX) -DSELECT $^ -o $@

server_poll: $(OBJ_POLL)
	$(CXX) -DPOLL $^ -o $@

server_epoll: $(OBJ_EPOLL)
	$(CXX) -DEPOLL $^ -o $@


clean:
	rm *.o *.d
