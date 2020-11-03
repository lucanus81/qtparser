#include "atom_parser.h"
#include "ftyp_parsed_atom.h"
#include "header_only_parsed_atom.h"

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

std::unique_ptr<base_parsed_atom> atom_parser::parse_ftyp_atom(atom_header_raw const& header)
{
	auto ftyp = std::make_unique<ftype_parsed_atom>(header.size(), header.type()); 
	
	uint32_t major_raw{};
	if (!reader_.read(major_raw, buffer_reader::BYTES_ORDER::DO_NOT_SWAP_BYTES))
		return {};
	ftyp->major_brand(mnemonic_to_string(uint32_string_shared{major_raw}));

	uint32_t minor_version{};
	if (!reader_.read(minor_version))
		return {};	
	ftyp->minor_version(minor_version);
	
	size_t total_brands_size{header.remaining_size() - sizeof(major_raw) - sizeof(minor_version)};
	size_t r{total_brands_size % sizeof(uint32_t)};
	if (r != 0)
		return {};
	
	size_t total_brands{total_brands_size / sizeof(uint32_t)};
	for (size_t i=0; i<total_brands; ++i)
	{
		uint32_t brand{};
		if (!reader_.read(brand, buffer_reader::BYTES_ORDER::DO_NOT_SWAP_BYTES))
			return {};
		ftyp->add_compatible_brand(mnemonic_to_string(uint32_string_shared{brand}));
	}

	return ftyp;
}

std::unique_ptr<base_parsed_atom> atom_parser::parse_header_only_atom(atom_header_raw const& header)
{
	auto header_only = std::make_unique<header_only_parsed_atom>(header.size(), header.type());

	size_t unused_data{header.remaining_size()};
	if (unused_data > 0)
	{
		auto buffer{ std::make_unique<char[]>(header.remaining_size()) };
		if (!reader_.read(buffer, header.remaining_size()))
			return {};
	}

	return header_only;
}

std::unique_ptr<base_parsed_atom> atom_parser::parse_base_atom(atom_header_raw const& header)
{
	if (header.type() == "ftyp")
		return parse_ftyp_atom(header);
	else
		if (header.type() == "free" || header.type() == "skip" || header.type() == "wide")
			return parse_header_only_atom(header);

	return {};
}

bool atom_parser::parse()
{
	while (reader_.has_more_bytes())
	{
		std::optional<atom_header_raw> header{read_atom_header()};
		if (!header.has_value())
			return false;
		
		std::cout <<"DEBUG: " <<header.value() <<' ';
		std::cout <<"DEBUG: " <<", non-container-type = " <<header.value().is_non_container_type() <<'\n';
		if (header.value().is_non_container_type())
		{
			auto parsed_atom=parse_base_atom(header.value());
			if (parsed_atom != nullptr)
				parsed_atom->print_atom_info();
		} else {
			auto buffer{ std::make_unique<char[]>(header.value().remaining_size()) };
			if (!reader_.read(buffer, header.value().remaining_size()))
				return false;
		}
	}

	return true;
}


