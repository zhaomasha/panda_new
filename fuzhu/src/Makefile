cppflags=
headers=-I ../../inc
libs=-lpanda -L ../../lib

bin_dir=bin

targets=../bin/input ../bin/input_new ../bin/input_random_edge ../bin/input_random_vertex ../bin/read_edge ../bin/read_edge_on_attr ../bin/read_vertex ../bin/read_vertex_number ../bin/read_vertex_random ../bin/read_all_vertex ../bin/read_edge_random

all: $(targets)


../bin/input: input.cpp
	mkdir -p ../bin
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/input_new: input_new.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/input_random_edge: input_random_edge.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/input_random_vertex: input_random_vertex.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/read_edge: read_edge.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/read_edge_on_attr: read_edge_on_attr.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/read_vertex: read_vertex.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/read_vertex_number: read_vertex_number.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/read_vertex_random: read_vertex_random.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^ 

../bin/read_all_vertex:read_all_vertex.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^

../bin/read_edge_random: read_edge_random.cpp
	g++ $(cppflags) $(headers) $(libs) -o $@ $^

.PHONY: clean

clean:
	rm $(targets)
