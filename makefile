CC=g++
CXXFLAGS=-std=c++17
#$CXXFLAGS=-std=c++17 -g -fsanitize=address -fno-omit-frame-pointer

SOURCE=$(wildcard *.cpp)
OBJECTS=$(SOURCE:.cpp=.o)
EXECUTABLE=mov_parser

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

