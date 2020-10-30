#ifndef _ATOM_H
#define _ATOM_H

#include <cstdint>
#include <cstring>

struct atom_header_raw
{
	uint32_t size_;
	union {
		uint32_t value_;
		uint8_t mnemonic_name_[4];
	} type_;
	uint64_t extended_size_;
};

struct qt_atom_container_header
{
	uint8_t reserved[10]{0};
	uint16_t lock_count_;
};

struct qt_atom_header
{
	static const uint8_t ROOT_ATOM[]={'s','e','a','n'};
	static const uint32_t ROOT_ATOM_ID{1};
	static const uint16_t NO_CHILDREN{0};

	uint32_t size_;
	union {
		uint32_t value_;
		uint8_t mnemonic_name_[4];
	} type_;
	uint32_t atom_id_;
	uint16_t reserved_16bits_{0};
	uint16_t child_count_;
	uint32_t reserved_32bits_{0};

	bool is_root() const {
		return !std::memcmp(&type_.mnemonic_name_, &ROOT_ATOM, sizeof(ROOT_ATOM)) &&
			atom_id_ == ROOT_ATOM_ID;
	}

	bool is_leaf() const
	{
		return child_count_ == NO_CHILDREN;
	}
};

#endif
