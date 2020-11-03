#include "atom.h"

std::string mnemonic_to_string(uint32_string_shared const& value)
{
		return std::string{reinterpret_cast<const char*>(&value.mnemonic_name_), 
		                   sizeof(value.mnemonic_name_)};
}

