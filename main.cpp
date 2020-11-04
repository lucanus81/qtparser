#include <iostream>
#include "command_line_parser.h"
#include "buffer_reader.h"
#include "atom_parser.h"

int main(int argc, char** argv)
{
  command_line_parser command_line;
  if (!command_line.parse(argc,argv))
    return -1;

  atom_parser parser{command_line.filename()};
  parser.parse();

  return 0;
}
