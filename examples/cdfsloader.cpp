//	zawa-ch/cdfs:/examples/cdfsloader
//	Copyright 2020 zawa-ch.
//
#include <array>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include "cdfs/loader.hpp"
using namespace zawa_ch::CDFS;

///	使用法を表示する
void usage()
{
	std::cout << "\tUsage: <program> filename.cdfs" << std::endl;
}

int main(int argc, char const *argv[])
{
	// 引数の数のチェック
	if (argc < 2)
	{
		std::cerr << "E: Too few arguments" << std::endl;
		usage();
		return 2;
	}
	///	読み込みファイルのパス
	auto source_filename = std::string_view(argv[1]);
	if(source_filename.rfind(".cdfs") != (source_filename.size() - 5U))
	{
		std::cerr << "E: Source file name MUST ends with \".cdfs\"" << std::endl;
		return 1;
	}
	///	読み込みファイルのストリーム
	auto source_stream = std::ifstream(source_filename.data());
	if (!source_stream.good())
	{
		std::cerr << "E: Can't open source file" << std::endl;
		return 1;
	}
	source_stream.exceptions(std::ios_base::badbit);
	///	書き込みファイルのパス
	auto dest_filename = std::string(source_filename.cbegin(), source_filename.cend() - 5U);
	///	書き込みファイルのストリーム
	auto dest_stream = std::ofstream(dest_filename);
	if (!dest_stream.good())
	{
		std::cerr << "E: Can't open destination file" << std::endl;
		return 1;
	}
	///	CDFSデータローダー
	auto cdfsloader = CDFSLoader();
	while(cdfsloader.ReadNext(source_stream))
	{
		// CDFSデータの検証に失敗した場合警告
		if (!cdfsloader.IsValidData())
		{
			std::cerr << "W: Frame " << uint64_t(cdfsloader.FrameIndex()) << " validation failed" << std::endl;
		}
		switch (cdfsloader.GetFrame()->frametype)
		{
		// 開始フレーム
		case CDFSFrameTypes::HEAD:
		{
			auto frame = CDFSHEADFrame(cdfsloader.GetFrame().value());
			std::cout << "-> HEAD Frame" << std::endl;
			std::cout << "Label: " << std::string(frame.data_label().data()) << std::endl;
			break;
		}
		// データフレーム
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
		// 終了フレーム
		case CDFSFrameTypes::FINF:
		{
			std::cout << "-> FINF Frame" << std::endl;
			break;
		}
		// 継続フレーム
		case CDFSFrameTypes::CONT:
		{
			auto frame = CDFSCONTFrame(cdfsloader.GetFrame().value());
			std::cout << "-> CONT Frame" << std::endl;
			std::cout << "Label: " << std::string(frame.data_label().data()) << std::endl;
			break;
		}
		// フレーム種類を特定できなかった場合
		default:
		{
			std::cerr << "W: Can't recognized frame type" << std::endl;
			break;
		}
		}
	}
	// CDFSデータの整合性チェック
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
