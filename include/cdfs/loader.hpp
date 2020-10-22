//	cdfs/loader
//	Copyright 2020 zawa-ch.
//
#ifndef __cdfs_loader__
#define __cdfs_loader__
#include <array>
#include <vector>
#include <optional>
#include <iostream>
#include "cdfs.hpp"
namespace zawa_ch::CDFS
{
	///	CDFSデータを読み出すための機能を提供します。
	class CDFSLoader
	{
	private:
		std::optional<CDFSFrame> buffer;
		std::string label;
		UInt128 framecount;
		UInt128 frameindex;
		UInt128 datasize;
		UInt128 dataindex;
		bool readhead;
		bool readfinf;
		bool fault;
	public:
		///	@a CDFSLoader を初期化します。
		CDFSLoader();

		///	開始フレームが読み込まれたかを取得します。
		bool HasHEAD() const;
		///	終了フレームが読み込まれたかを取得します。
		bool HasFINF() const;
		///	現在読み込まれているフレームのインデックスを取得します。
		const UInt128& FrameIndex() const;
		///	現在読み込んでいるCDFSデータの総フレーム数を取得します。
		const UInt128& FrameCount() const;
		///	現在までに読み込まれたCDFSデータの総サイズを取得します。
		const UInt128& DataIndex() const;
		///	現在読み込んでいるCDFSデータの総サイズを取得します。
		const UInt128& DataSize() const;
		///	次のフレームを指定されたストリームから読み出します。
		bool ReadNext(std::istream& stream);
		///	現在このオブジェクトがフレームを保持しているかを取得します。
		bool HasValue() const noexcept;
		///	現在保持しているフレームがCDFSフレームとして有効であるか取得します。
		bool IsValidData() const noexcept;
		///	現在保持しているフレームに含まれるデータを取得します。
		std::vector<uint8_t> GetData(const size_t& size = 240U) const;
		///	現在保持しているフレームを取得します。
		const std::optional<CDFSFrame>& GetFrame() const;
		///	読み込まれたCDFSデータの整合性をチェックします。
		std::optional<bool> CheckIntegrity() const;

		///	指定されたストリームからフレームを取得します。
		static std::optional<CDFSFrame> ReadFrameFromStream(std::istream& stream);
		///	ヘッダのCDFSフォーマットバージョンがこのライブラリで対応しているかを取得します。
		static bool IsVersionCompatible(const CDFSHEADFrame& frame);
		///	フレームのシーケンス番号を検証します。
		static bool VerifySequence(const CDFSFrame& frame, const uint64_t& seq);
	};
}
#endif // __cdfs_loader__
