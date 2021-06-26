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
#ifndef LIBBITCOIN_SYSTEM_STREAM_SOURCES_COPY_SOURCE_HPP
#define LIBBITCOIN_SYSTEM_STREAM_SOURCES_COPY_SOURCE_HPP

#include <boost/iostreams/stream.hpp>
#include <bitcoin/system/data/data.hpp>
#include <bitcoin/system/math/limits.hpp>
#include <bitcoin/system/stream/device.hpp>
#include <bitcoin/system/type_constraints.hpp>

namespace libbitcoin {
namespace system {

/// Source for boost::iostreams::stream, copies bytes from Container.
/// Container may be any iterable object with contiguous byte data.
template <typename Container, if_base_of<data_slice, Container> = true>
class copy_source
  : public device<Container>
{
public:
    // istream_tag (compiles)
    struct category
      : boost::iostreams::input_seekable,
        boost::iostreams::direct_tag {};
    typedef const Container& container;

    copy_source(const Container& data) noexcept
      : device(limit<size_type>(data.size())),
        container_(data),
        next_(data.begin())
    {
    }

protected:
    sequence do_sequence() noexcept override
    {
        // input_sequence/output_sequence both require non-const buffer ptrs,
        // but the data member is const, so we must cast it for direct devices.
        const auto begin = const_cast<value_type*>(container_.data());
        const auto end = std::next(begin, container_.size());

        return std::make_pair(
            reinterpret_cast<char_type*>(begin),
            reinterpret_cast<char_type*>(end));
    }

    void do_read(value_type* to, size_type size) noexcept override
    {
        // std::copy_n returns iterator past last element copied to.
        std::copy_n(next_, size, to);
        std::advance(next_, size);
    }

private:
    const Container container_;
    typename Container::const_iterator next_;
};

} // namespace system
} // namespace libbitcoin

#endif