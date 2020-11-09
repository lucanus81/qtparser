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
  bool parse();

private:
//  using atom_ptr = std::unique_ptr<base_parsed_atom>;
//  using vector_atoms_ptr = std::vector<atom_ptr>;

  buffer_reader reader_;
  vector_atoms_ptr atoms_;

  vector_atoms_ptr parse_atoms(uint64_t atom_total_bytes);
  void verify_file_type() const;

  std::optional<atom_header_raw> read_atom_header();
  atom_ptr parse_base_atom(atom_header_raw const& header);
  atom_ptr parse_ftyp_atom(atom_header_raw const& header);
  atom_ptr parse_tkhd_atom(atom_header_raw const& header);
  atom_ptr parse_header_only_atom(atom_header_raw const& header);
};

#endif
