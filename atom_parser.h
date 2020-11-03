#ifndef _ATOM_PARSER_H
#define _ATOM_PARSER_H

#include "buffer_reader.h"
#include "atom.h"
#include "base_parsed_atom.h"
#include <optional>
#include <memory>
#include <vector>

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
	std::vector<std::unique_ptr<base_parsed_atom>> atoms_;

	std::optional<atom_header_raw> read_atom_header();
	std::unique_ptr<base_parsed_atom> parse_base_atom(atom_header_raw const& header);
	std::unique_ptr<base_parsed_atom> parse_ftyp_atom(atom_header_raw const& header);
};

#endif
