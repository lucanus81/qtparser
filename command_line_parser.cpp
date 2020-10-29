#include "command_line_parser.h"
#include <getopt.h>
#include <iostream>
#include <filesystem>

/**
 * Method used to show the help
 */
void command_line_parser::show_help() const
{
	std::cout <<"mov_reader [-h] [-f <input .mov file>]\n"; 
}

/**
 * General method for all parameters' validation
 */
bool command_line_parser::validate_parameters() const
{
	bool const input_file_exists = validate_filename();
	if (!input_file_exists)
		std::cerr <<filename() <<" does not exist\n";
	
	return input_file_exists;
}

/**
 * Method used to check weather the specified filename exists.
 */
bool command_line_parser::validate_filename() const
{
	std::filesystem::path const input_path{filename()};

	return std::filesystem::exists(input_path);
}

/**
 * Method used to parse argc/argv
 */
bool command_line_parser::parse(int argc, char** argv)
{
	if (argc == 1)
	{
		show_help();
		return false;
	}
	
	int opt{};
	while ((opt=getopt(argc, argv, ":hf:")) != -1)
	{
		switch (opt)
		{
			case 'f':
				filename_ = optarg;
				break;
			case 'h':
				show_help();
				return false;
			case ':':
				std::cerr <<"An option is required\n";
				show_help();
				return false;
			case '?':
				std::cerr <<"Unknown option " <<optopt <<'\n';
				return false;
		}
	}
	
	return validate_parameters();
}
