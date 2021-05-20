/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_SYSTEM_BASE_16_IPP
#define LIBBITCOIN_SYSTEM_BASE_16_IPP

#include <algorithm>
#include <bitcoin/system/assert.hpp>

namespace libbitcoin {
namespace system {

template <size_t Size>
bool decode_base16(byte_array<Size>& out, const std::string& in)
{
    data_chunk data(Size);
    if (decode_base16(data, in) && data.size() == Size)
    {
        std::copy(data.begin(), data.end(), out.begin());
        return true;
    }

    return false;
}

template <size_t Size>
bool decode_hash(byte_array<Size>& out, const std::string& in)
{
    data_chunk data(Size);
    if (decode_base16(data, in.data()) && data.size() == Size)
    {
        std::reverse_copy(data.begin(), data.end(), out.begin());
        return true;
    }

    return false;
}

template <size_t Size>
std::string base16_string(const char(&string)[Size])
{
    return to_string(base16_chunk(string));
}

template <size_t Size>
data_chunk base16_chunk(const char(&string)[Size])
{
    static_assert((Size - 1u) % 2u == 0, "odd number of hexidecimal bytes");

    data_chunk out;
    decode_base16(out, string);
    return out;
}

template <size_t Size>
byte_array<(Size - 1u) / 2u> base16_array(const char(&string)[Size])
{
    static_assert((Size - 1u) % 2u == 0, "odd number of hexidecimal bytes");

    byte_array<(Size - 1u) / 2u> out;
    if (!decode_base16(out, string))
        out.fill(0);

    return out;
}

template <size_t Size>
byte_array<(Size - 1u) / 2u> base16_hash(const char(&string)[Size])
{
    static_assert((Size - 1u) % 2u == 0, "odd number of hexidecimal bytes");

    byte_array<(Size - 1u) / 2u> out;
    if (!decode_hash(out, string))
        out.fill(0);

    return out;
}

// DEPRECATED: use base16_array (renamed).
template <size_t Size>
byte_array<(Size - 1u) / 2u> base16_literal(const char(&string)[Size])
{
    return base16_array(string);
}

// DEPRECATED: use base16_hash (renamed).
template <size_t Size>
byte_array<(Size - 1u) / 2u> hash_literal(const char(&string)[Size])
{
    return base16_hash(string);
}

} // namespace system
} // namespace libbitcoin

#endif
