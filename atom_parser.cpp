#include "atom_parser.h"
#include "ftyp_parsed_atom.h"
#include "header_only_parsed_atom.h"
#include "parsed_atom_container.h"
#include "tkhd_parsed_atom.h"

#include <memory>
#include <iostream>
#include <iterator>

/**
 * Read an atom header:
 * - 4 bytes: size
 * - 4 bytes: type
 */
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

/**
 * Method used to parse an "ftyp" atom
 */
atom_ptr atom_parser::parse_ftyp_atom(atom_header_raw const& header)
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

/**
 * Method used to parse a track header atom, used to extract info about
 * width and height of a single track.
 */
atom_ptr atom_parser::parse_tkhd_atom(atom_header_raw const& header)
{
  auto tkhd = std::make_unique<tkhd_parsed_atom>(header.size(), header.type());

  uint8_t version{};
  if (!reader_.read(version))
    return {};
  
  uint8_t flags[3]={};
  for (size_t i=0; i<sizeof(flags); ++i)
    if (!reader_.read(flags[i]))
      return {};
  
  uint32_t creation_time{};
  if (!reader_.read(creation_time))
    return {};

  uint32_t modification_time{};
  if (!reader_.read(modification_time))
    return {};

  uint32_t track_id{};
  if (!reader_.read(track_id))
    return {};
  tkhd->track_id(track_id);
  
  uint32_t reserved{};
  if (!reader_.read(reserved))
    return {};
  
  uint32_t duration{};
  if (!reader_.read(duration))
    return {};
  
  uint64_t reserver_8_bytes{};
  if (!reader_.read(reserver_8_bytes))
    return {};
  
  uint16_t layer{};
  if (!reader_.read(layer))
    return {};
  
  uint16_t alternate_group{};
  if (!reader_.read(alternate_group))
    return {};
  
  uint16_t volume{};
  if (!reader_.read(volume))
    return {};
  
  uint16_t reserved_2_bytes;
  if (!reader_.read(reserved_2_bytes))
    return {};
  
  const size_t matrix_size_in_bytes{36};
  auto matrix{ std::make_unique<char[]>(matrix_size_in_bytes) };
  if (!reader_.read(matrix, matrix_size_in_bytes))
    return {};
  
  uint32_t track_width{};
  if (!reader_.read(track_width))
    return {};
  tkhd->track_width(track_width >> 16);
  
  uint32_t track_height{};
  if (!reader_.read(track_height))
    return {};
  tkhd->track_height(track_height >> 16);

  return tkhd;
}

/**
 * Used to parse atoms that consist only of headers.
 * This function also skips any additional data that we do not want to parse.
 */
atom_ptr atom_parser::parse_header_only_atom(atom_header_raw const& header)
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

/**
 * Method used to parse a single atom. Based on each type, we call a different parser. 
 */
atom_ptr atom_parser::parse_base_atom(atom_header_raw const& header)
{
  if (header.type() == "ftyp")
    return parse_ftyp_atom(header);
  else
    if (header.type() == "free" || header.type() == "skip" || header.type() == "wide")
      return parse_header_only_atom(header);
    else
      if (header.type() == "tkhd")
        return parse_tkhd_atom(header);

	return {};
}

/**
 * Main method used to parse a MOV file.
 * We have pretty much 3 paths for this:
 * - we have an atom to parse
 * - we have an atom container
 * - we have an atom/container that needs to be skipped (in this case we simply ignore the data)
 */
vector_atoms_ptr atom_parser::parse_atoms(uint64_t atom_total_bytes)
{
  uint64_t bytes_read{0};
  vector_atoms_ptr current_atoms;

  while (bytes_read < atom_total_bytes)
  {
    std::optional<atom_header_raw> header{read_atom_header()};
    if (!header.has_value())
      return {};

    if (header.value().is_non_container_type()) 
    {
      auto parsed_atom=parse_base_atom(header.value());
      if (parsed_atom != nullptr) 
        current_atoms.push_back(std::move(parsed_atom));
    } else 
      if (header.value().is_container_type()) 
      {
        vector_atoms_ptr children = parse_atoms(header.value().remaining_size());
        auto container_atom = std::make_unique<parsed_atom_container>(header.value().size(), header.value().type());
        current_atoms.insert(current_atoms.end(), std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));
      } else 
        {
          auto unknown_atom = parse_header_only_atom(header.value());
          if (unknown_atom != nullptr)
            current_atoms.push_back(std::move(unknown_atom));
        }

    bytes_read += header.value().size();
  }

  return current_atoms;
}

/**
 * Method used to verify whether we have a real MOV file (based on Apple's documentation)
 */
void atom_parser::verify_file_type() const
{
  if (atoms_.size() == 0)
    return;
  
  ftype_parsed_atom *ftyp=dynamic_cast<ftype_parsed_atom*>(atoms_.front().get());
  if (ftyp == nullptr || !ftyp->is_quicktime_file())
      std::cout <<"WARN: the specified file doesn't seem to be a MOV file\n";
}

/**
 * Main function used to parse a MOV file
 */
bool atom_parser::parse()
{
  atoms_ = parse_atoms(reader_.size());
  std::cout <<"INFO: File Summary \n";
  verify_file_type();
  
  std::cout <<"\nINFO: Atoms Summary\n";
  for (auto&& a : atoms_)
    a->print_atom_info();

  return true;
}

