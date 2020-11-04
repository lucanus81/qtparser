#ifndef _ATOM_H
#define _ATOM_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <string>

union uint32_string_shared {
  uint32_t value_;
  uint8_t mnemonic_name_[4];
};

std::string mnemonic_to_string(uint32_string_shared const& value);

struct atom_header_raw
{
  static const uint32_t HAS_EXTENDED_SIZE{1};
  
  uint32_t size_;
  uint32_string_shared type_;
  uint64_t extended_size_;

  uint64_t size() const 
  { 
    return size_ == HAS_EXTENDED_SIZE ? extended_size_ : size_; 
  }

  std::string type() const
  {
    return mnemonic_to_string(type_);
  }

  bool is_non_container_type() const
  {
    static const std::unordered_set<std::string> non_container_types_{"ftyp", "free", "skip", "wide", "tkhd"};
    return non_container_types_.find(type()) != non_container_types_.cend();
  }

  bool is_container_type() const
  {
    static const std::unordered_set<std::string> container_types_{"moov", "trak", "mdia", "minf", "stbl"};
    return container_types_.find(type()) != container_types_.cend();
  }

  uint64_t remaining_size() const
  {
    return size_ == HAS_EXTENDED_SIZE
      ? extended_size_ - sizeof(size_) - sizeof(type_) - sizeof(extended_size_)
      : size_ - sizeof(size_) - sizeof(type_);
  }

  friend std::ostream& operator<<(std::ostream& os, atom_header_raw const& header)
  {
    os <<"size = " <<header.size() <<", type = " <<header.type();
    os <<", extended size = " <<(header.extended_size_ == HAS_EXTENDED_SIZE ? "yes" : "no");	
    return os;
  }
};

#endif
