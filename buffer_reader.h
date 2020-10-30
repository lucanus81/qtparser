#ifndef _BUFFER_READER_H
#define _BUFFER_READER_H

#include <fstream>
#include <filesystem>
#include <type_traits>
#include <utility>

struct buffer_reader {
	/**
	 * Constructor that opens a file in binary mode (read only)
	 */
	buffer_reader(std::string const& filename) {
		file_.open(filename, std::ios::binary);
		if (file_.is_open())
		{
			std::filesystem::path const path{filename};
			total_size_ = std::filesystem::file_size(path);
		}
	}

  /**
	 * Method that return the total file size
	 */
	size_t size() const { return total_size_; }
	
	/**
	 * This function is used to read a chunk of bytes and convert it into
	 * any numeric value. Keep in mind, that as per Apple's specification,
	 * all integers are stored in big-endian format.
	 */
	template <typename IntegerType, typename = std::enable_if_t<std::is_integral<IntegerType>::value>>
	bool read(IntegerType& value) {
		union {
			char buffer[sizeof(IntegerType)];
			IntegerType value;
		} buffer_chunk{};
		
		file_.read(static_cast<char*>(buffer_chunk.buffer), sizeof(buffer_chunk.buffer));
		swap_bytes(buffer_chunk.buffer, sizeof(buffer_chunk.buffer));

		value = buffer_chunk.value;
		return sizeof(value) == file_.gcount();
	}

private:
	void swap_bytes(char* buffer, size_t size)
	{
		for (size_t i=0; i<size/2; ++i)
			std::swap(buffer[i], buffer[size-1-i]);
	}

private:
	std::ifstream file_;
	size_t total_size_;
};

#endif
