#ifndef _COMMAND_LINE_PARSER_H
#define _COMMAND_LINE_PARSER_H

#include <string>

/**
 * A very simple command line parser
 */
struct command_line_parser 
{
	bool parse(int argc, char** argv);
	std::string filename() const
	{
		return filename_;
	}

private:
	std::string filename_;

	void show_help() const;
	bool validate_parameters() const;
	bool validate_filename() const;
};

#endif
