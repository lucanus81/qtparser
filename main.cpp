#include <iostream>
#include "command_line_parser.h"

int main(int argc, char** argv) {
	command_line_parser command_line;
	if (!command_line.parse(argc,argv))
		return -1;
	
	std::cout <<"Processing " <<command_line.filename() <<'\n';
  return 0;
}
