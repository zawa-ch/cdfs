//	zawa-ch/cdfs:/src/loader
//	Copyright 2020 zawa-ch.
//
#include "cdfs/loader.hpp"
using namespace zawa_ch::CDFS;

CDFSLoader::CDFSLoader()
	: buffer(), label(), framecount(), frameindex(), datasize(), dataindex(), readhead(), readfinf(), fault()
{}

bool CDFSLoader::HasHEAD() const { return readhead; }
bool CDFSLoader::HasFINF() const { return readfinf; }
const UInt128& CDFSLoader::FrameIndex() const { return frameindex; }
const UInt128& CDFSLoader::FrameCount() const { return framecount; }
const UInt128& CDFSLoader::DataIndex() const { return dataindex; }
const UInt128& CDFSLoader::DataSize() const { return datasize; }
bool CDFSLoader::ReadNext(std::istream& stream)
{
	// 終了フレームが読み込まれている場合は何もしない
	if (readfinf) { return false; }
	// シーケンス番号送り
	if (buffer.has_value()) { ++frameindex; }
	// ストリームからフレーム取得
	buffer = ReadFrameFromStream(stream);
	// 取得に失敗した場合は処理終了
	if (!buffer.has_value()) { return false; }
	// フレームの検証に失敗した場合は検証失敗のフラグを立てて処理終了
	if (!IsValidData())
	{
		fault = true;
		return buffer.has_value();
	}
	// 開始フレームの読み込み
	if ((!readhead)&&(CDFSHEADFrame::IsHEADFrame(*buffer)))
	{
		auto header = CDFSHEADFrame(*buffer);
		// フォーマットバージョン確認
		// (対応していないフォーマットバージョンのCDFSデータが来た場合の処理は未規定)
		if (IsVersionCompatible(header))
		{
			label = std::string(header.data_label().data());
			framecount = header.data_count();
			datasize = header.data_size();
			readhead = true;
		}
	}
	// 終了フレームの読み込み
	if ((readhead)&&(!readfinf)&&(CDFSFINFFrame::IsFINFFrame(*buffer)))
	{
		auto finf = CDFSFINFFrame(*buffer);
		if (framecount == 0) { framecount = finf.data_count(); }
		if (datasize == 0) { datasize = finf.data_size(); }
		readfinf = true;
	}
	// データフレームの読み込み
	if ((readhead)&&(!readfinf)&&(CDFSDATAFrame::IsDATAFrame(*buffer)))
	{
		dataindex += 240U;
	}
	return true;
}
bool CDFSLoader::HasValue() const noexcept { return buffer.has_value(); }
bool CDFSLoader::IsValidData() const noexcept
{
	if(!HasValue()) { return false; }
	return buffer->IsValid()&&VerifySequence(*buffer, uint64_t(frameindex));
}
std::vector<uint8_t> CDFSLoader::GetData(const size_t& size) const
{
	// フレームを取得していない場合は空のオブジェクトを渡す
	if (!HasValue()) { return std::vector<uint8_t>(); }
	// データフレームが来ている場合はデータフレームの内容を渡す
	if (CDFSDATAFrame::IsDATAFrame(*buffer))
	{
		auto result = CDFSDATAFrame(*buffer);
		// 要求されたデータの大きさとデータフレーム内のデータの大きさを比較、いずれか小さい領域をコピー
		if ((result.data().cbegin() + size) <= (result.data().cend()))
		{
			return std::vector<uint8_t>(result.data().cbegin(), result.data().cbegin() + size);
		}
		else
		{
			return std::vector<uint8_t>(result.data().cbegin(), result.data().cend());
		}
	}
	// データフレームではない場合は空のオブジェクトを渡す
	return std::vector<uint8_t>();
}
const std::optional<CDFSFrame>& CDFSLoader::GetFrame() const { return buffer; }
std::optional<bool> CDFSLoader::CheckIntegrity() const
{
	// 終了フレームが来ていない場合は検証できないためnulloptを渡す
	if (!readfinf) { return std::nullopt; }
	return (!fault)&&((frameindex+1) == framecount);
}

std::optional<CDFSFrame> CDFSLoader::ReadFrameFromStream(std::istream& stream)
{
	if (stream.good())
	{
		auto result = CDFSFrame();
		stream.read((std::istream::char_type*)&result, sizeof(CDFSFrame));
		// CDFSフレーム長が正しく読み込まれた場合のみデータを返す
		if (stream.gcount() == sizeof(CDFSFrame)) { return result; }
	}
	return std::nullopt;
}
bool CDFSLoader::IsVersionCompatible(const CDFSHEADFrame& frame)
{
	return frame.data_version() <= CDFS::FormatVersion;
}
bool CDFSLoader::VerifySequence(const CDFSFrame& frame, const uint64_t& seq)
{
	return frame.sequence == seq;
}
