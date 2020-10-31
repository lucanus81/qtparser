#ifndef _ATOM_PARSER_H
#define _ATOM_PARSER_H

#include "buffer_reader.h"
#include "atom.h"
#include <optional>

class atom_parser
{
public:
	atom_parser(std::string const& filename)
		: reader_{filename} {}
	atom_parser(atom_parser const&) = delete;
	atom_parser(atom_parser&&) = delete;
	atom_parser& operator=(atom_parser const&) = delete;
	atom_parser& operator=(atom_parser&&) = delete;

	bool parse();

private:
	buffer_reader reader_;

	std::optional<atom_header_raw> read_atom_header();
};

#endif
