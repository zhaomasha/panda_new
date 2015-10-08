#!/usr/bin/env bash
g++ -o read_edge read_edge.cpp -I ../inc/ -lpanda -L ../lib/
g++ -o read_vertex read_vertex.cpp -I ../inc/ -lpanda -L ../lib/
g++ -o read_vertex_number read_vertex_number.cpp -I ../inc/ -lpanda -L ../lib/
g++ -o read_edge_on_attr read_edge_on_attr.cpp -I ../inc/ -lpanda -L ../lib/
g++ -o input input.cpp -I ../inc/ -lpanda -L ../lib/
g++ -g -o read_vertex_random read_vertex_random.cpp -I ../inc/ -lpanda -L ../lib/
g++ -o input_random_edge input_random_edge.cpp -I ../inc/ -lpanda -L ../lib/
