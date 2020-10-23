//	zawa-ch/cdfs:/examples/cdfsbuilder
//	Copyright 2020 zawa-ch.
//
#include <array>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include "cdfs/builder.hpp"
using namespace zawa_ch::CDFS;

///	使用法を表示する
void usage()
{
	std::cout << "\tUsage: <program> filename" << std::endl;
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
	///	読み込みファイルのストリーム
	auto source_stream = std::ifstream(source_filename.data());
	source_stream.exceptions(std::ios_base::badbit);
	{
		///	読み込みストリームのsentryオブジェクト
		auto source_sentry = std::istream::sentry(source_stream);
		if (!bool(source_sentry))
		{
			std::cerr << "E: Can't open source file" << std::endl;
			return 1;
		}
		///	書き込みファイルのパス
		auto dest_filename = std::string(argv[1]) + ".cdfs";
		///	書き込みファイルのストリーム
		auto dest_stream = std::ofstream(dest_filename);
		if (!bool(dest_stream))
		{
			std::cerr << "E: Can't open destination file" << std::endl;
			return 1;
		}
		///	CDFSデータビルダー
		auto builder = CDFSBuilder();
		// 開始フレーム書き込み
		builder.WriteHEADFrame(dest_stream);
		// 読み込みストリームから読み込み可能な限りデータを読み込む
		while(source_stream.good())
		{
			///	ストリームから読み込んだデータ
			auto buffer = CDFSBuilder::ContainsType();
			source_stream.read((std::istream::char_type*)buffer.data(), buffer.size());
			///	ストリームから読み込まれたデータのサイズ
			auto readsize = source_stream.gcount();
			// データフレーム書き込み
			builder.WriteDATAFrame(dest_stream, buffer, size_t(readsize));
		}
		// 終了フレーム書き込み
		builder.WriteFINFFrame(dest_stream);
		// 書き込みストリームの最初にシーク
		dest_stream.seekp(std::streampos(0), std::ios_base::beg);
		// シークに成功した場合開始フレームを再度書き込み
		// フレーム数とデータサイズの情報を書き込む
		if (!dest_stream.fail())
		{
			builder.WriteHEADFrame(dest_stream);
		}
		dest_stream.clear();
	}
	return 0;
}
