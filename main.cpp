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
	
	size_t count{0};
	while (reader.has_more_bytes()) {
		uint32_t atom_size{0};
		if (reader.read(atom_size))
			count += sizeof(atom_size);
		std::cout <<"atom_size = " <<atom_size <<std::endl;

		uint32_t type;
		if (reader.read(type))
			count += sizeof(type);
		union {
			uint32_t value;
			char name[4];
		} demagler{type};
		std::cout <<"atom_type = ";
		for (int i=3; i>=0; --i)
			std::cout <<demagler.name[i];
		std::cout <<std::endl;

		size_t buffer_size{atom_size - sizeof(atom_size) - sizeof(type)};
		auto buffer{ std::make_unique<char[]>(buffer_size) };
		if (reader.read(buffer, buffer_size))
			count += buffer_size;
		std::cout <<buffer_size <<std::endl;
	}

	std::cout <<"Total is " <<count <<" bytes\n";

//	uint32_t size=0;
//	if (reader.read(size))
//		std::cout <<size <<'\n';

//	for (unsigned int i=0; i<4; ++i) {
//		char type=0;
//		if (reader.read(type))
//			std::cout <<type <<'\n';
//	}

  return 0;
}
