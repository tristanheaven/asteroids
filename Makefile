CXXFLAGS ?= -O2 -march=native -ggdb
CXXFLAGS += -Ineural2d -std=c++11 -pthread -Wall -DDISABLE_WEBSERVER

CFLAGS ?= -O2 -march=native -ggdb
CFLAGS += -std=c99 -Wall

all: neural metrics .data

neural: neural.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< neural2d/neural2d-core.cpp

metrics: metrics.c
	$(CC) $(CFLAGS) -o $@ $< -lfftw3

inputdata:
	scripts/run_metrics.sh > inputdata.txt

.data: ALCDEF_ALL
	scripts/squash_alcdef.sh
	ruby scripts/select_alcdef.rb
	touch .data

clean:
	rm -f metrics neural
