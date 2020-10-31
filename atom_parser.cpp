#include "atom_parser.h"
#include <memory>
#include <iostream>

std::optional<atom_header_raw> atom_parser::read_atom_header()
{
	uint32_t full_atom_size;
	if (!reader_.read(full_atom_size))
		return {};
	uint32_t atom_type;
	if (!reader_.read(atom_type, buffer_reader::BYTES_ORDER::DO_NOT_SWAP_BYTES))
		return {};
	
	atom_header_raw header{full_atom_size, atom_type, 0};
	if (full_atom_size == atom_header_raw::HAS_EXTENDED_SIZE)
	{
		uint64_t extended_size;
		if (!reader_.read(extended_size))
			return {};
		header.extended_size_ = extended_size;	
	}

	return header;
}

bool atom_parser::parse()
{
	while (reader_.has_more_bytes())
	{
		std::optional<atom_header_raw> header{read_atom_header()};
		if (!header.has_value())
			return false;
		
		char type[5]{};
		for (size_t i=0; i<4; ++i)
			type[i]=header.value().type_.mnemonic_name_[i];

		std::cout <<"type = " <<type 
		          <<", size = " <<header.value().size() 
							<<", data size = " <<header.value().remaining_size() <<'\n';
		auto buffer{ std::make_unique<char[]>(header.value().remaining_size()) };
		if (!reader_.read(buffer, header.value().remaining_size()))
			return false;
	}

	return true;
}
