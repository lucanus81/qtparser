#ifndef HEADER_ONLY_PARSED_ATOM_H
#define HEADER_ONLY_PARSED_ATOM_H

#include "base_parsed_atom.h"
#include <iostream>

class header_only_parsed_atom : public base_parsed_atom
{
public:
	header_only_parsed_atom(uint64_t size, std::string const& type)
		: base_parsed_atom{size, type} {}

	void print_atom_info() const override
	{
		std::cout <<"atom: size = " <<size_ <<"bytes, type = " <<type_ <<"\n";
	}
};

#endif
