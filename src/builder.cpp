//	zawa-ch/cdfs:/src/builder
//	Copyright 2020 zawa-ch.
//
#include "cdfs/builder.hpp"
using namespace zawa_ch::CDFS;

CDFSBuilder::CDFSBuilder() : label(), frameindex(), datasize(), wrotehead(), wrotefinf() {}
CDFSBuilder::CDFSBuilder(const std::string& label) : label(label), frameindex(), datasize(), wrotehead(), wrotefinf() {}

const std::string& CDFSBuilder::Label() const { return label; }
const UInt128& CDFSBuilder::FrameIndex() const { return frameindex; }
const UInt128& CDFSBuilder::DataSize() const { return datasize; }
void CDFSBuilder::WriteHEADFrame(std::ostream& stream) { WriteHEADFrame(stream, frameindex + 1, datasize); }
void CDFSBuilder::WriteHEADFrame(std::ostream& stream, const UInt128& framecount, const UInt128& datasize)
{
	///	書き込むCDFS開始フレーム
	CDFSHEADFrame frame = CDFSHEADFrame();
	// コンストラクタを明示的に呼び出し、内容をすべて0でフィルしておく
	frame.sequence() = 0U;
	frame.data_version() = CDFS::FormatVersion;
	frame.data_count() = framecount;
	// ボリュームラベルのコピー
	// データ境界を超えないようイテレータを使ってC/P
	{
		///	source iterator
		auto si = label.cbegin();
		///	destination iterator
		auto di = frame.data_label().begin();
		///	source end
		auto se = label.cend();
		///	destination end
		auto de = frame.data_label().end();
		while((si != se)&&(di != de)) { *(di++) = *(si++); }
	}
	frame.data_size() = datasize;
	frame.Validate();
	// ストリーム書き込み
	WriteToStream(stream, frame.Frame());
	// 開始フレーム書き込みフラグを立てる
	wrotehead = true;
	if (!wrotefinf) { ++frameindex; }
}
void CDFSBuilder::WriteFINFFrame(std::ostream& stream)
{
	// 開始フレーム書き込んでいない/終了フレーム書き込み済みの場合は何もせず処理終了
	if ((!wrotehead)||(wrotefinf)) { return; }
	///	書き込むCDFS終了フレーム
	CDFSFINFFrame frame = CDFSFINFFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data_count() = frameindex + 1;
	// TODO: チェックサムの仕様策定と実装
	// frame.data_hash;
	frame.data_size() = datasize;
	frame.Validate();
	// ストリーム書き込み
	WriteToStream(stream, frame.Frame());
	// 終了フレーム書き込みフラグを立てる
	wrotefinf = true;
}
void CDFSBuilder::WriteDATAFrame(std::ostream& stream, const ContainsType& data, const size_t& size)
{
	// 開始フレーム書き込んでいない/終了フレーム書き込み済みの場合は何もせず処理終了
	if ((!wrotehead)||(wrotefinf)) { return; }
	///	書き込むCDFSデータフレーム
	CDFSDATAFrame frame = CDFSDATAFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data() = data;
	frame.Validate();
	// ストリーム書き込み
	WriteToStream(stream, frame.Frame());
	if ((wrotehead)&&(!wrotefinf))
	{
		++frameindex;
		datasize += size;
	}
}
void CDFSBuilder::WriteCONTFrame(std::ostream& stream)
{
	// 開始フレーム書き込んでいない/終了フレーム書き込み済みの場合は何もせず処理終了
	if ((!wrotehead)||(wrotefinf)) { return; }
	///	書き込むCDFS継続フレーム
	CDFSCONTFrame frame = CDFSCONTFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data_current() = frameindex;
	// ボリュームラベルのコピー
	// データ境界を超えないようイテレータを使ってC/P
	{
		///	source iterator
		auto si = label.cbegin();
		///	destination iterator
		auto di = frame.data_label().begin();
		///	source end
		auto se = label.cend();
		///	destination end
		auto de = frame.data_label().end();
		while((si != se)&&(di != de)) { *(di++) = *(si++); }
	}
	frame.Validate();
	// ストリーム書き込み
	WriteToStream(stream, frame.Frame());
	if ((wrotehead)&&(!wrotefinf)) { ++frameindex; }
}

void CDFSBuilder::WriteToStream(std::ostream& stream, const CDFSFrame& frame)
{
	auto sentry = std::ostream::sentry(stream);
	if (bool(sentry))
	{
		stream.write((const std::ostream::char_type*)&frame, sizeof(CDFSFrame));
	}
}
