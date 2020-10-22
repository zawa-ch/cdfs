#include <array>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include "cdfs/loader.hpp"
using namespace zawa_ch::CDFS;

void usage()
{
	std::cout << "\tUsage: <program> filename.cdfs" << std::endl;
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
	if(source_filename.rfind(".cdfs") != (source_filename.size() - 5U))
	{
		std::cerr << "E: Source file name MUST ends with \".cdfs\"" << std::endl;
		return 1;
	}
	auto source_stream = std::ifstream(source_filename.data());
	if (!source_stream.good())
	{
		std::cerr << "E: Can't open source file" << std::endl;
		return 1;
	}
	source_stream.exceptions(std::ios_base::badbit);
	auto dest_filename = std::string(source_filename.cbegin(), source_filename.cend() - 5U);
	auto dest_stream = std::ofstream(dest_filename);
	if (!dest_stream.good())
	{
		std::cerr << "E: Can't open destination file" << std::endl;
		return 1;
	}
	auto cdfsloader = CDFSLoader();
	while(cdfsloader.ReadNext(source_stream))
	{
		if (!cdfsloader.IsValidData())
		{
			std::cerr << "W: Frame " << uint64_t(cdfsloader.FrameIndex()) << " validation failed" << std::endl;
		}
		switch (cdfsloader.GetFrame()->frametype)
		{
		case CDFSFrameTypes::HEAD:
		{
			auto frame = CDFSHEADFrame(cdfsloader.GetFrame().value());
			std::cout << "-> HEAD Frame" << std::endl;
			std::cout << "Label: " << std::string(frame.data_label().data()) << std::endl;
			break;
		}
		case CDFSFrameTypes::DATA:
		{
			auto data = cdfsloader.GetData();
			if (cdfsloader.DataSize() <= cdfsloader.DataIndex())
			{
				dest_stream.write((const std::ostream::char_type*)data.data(), std::streamsize(size_t(cdfsloader.DataSize()%240)));
			}
			else
			{
				dest_stream.write((const std::ostream::char_type*)data.data(), std::streamsize(240));
			}
			break;
		}
		case CDFSFrameTypes::FINF:
		{
			std::cout << "-> FINF Frame" << std::endl;
			break;
		}
		case CDFSFrameTypes::CONT:
		{
			auto frame = CDFSCONTFrame(cdfsloader.GetFrame().value());
			std::cout << "-> CONT Frame" << std::endl;
			std::cout << "Label: " << std::string(frame.data_label().data()) << std::endl;
			break;
		}
		default:
		{
			std::cerr << "W: Can't recognized frame type" << std::endl;
			break;
		}
		}
	}
	if (cdfsloader.CheckIntegrity().value_or(false))
	{
		std::cout << "Complete" << std::endl;
	}
	else
	{
		std::cerr << "W: Integrity check failed." << std::endl;
	}
	return 0;
}
