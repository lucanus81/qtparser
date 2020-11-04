#ifndef _FTYP_PARSED_ATOM_H
#define _FTYP_PARSED_ATOM_H

#include "base_parsed_atom.h"
#include <iostream>
#include <algorithm>

class ftype_parsed_atom : public base_parsed_atom
{
public:
	ftype_parsed_atom(uint64_t size, std::string const& type)
		: base_parsed_atom{size, type} {}

	ftype_parsed_atom& major_brand(std::string const& major) {
		major_brand_ = major;
		return *this;
	}
	
	ftype_parsed_atom& minor_version(uint32_t minor) {
		minor_ = minor;
		return *this;
	}

	ftype_parsed_atom& add_compatible_brand(std::string const& brand) {
		compatible_brands_.push_back(brand);
		return *this;
	}

  bool is_quicktime_file() const
  {
    static const std::string QUICKTIME_FORMAT{"qt  "};

    return major_brand_ == QUICKTIME_FORMAT ||
      std::any_of(compatible_brands_.cbegin(), compatible_brands_.cend(), 
        [](auto const& brand) { return brand == QUICKTIME_FORMAT; });
  }

	void print_atom_info() const override
	{
		std::cout <<"INFO: atom: size = " <<size_ <<" bytes, type = " <<type_ <<", major_brand = '" 
						  <<major_brand_ <<"', compatible brands { ";
		for (auto const& brand : compatible_brands_)
			std::cout <<"'" <<brand <<"' ";
		std::cout <<"}\n";
	}

private:
	std::string major_brand_;
	uint32_t minor_;
	std::vector<std::string> compatible_brands_;
};

#endif
