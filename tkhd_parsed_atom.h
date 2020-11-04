#ifndef _TKHD_PARSED_ATOM_H
#define _TKHD_PARSED_ATOM_H

#include "base_parsed_atom.h"
#include <iostream>

class tkhd_parsed_atom : public base_parsed_atom
{
public:
  tkhd_parsed_atom(uint64_t size, std::string const& type)
    : base_parsed_atom{size, type} {}

  tkhd_parsed_atom& track_id(uint32_t track_id) { track_id_ = track_id; return *this; }
  tkhd_parsed_atom& track_width(uint32_t track_width) { track_width_ = track_width; return *this; }
  tkhd_parsed_atom& track_height(uint32_t track_height) { track_height_ = track_height; return *this; }

  void print_atom_info() const override
  {
    std::cout <<"INFO: atom: size = " <<size_ <<" bytes, type = "	
      <<type_ <<", track_id = " <<track_id_ <<", width = " <<track_width_ <<" pixels, height = "
      <<track_height_ <<" pixels\n";
  }

private:
  uint32_t track_id_;
  uint32_t track_width_;
  uint32_t track_height_;
};

#endif
