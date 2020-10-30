#include <iostream>
#include "command_line_parser.h"
#include "buffer_reader.h"

int main(int argc, char** argv) {
	command_line_parser command_line;
	if (!command_line.parse(argc,argv))
		return -1;
	
	std::cout <<"Processing " <<command_line.filename() <<'\n';
	buffer_reader reader{command_line.filename()};
	std::cout <<"File size is " <<reader.size() <<'\n';
	
	uint32_t size=0;
	if (reader.read(size))
		std::cout <<size <<'\n';

	for (unsigned int i=0; i<4; ++i) {
		char type=0;
		if (reader.read(type))
			std::cout <<type <<'\n';
	}

  return 0;
}
