#ifndef _BASE_ATOMS_H
#define _BASE_ATOMS_H

class base_atom
{
protected:
	uint64_t size_{0};
	char type_[5]{0};

protected:
	base_atom(uint64_t size, char (&buffer)[4])
	{
		size_=size;
		for (size_t i=0; i<4; ++i)
			type_[i]=buffer[i];
	}
};

#endif
