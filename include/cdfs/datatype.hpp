//	cdfs/datatype
//	Copyright 2020 zawa-ch.
//
#ifndef __cdfs_datatype__
#define __cdfs_datatype__
#include <limits>
#include <array>
#include "checksum.hpp"
namespace zawa_ch::CDFS
{
	///	128ビットの符号なし整数を表します。
	struct UInt128 final
	{
	private:
		///	内部でデータを表現するために用いる符号なし整数型。
		typedef uintmax_t uint_value;
		///	内部でデータを表現するために用いる型。
		typedef std::array<uint_value, 16 / sizeof(uint_value)> value_type;
		value_type _data;
		///	内部表現型から @a UInt128 のオブジェクトを作成します。
		constexpr explicit UInt128(const value_type& data) : _data(data) {}
	public:
		///	空の @a UInt128 のオブジェクトを作成します。
		constexpr UInt128() noexcept : _data() {}
		///	@a uintmax_t 型を @a UInt128 に変換します。
		constexpr UInt128(const uintmax_t& value) noexcept : _data({value}) {}

		[[nodiscard]] constexpr UInt128 operator+() const noexcept { return *this; }
		[[nodiscard]] constexpr UInt128 operator-() const noexcept { return ~*this; }
		[[nodiscard]] constexpr UInt128 operator+(const UInt128& other) const noexcept
		{
			bool c = false;
			UInt128 result;
			for (size_t i = 0; i < _data.size(); i++)
			{
				result._data[i] = _data[i] + (c?1U:0U);
				c = (c && (std::numeric_limits<uint_value>::max() == result._data[i])) || ((std::numeric_limits<uint_value>::max()-other._data[i]) < result._data[i]);
				result._data[i] += other._data[i];
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator-(const UInt128& other) const noexcept
		{
			bool b = false;
			UInt128 result;
			for (size_t i = 0; i < _data.size(); i++)
			{
				result._data[i] = _data[i] - (b?1U:0U);
				b = (_data[i] < (b?1U:0U)) || (result._data[i] < other._data[i]);
				result._data[i] -= other._data[i];
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator*(const UInt128& other) const noexcept
		{
			UInt128 result;
			for (size_t i = 0; i < 128; i++)
			{
				if ((other & (UInt128(1)<<i)) != 0)
				{
					result += (*this) << i;
				}
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator/(const UInt128& other) const noexcept
		{
			size_t w = 0;
			for (size_t i = 0; i < 128; i++)
			{
				if (((other << i) & (UInt128(1U) << 127)) != 0) { w = i; break; }
			}
			UInt128 result;
			UInt128 surplus = *this;
			for (size_t i = w; i < 128; i--)
			{
				UInt128 div = other << i;
				if (div <= surplus)
				{
					result |= UInt128(1) << i;
					surplus -= div;
				}
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator%(const UInt128& other) const noexcept
		{
			size_t w = 0;
			for (size_t i = 0; i < 128; i++)
			{
				if (((other << i) & (UInt128(1U) << 127)) != 0) { w = i; }
			}
			UInt128 result;
			UInt128 surplus = *this;
			for (size_t i = w; i < 128; i--)
			{
				UInt128 div = other << i;
				if (div <= surplus)
				{
					result |= UInt128(1) << i;
					surplus -= div;
				}
			}
			return surplus;
		}
		[[nodiscard]] constexpr UInt128 operator~() const noexcept
		{
			UInt128 result;
			for (size_t i = 0; i < _data.size(); i++)
			{
				result._data[i] = ~_data[i];
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator|(const UInt128& other) const noexcept
		{
			UInt128 result;
			for (size_t i = 0; i < _data.size(); i++)
			{
				result._data[i] = _data[i] | other._data[i];
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator&(const UInt128& other) const noexcept
		{
			UInt128 result;
			for (size_t i = 0; i < _data.size(); i++)
			{
				result._data[i] = _data[i] & other._data[i];
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator^(const UInt128& other) const noexcept
		{
			UInt128 result;
			for (size_t i = 0; i < _data.size(); i++)
			{
				result._data[i] = _data[i] ^ other._data[i];
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator>>(const size_t& count) const noexcept
		{
			size_t shiftindex = count / (sizeof(uint_value) * 8U);
			size_t shiftbit = count % (sizeof(uint_value) * 8U);
			UInt128 result;
			for (size_t i = 0U; (i < _data.size()) && ((i + shiftindex) < _data.size()); i++)
			{
				if ((shiftbit != 0U) && (i + shiftindex + 1U) < _data.size()) { result._data[i] |= _data[i + shiftindex + 1U] << ((sizeof(uint_value) * 8U) - shiftbit); }
				result._data[i] |= _data[i + shiftindex] >> shiftbit;
			}
			return result;
		}
		[[nodiscard]] constexpr UInt128 operator<<(const size_t& count) const noexcept
		{
			size_t shiftindex = count / (sizeof(uint_value) * 8U);
			size_t shiftbit = count % (sizeof(uint_value) * 8U);
			UInt128 result;
			for (size_t i = shiftindex; i < _data.size(); i++)
			{
				if ((shiftbit != 0U) && (0U < (i - shiftindex))) { result._data[i] |= _data[i - shiftindex - 1U] >> ((sizeof(uint_value) * 8U) - shiftbit); }
				result._data[i] |= _data[i - shiftindex] << shiftbit;
			}
			return result;
		}

		[[nodiscard]] constexpr bool operator!() const noexcept { return *this == 0U; }
		[[nodiscard]] constexpr bool operator||(const UInt128& other) const noexcept { return (*this != 0U) || (other != 0U); }
		[[nodiscard]] constexpr bool operator&&(const UInt128& other) const noexcept { return (*this != 0U) && (other != 0U); }

		constexpr UInt128& operator+=(const UInt128& other) noexcept { return *this = *this + other; }
		constexpr UInt128& operator-=(const UInt128& other) noexcept { return *this = *this - other; }
		constexpr UInt128& operator*=(const UInt128& other) noexcept { return *this = *this * other; }
		constexpr UInt128& operator|=(const UInt128& other) noexcept { return *this = *this | other; }
		constexpr UInt128& operator&=(const UInt128& other) noexcept { return *this = *this & other; }
		constexpr UInt128& operator^=(const UInt128& other) noexcept { return *this = *this ^ other; }
		constexpr UInt128& operator>>=(const size_t& count) noexcept { return *this = *this >> count; }
		constexpr UInt128& operator<<=(const size_t& count) noexcept { return *this = *this << count; }

		constexpr UInt128& operator++() noexcept { return *this = *this + 1; }
		constexpr UInt128& operator++(int) noexcept { UInt128 result = *this; ++(*this); return *this; }
		constexpr UInt128& operator--() noexcept { return *this = *this - 1; }
		constexpr UInt128& operator--(int) noexcept { UInt128 result = *this; --(*this); return *this; }

		///	このオブジェクトが他のオブジェクトと等しいか比較します。
		[[nodiscard]] constexpr bool Equals(const UInt128& other) const noexcept
		{
			for (size_t i = 0; i < _data.size(); i++)
			{
				if (_data[i] != other._data[i]) { return false; }
			}
			return true;
		}
		///	このオブジェクトと他のオブジェクトの大小関係を比較します。
		[[nodiscard]] constexpr int Compare(const UInt128& other) const noexcept
		{
			for (size_t i = _data.size() - 1; i < _data.size(); i--)
			{
				if (other._data[i] < _data[i]) { return 1; }
				if (_data[i] < other._data[i]) { return -1; }
			}
			return 0;
		}
		[[nodiscard]] constexpr bool operator==(const UInt128& other) const noexcept { return Equals(other); }
		[[nodiscard]] constexpr bool operator!=(const UInt128& other) const noexcept { return !Equals(other); }
		[[nodiscard]] constexpr bool operator<(const UInt128& other) const noexcept { return Compare(other) < 0;}
		[[nodiscard]] constexpr bool operator>=(const UInt128& other) const noexcept { return Compare(other) >= 0;}
		[[nodiscard]] constexpr bool operator>(const UInt128& other) const noexcept { return Compare(other) > 0;}
		[[nodiscard]] constexpr bool operator<=(const UInt128& other) const noexcept { return Compare(other) <= 0;}

		constexpr explicit operator uint64_t() const noexcept { return uint64_t(_data[0]); }
		constexpr explicit operator uint32_t() const noexcept { return uint32_t(_data[0]); }
		constexpr explicit operator uint16_t() const noexcept { return uint16_t(_data[0]); }
		constexpr explicit operator uint8_t() const noexcept { return uint8_t(_data[0]); }
	};

	///	CDFSフレームの種類。
	enum class CDFSFrameTypes : uint32_t
	{
		HEAD = 0x43444653,
		FINF = 0x46494E46,
		DATA = 0x44415444,
		CONT = 0x434F4E54,
		META = 0x4D455441,
	};

	///	CDFSフレームの基本型です。
	struct CDFSFrame final
	{
	public:
		///	シーケンス番号。
		uint64_t sequence;
		///	CDFSフレームの種類。
		CDFSFrameTypes frametype;
		///	CDFSフレームの内容。
		std::array<uint8_t, 240> data;
		///	CRC32 チェックサム。
		uint32_t checksum;

		///	CRC32チェックサムを計算し、このオブジェクトに適用します。
		void Validate();
		///	CRC32チェックサムを計算し、オブジェクト内のチェックサムが一致しているか検証します。
		bool IsValid() const;
	};

	///	開始フレーム(CDFS)のシグネチャを持つCDFSフレームです。
	struct CDFSHEADFrame final
	{
	private:
		CDFSFrame frame;
	public:
		///	空の @a CDFSHEADFrame を作成します。
		CDFSHEADFrame();
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSHEADFrame(const CDFSFrame& frame);
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSHEADFrame(CDFSFrame&& frame);

		///	データを保持している @a CDFSFrame を取得します。
		const CDFSFrame& Frame() const;
		///	このフレームのシーケンス番号を取得します。
		uint64_t& sequence();
		///	このフレームのシーケンス番号を取得します。
		const uint64_t& sequence() const;
		///	このヘッダーが持つCDFSデータバージョンを取得します。
		uint32_t& data_version();
		///	このヘッダーが持つCDFSデータバージョンを取得します。
		const uint32_t& data_version() const;
		///	このヘッダーが持つCDFSフレーム数を取得します。
		UInt128& data_count();
		///	このヘッダーが持つCDFSフレーム数を取得します。
		const UInt128& data_count() const;
		///	このヘッダーが持つCDFSラベルを取得します。
		std::array<char, 32>& data_label();
		///	このヘッダーが持つCDFSラベルを取得します。
		const std::array<char, 32>& data_label() const;
		///	このヘッダーが持つCDFSデータの総サイズを取得します。
		UInt128& data_size();
		///	このヘッダーが持つCDFSデータの総サイズを取得します。
		const UInt128& data_size() const;

		///	CRC32チェックサムを計算し、このオブジェクトに適用します。
		void Validate();
		///	CRC32チェックサムを計算し、オブジェクト内のチェックサムが一致しているか検証します。
		bool IsValid() const;

		/// 指定された @a CDFSFrame が開始フレームであるかを取得します。
		static bool IsHEADFrame(const CDFSFrame& frame);
	};

	///	終了フレーム(FINF)のシグネチャを持つCDFSフレームです。
	struct CDFSFINFFrame final
	{
	private:
		CDFSFrame frame;
	public:
		///	空の @a CDFSFINFFrame を作成します。
		CDFSFINFFrame();
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSFINFFrame(const CDFSFrame& frame);
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSFINFFrame(CDFSFrame&& frame);

		///	データを保持している @a CDFSFrame を取得します。
		const CDFSFrame& Frame() const;
		///	このフレームのシーケンス番号を取得します。
		uint64_t& sequence();
		///	このフレームのシーケンス番号を取得します。
		const uint64_t& sequence() const;
		///	このフッターが持つCDFSフレーム数を取得します。
		UInt128& data_count();
		///	このフッターが持つCDFSフレーム数を取得します。
		const UInt128& data_count() const;
		///	このフッターが持つCDFSデータのハッシュ値を取得します。
		std::array<uint8_t, 32>& data_hash();
		///	このフッターが持つCDFSデータのハッシュ値を取得します。
		const std::array<uint8_t, 32>& data_hash() const;
		///	このフッターが持つCDFSデータの総サイズを取得します。
		UInt128& data_size();
		///	このフッターが持つCDFSデータの総サイズを取得します。
		const UInt128& data_size() const;

		///	CRC32チェックサムを計算し、このオブジェクトに適用します。
		void Validate();
		///	CRC32チェックサムを計算し、オブジェクト内のチェックサムが一致しているか検証します。
		bool IsValid() const;

		/// 指定された @a CDFSFrame が終了フレームであるかを取得します。
		static bool IsFINFFrame(const CDFSFrame& frame);
	};

	///	データフレーム(DATA)のシグネチャを持つCDFSフレームです。
	struct CDFSDATAFrame final
	{
	private:
		CDFSFrame frame;
	public:
		///	空の @a CDFSDATAFrame を作成します。
		CDFSDATAFrame();
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSDATAFrame(const CDFSFrame& frame);
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSDATAFrame(CDFSFrame&& frame);

		///	データを保持している @a CDFSFrame を取得します。
		const CDFSFrame& Frame() const;
		///	このフレームのシーケンス番号を取得します。
		uint64_t& sequence();
		///	このフレームのシーケンス番号を取得します。
		const uint64_t& sequence() const;
		///	このフレームが保持しているデータを取得します。
		std::array<uint8_t, 240>& data();
		///	このフレームが保持しているデータを取得します。
		const std::array<uint8_t, 240>& data() const;

		///	CRC32チェックサムを計算し、このオブジェクトに適用します。
		void Validate();
		///	CRC32チェックサムを計算し、オブジェクト内のチェックサムが一致しているか検証します。
		bool IsValid() const;

		/// 指定された @a CDFSFrame がデータフレームであるかを取得します。
		static bool IsDATAFrame(const CDFSFrame& frame);
	};

	///	継続フレーム(CONT)のシグネチャを持つCDFSフレームです。
	struct CDFSCONTFrame final
	{
	private:
		CDFSFrame frame;
	public:
		///	空の @a CDFSCONTFrame を作成します。
		CDFSCONTFrame();
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSCONTFrame(const CDFSFrame& frame);
		///	@a CDFSFrame をこの型に変換します。
		///	@exception
		explicit CDFSCONTFrame(CDFSFrame&& frame);

		///	データを保持している @a CDFSFrame を取得します。
		const CDFSFrame& Frame() const;
		///	このフレームのシーケンス番号を取得します。
		uint64_t& sequence();
		///	このフレームのシーケンス番号を取得します。
		const uint64_t& sequence() const;
		///	このフレームの完全なシーケンス番号を取得します。
		UInt128& data_current();
		///	このフレームの完全なシーケンス番号を取得します。
		const UInt128& data_current() const;
		///	このヘッダーが持つCDFSラベルを取得します。
		std::array<char, 32>& data_label();
		///	このヘッダーが持つCDFSラベルを取得します。
		const std::array<char, 32>& data_label() const;

		///	CRC32チェックサムを計算し、このオブジェクトに適用します。
		void Validate();
		///	CRC32チェックサムを計算し、オブジェクト内のチェックサムが一致しているか検証します。
		bool IsValid() const;

		/// 指定された @a CDFSFrame が継続フレームであるかを取得します。
		static bool IsCONTFrame(const CDFSFrame& frame);
	};
}
#endif // __cdfs_datatype__
