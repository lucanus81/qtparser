#ifndef _BUFFER_READER_H
#define _BUFFER_READER_H

#include <fstream>
#include <filesystem>
#include <type_traits>
#include <utility>
#include <memory>

struct buffer_reader {
	enum class BYTES_ORDER { SWAP_BYTES, DO_NOT_SWAP_BYTES }; 
	
  /**
	 * Constructor that opens a file in binary mode (read only)
	 */
	buffer_reader(std::string const& filename) {
		file_.open(filename, std::ios::binary);
		if (file_.is_open())
		{
			std::filesystem::path const path{filename};
			total_size_ = std::filesystem::file_size(path);
			bytes_left_ = total_size_;
		}
	}

	buffer_reader(buffer_reader const&) = delete;
	buffer_reader(buffer_reader&&) = delete;
	buffer_reader& operator=(buffer_reader const&) = delete;
	buffer_reader& operator=(buffer_reader&&) = delete;

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
	bool read(IntegerType& value, BYTES_ORDER operation=BYTES_ORDER::SWAP_BYTES) {
		union {
			char buffer[sizeof(IntegerType)];
			IntegerType value;
		} buffer_chunk{};
		
		file_.read(static_cast<char*>(buffer_chunk.buffer), sizeof(buffer_chunk.buffer));
		if (operation == BYTES_ORDER::SWAP_BYTES)
			swap_bytes(buffer_chunk.buffer, sizeof(buffer_chunk.buffer));

		value = buffer_chunk.value;
		
		size_t bytes_read = file_.gcount();
		bytes_left_ -= bytes_read;
		return sizeof(value) == bytes_read;
	}

	bool read(std::unique_ptr<char[]>& buffer, size_t buffer_size)
	{
		file_.read(buffer.get(), buffer_size);
		
		size_t bytes_read = file_.gcount();
		bytes_left_ -= bytes_read;
		return buffer_size == file_.gcount();
	}

	bool has_more_bytes() const
	{
		return bytes_left_ > 0;
	}

  size_t bytes_left() const
  {
    return bytes_left_;
  }

private:
	void swap_bytes(char* buffer, size_t size)
	{
		for (size_t i=0; i<size/2; ++i)
			std::swap(buffer[i], buffer[size-1-i]);
	}

private:
	std::ifstream file_;
	size_t total_size_{0};;
	size_t bytes_left_{0};
};

#endif
