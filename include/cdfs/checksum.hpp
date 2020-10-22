//	cdfs/checksum
//	Copyright 2020 zawa-ch.
//
#ifndef __cdfs_checksum__
#define __cdfs_checksum__
#include <cstdint>
#include <array>
namespace zawa_ch::CDFS
{
	///	CRC32チェックサムの計算を行います。
	class CRC32
	{
	private:
		///	@a CRC32 の計算用のデータセットです。
		struct Dataset final
		{
		public:
			///	CRC32 テーブル。
			std::array<uint32_t, 256> table;
			///	データセットの初期化を行います。
			Dataset();
		};
		///	@a CRC32 の計算用のデータセットです。
		static Dataset data;

		///	ハッシュ計算値。
		uint32_t curr;

	public:
		///	既定の設定でこのオブジェクトを初期化します。
		CRC32() noexcept;
		///	初期値を指定してこのオブジェクトを初期化します。
		CRC32(const uint32_t& init) noexcept;

		///	指定されたデータをハッシュの一部に追加します。
		void Push(const uint8_t& value) noexcept;
		///	指定されたデータをハッシュの一部に追加します。
		void Push(const std::initializer_list<uint8_t>& list) noexcept;
		///	指定されたデータをハッシュの一部に追加します。
		void Push(const uint8_t* begin, const uint8_t* end) noexcept;
		///	指定されたデータをハッシュの一部に追加します。
		template<size_t length>
		void Push(const std::array<uint8_t, length>& array) noexcept { for(const auto& item: array) { Push(item); } }
		///	計算されたダイジェスト値を取得します。
		uint32_t GetValue() const noexcept;
	};
}
#endif // __cdfs_checksum__