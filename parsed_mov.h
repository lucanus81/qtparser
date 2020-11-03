#ifndef _PARSED_MOV_H
#define _PARSED_MOV_H

#include <memory>
#include <vector>

#include "base_parsed_atom.h"

class parsed_mov
{
public:
		
private:
	std::vector<std::unique_ptr<base_parsed_atom>> atoms_;
};

#endif
