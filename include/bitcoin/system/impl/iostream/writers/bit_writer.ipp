/**
 * Copyright (c) 2011-2021 libbitcoin developers (see AUTHORS)
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

// Sponsored in part by Digital Contract Design, LLC

#ifndef LIBBITCOIN_SYSTEM_IOSTREAM_READERS_BIT_WRITER_IPP
#define LIBBITCOIN_SYSTEM_IOSTREAM_READERS_BIT_WRITER_IPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <bitcoin/system/constants.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/sign.hpp>
#include <bitcoin/system/serialization/endian.hpp>

namespace libbitcoin {
namespace system {

template <typename OStream>
bit_writer<OStream>::bit_writer(OStream& sink) noexcept
  : byte_writer(sink), buffer_(0x00), offset_(0)
{
}

template <typename OStream>
bit_writer<OStream>::~bit_writer() noexcept
{
    dump();
    byte_writer::~byte_writer();
}

// bits
//-----------------------------------------------------------------------------

template <typename OStream>
void bit_writer<OStream>::write_bit(bool value) noexcept
{
    if (is_aligned())
        dump();

    buffer_ |= ((value ? 0x80 : 0x00) >> offset_++);
}

// TODO: change bits type to size_t.
// TODO: templatize on value type and default to all bits (0).
template <typename OStream>
void bit_writer<OStream>::write_bits(uint64_t value, uint8_t bits) noexcept
{
    if (is_zero(bits))
        return;

    // limit value of bits parameter to 64.
    auto write_bits = lesser<uint8_t>(to_bits(sizeof(uint64_t)), bits);

    while (write_bits > byte_bits)
        do_write((value >> ((write_bits -= byte_bits))) & 0xff);

    for (uint8_t index = 0; index < write_bits; ++index)
        write_bit((value >> (write_bits - add1(index))) & 0x01);
}

// TODO: implement forward seek.
template <typename OStream>
void bit_writer<OStream>::skip_bit(size_t bits) noexcept
{
    while (!is_zero(bits--))
        write_bit(false);
}

// protected overrides
//-----------------------------------------------------------------------------

template <typename OStream>
void bit_writer<OStream>::do_write(uint8_t byte) noexcept
{
    do_write(&byte, one);
}

template <typename OStream>
void bit_writer<OStream>::do_write(const uint8_t* data, size_t size) noexcept
{
    if (is_zero(size))
        return;

    if (is_zero(offset_))
    {
        byte_writer::do_write(data, size);
        return;
    }

    // Create a mutable copy for shifting.
    data_chunk buffer(data, std::next(data, size));

    // Shift all bytes.
    for (auto index = sub1(size); !is_zero(index); --index)
        buffer[index] =
            ((buffer[index] << shift()) |
            (buffer[sub1(index)] >> offset_));

    // Or buffer into first byte.
    buffer[0] = (buffer_ | (buffer[0] >> offset_));

    byte_writer::do_write(data, size);
    buffer_ = data[sub1(size)] << shift();
}

template <typename OStream>
void bit_writer<OStream>::do_skip(size_t size) noexcept
{
    skip_bit(to_bits(size));
}

template <typename OStream>
void bit_writer<OStream>::do_flush() noexcept
{
    dump();
    byte_writer::do_flush();
}

template <typename OStream>
bool bit_writer<OStream>::get_valid() const noexcept
{
    return byte_writer::get_valid();
}

// private
//-----------------------------------------------------------------------------

template <typename OStream>
bool bit_writer<OStream>::is_aligned() const noexcept
{
    return is_zero(shift());
}

template <typename OStream>
uint8_t bit_writer<OStream>::shift() const noexcept
{
    return byte_bits - offset_;
}

template <typename OStream>
void bit_writer<OStream>::dump() noexcept
{
    if (is_zero(offset_))
        return;

    byte_writer::do_write(buffer_);
    buffer_ = 0x00;
    offset_ = 0;
}

} // namespace system
} // namespace libbitcoin

#endif