CC=g++
CXXFLAGS=-std=c++17 -g

SOURCE=$(wildcard *.cpp)
OBJECTS=$(SOURCE:.cpp=.o)
EXECUTABLE=mov_parser

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

