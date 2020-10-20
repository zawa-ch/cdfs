#ifndef __cdfs_datatype__
#define __cdfs_datatype__
#include <limits>
#include <array>
namespace zawa_ch::CDFS
{
	struct UInt128 final
	{
	private:
		typedef uintmax_t uint_value;
		typedef std::array<uint_value, 16 / sizeof(uint_value)> value_type;
		value_type _data;
		constexpr explicit UInt128(const value_type& data) : _data(data) {}
	public:
		constexpr UInt128() noexcept : _data() {}
		constexpr UInt128(const uintmax_t& value) noexcept : _data({value}) {};

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

		[[nodiscard]] constexpr bool Equals(const UInt128& other) const noexcept
		{
			for (size_t i = 0; i < _data.size(); i++)
			{
				if (_data[i] != other._data[i]) { return false; }
			}
			return true;
		}
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
	};

	enum class CDFSFrameTypes : uint32_t
	{
		HEAD = 0x48454144,
		FINF = 0x46494E46,
		DATA = 0x44415444,
		CONT = 0x434F4E54,
		META = 0x4D455441,
	};

	struct CDFSFrame;
	struct CDFSHEADFrame;
	struct CDFSFINFFrame;
	struct CDFSCONTFrame;

	struct CDFSFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		std::array<uint8_t, 240> data;
		uint32_t checksum;
	};

	struct CDFSHEADFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		uint32_t formatver;
		UInt128 count;
		std::array<char, 32> label;
		UInt128 size;
		std::array<uint8_t, 172> _res;
		uint32_t checksum;
	};

	struct CDFSFINFFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		std::array<uint8_t, 4> _res1;
		UInt128 count;
		std::array<uint8_t, 32> hash;
		UInt128 size;
		std::array<uint8_t, 172> _res2;
		uint32_t checksum;
	};

	struct CDFSCONTFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		std::array<uint8_t, 4> _res1;
		UInt128 current;
		std::array<uint8_t, 32> hash;
		std::array<uint8_t, 188> _res2;
		uint32_t checksum;
	};
}
#endif // __cdfs_datatype__