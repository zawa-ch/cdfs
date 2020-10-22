//	cdfs/builder
//	Copyright 2020 zawa-ch.
//
#ifndef __cdfs_builder__
#define __cdfs_builder__
#include <array>
#include <iostream>
#include "cdfs.hpp"
namespace zawa_ch::CDFS
{
	///	CDFSデータを構築するための機能を提供します。
	class CDFSBuilder
	{
	public:
		typedef std::array<uint8_t, 240> ContainsType;
	private:
		std::string label;
		UInt128 frameindex;
		UInt128 datasize;
		bool wrotehead;
		bool wrotefinf;
	public:
		///	既定の設定で @a CDFSBuilder を初期化します。
		CDFSBuilder();
		///	CDFSボリュームラベルを付けて @a CDFSBuilder を初期化します。
		CDFSBuilder(const std::string& label);

		///	CDFSデータに付けられたCDFSボリュームラベルを取得します。
		const std::string& Label() const;
		///	書き込まれているCDFSデータのシーケンス番号を取得します。
		const UInt128& FrameIndex() const;
		///	これまでに書き込まれたCDFSデータの総サイズを取得します。
		const UInt128& DataSize() const;
		///	指定されたストリームに開始フレームを書き込みます。
		void WriteHEADFrame(std::ostream& stream);
		///	指定されたストリームに開始フレームを書き込みます。
		void WriteHEADFrame(std::ostream& stream, const UInt128& framecount, const UInt128& datasize);
		///	指定されたストリームに終了フレームを書き込みます。
		void WriteFINFFrame(std::ostream& stream);
		///	指定されたストリームにデータフレームを書き込みます。
		void WriteDATAFrame(std::ostream& stream, const ContainsType& data, const size_t& size = 240U);
		///	指定されたストリームに継続フレームを書き込みます。
		void WriteCONTFrame(std::ostream& stream);

		/// 指定されたストリームに指定されたCDFSフレームを書き込みます。
		static void WriteToStream(std::ostream& stream, const CDFSFrame& frame);
	};
}
#endif // __cdfs_builder__
