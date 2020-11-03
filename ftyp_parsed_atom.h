#ifndef _FTYP_PARSED_ATOM_H
#define _FTYP_PARSED_ATOM_H

#include <iostream>

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

	void print_atom_info() const
	{
		std::cout <<size_ <<", " <<type_ <<", " <<major_brand_ <<", { ";
		for (auto const& brand : compatible_brands_)
			std::cout <<brand <<' ';
		std::cout <<"}\n";
	}

private:
	std::string major_brand_;
	uint32_t minor_;
	std::vector<std::string> compatible_brands_;
};

#endif
