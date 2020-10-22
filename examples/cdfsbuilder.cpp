#include <array>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include "cdfs/builder.hpp"
using namespace zawa_ch::CDFS;

void usage()
{
	std::cout << "\tUsage: <program> filename" << std::endl;
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cerr << "E: Too few arguments" << std::endl;
		usage();
		return 2;
	}
	auto source_filename = std::string_view(argv[1]);
	auto source_stream = std::ifstream(source_filename.data());
	source_stream.exceptions(std::ios_base::badbit);
	{
		auto source_sentry = std::istream::sentry(source_stream);
		if (!bool(source_sentry))
		{
			std::cerr << "E: Can't open source file" << std::endl;
			return 1;
		}
		auto dest_filename = std::string(argv[1]) + ".cdfs";
		auto dest_stream = std::ofstream(dest_filename);
		if (!bool(dest_stream))
		{
			std::cerr << "E: Can't open destination file" << std::endl;
			return 1;
		}
		auto builder = CDFSBuilder();
		builder.WriteHEADFrame(dest_stream);
		while(source_stream.good())
		{
			auto buffer = CDFSBuilder::ContainsType();
			source_stream.read((std::istream::char_type*)buffer.data(), buffer.size());
			auto readsize = source_stream.gcount();
			builder.WriteDATAFrame(dest_stream, buffer, size_t(readsize));
		}
		builder.WriteFINFFrame(dest_stream);
		dest_stream.seekp(std::streampos(0), std::ios_base::beg);
		if (!dest_stream.fail())
		{
			builder.WriteHEADFrame(dest_stream);
		}
		dest_stream.clear();
	}
	return 0;
}
