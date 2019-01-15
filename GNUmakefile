CXXFLAGS += -Wall -O0 -ggdb -std=c++14
CFLAGS = -Wall -O0 -ggdb
#CXXFLAGS += -I/usr/include
LDFLAGS += -L/usr/lib64 -lstdc++ -lpthread

VPATH= ./

all: test csprint_unit_test

test: test.o csprint.o
csprint_unit_test: csprint_unit_test.o csprint.o /usr/lib/libgtest.a /usr/lib/libgmock.a
