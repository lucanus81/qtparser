#ifndef _BASE_PARSED_ATOM_H
#define _BASE_PARSED_ATOM_H

#include <string>
#include <memory>
#include <vector>

class base_parsed_atom
{
public:
  virtual ~base_parsed_atom() {}
  virtual void print_atom_info() const = 0;

protected:
  uint64_t size_{0};
  std::string type_;

protected:
  base_parsed_atom(uint64_t size, std::string const& type)
    : size_{size}, type_{type}
  {
  }
};
  
using atom_ptr = std::unique_ptr<base_parsed_atom>;
using vector_atoms_ptr = std::vector<atom_ptr>;

#endif
