cppflags=-g -rdynamic
headers=-I inc

bin_dir=bin

server_source=src/panda_master.cpp src/panda_util.cpp src/panda_metadata.cpp src/panda_split_method.cpp
slave_source=src/panda_slave.cpp src/panda_subgraph.cpp src/panda_graph.cpp src/panda_graph_set.cpp src/panda_split_method.cpp src/panda_util.cpp 
client_source=src/panda_client.cpp src/panda_split_method.cpp src/panda_util.cpp

all: bin/panda_server bin/panda_slave lib/libpanda.so

bin/panda_server: $(server_source)
	mkdir -p bin
	g++ $(cppflags) $(headers) -o $@ $^ -lzmq -lpthread

bin/panda_slave: $(slave_source)
	g++ $(cppflags) $(headers) -o $@ $^ -lzmq -lpthread

lib/libpanda.so: $(client_source)
	mkdir -p lib
	g++ -shared -fPIC $(cppflags) $(headers) -o $@ $^ -lzmq -lpthread

.PHONY: clean

clean:
	rm -fr bin lib 
