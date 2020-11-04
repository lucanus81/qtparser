#ifndef _PARSED_ATOM_CONATINER_H
#define _PARSED_ATOM_CONATINER_H

#include "base_parsed_atom.h"
#include <iostream>
#include <vector>

class parsed_atom_container : public base_parsed_atom
{
public:
  parsed_atom_container(uint64_t size, std::string const& type)
    : base_parsed_atom{size, type} {}
  
  void print_atom_info() const override
  {
    std::cout <<"atom container: size = " <<size_ <<" bytes, type = " <<type_ <<"\n";
    for (auto&& child : children_)
      child->print_atom_info();
  }

  void add_child(std::unique_ptr<base_parsed_atom>&& atom)
  {
    children_.push_back(std::move(atom));
  }

private:
  std::vector<std::unique_ptr<base_parsed_atom>> children_;
};

#endif
