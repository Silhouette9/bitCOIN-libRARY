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
#ifndef LIBBITCOIN_SYSTEM_DATA_COLLECTION_HPP
#define LIBBITCOIN_SYSTEM_DATA_COLLECTION_HPP

#include <iterator>
#include <vector>
#include <bitcoin/system/data/data.hpp>

namespace libbitcoin {
namespace system {

/**
 * Find the position of an element in a *lexically sorted* collection.
 * @param      <Container>  The type of collection.
 * @param      <Element>    The type of collection member elements.
 * @param[in]  list         The iterable list of comparable elements to search.
 * @param[in]  value        The value of the element to find.
 * @return                  The position or negative if not found.
 *                          Negative if list size > max_int32.
 */
template <typename Element, typename Container>
int binary_search(const Container& list, const Element& value);

/**
 * Cast vector/enumerable elements into a new vector.
 * @param      <Target>  The target element type.
 * @param      <Source>  The source element type.
 * @param[in]  source    The enumeration of Source elements to cast.
 * @returns              A new enumeration with elements cast to Target.
 */
template <typename Target, typename Source>
std::vector<Target> cast(const std::vector<Source>& source);

/**
 * Determine if a collection contains the specified element.
 * @param      <Container>  The type of list.
 * @param      <Element>    The type of list member elements.
 * @param[in]  list         The list to search.
 * @param[in]  value        The value of the element to find.
 * @return                  True if contained, otherwise false.
 */
template <typename Container, typename Element>
bool contains(const Container& list, const Element& value);

/**
 * Obtain the sorted distinct elements of the list.
 * @param      <Element>  The list element type.
 * @param[in]  list       The list.
 * @return                The sorted distinct list.
 */
template <typename Element>
std::vector<Element> distinct(std::vector<Element>&& list);

/**
 * Find the position of a pair in an ordered list.
 * @param      <Pair>  The type of list member elements.
 * @param      <Key>   The type of list member element keys.
 * @param[in]  list    The list to search.
 * @param[in]  key     The key to the element to find.
 * @return             The position or negative if not found.
 */
template <typename Pair, typename Key>
int find_pair_position(const std::vector<const Pair>& list, Key& key);

/**
 * Find the position of an element in an ordered collection.
 * @param      <Container>  The type of collection.
 * @param      <Element>    The type of collection member elements.
 * @param[in]  list         The list to search.
 * @param[in]  value        The value of the element to find.
 * @return                  The position or negative if not found.
 */
template <typename Element, typename Container>
int find_position(const Container& list, const Element& value);

/**
 * Facilitate a list insertion sort by inserting into a sorted position.
 * @param      <Type>       The type of list member elements.
 * @param      <Predicate>  The type of the predicate.
 * @param[in]  list         The list to modify.
 * @param[in]  element      The element to insert.
 * @param[in]  predicate    The sort predicate.
 * @return                  The vector iterator.
 */
template <typename Type, typename Predicate>
typename std::vector<Type>::iterator insert_sorted(std::vector<Type>& list,
    const Type& element, Predicate predicate);

/**
 * Determine if a collection contains only distinct members.
 * @param      <Container>  The type of list.
 * @param      <Element>    The type of list member elements.
 * @param[in]  list         The list to search.
 * @return                  True if distinct, otherwise false.
 */
template <typename Element>
bool is_distinct(std::vector<Element>&& list);

/**
 * Move members of a source list to end of a target list. Source members
 * are undefined upon return.
 * @param      <Type>  The type of list member elements.
 * @param[in]  target  The target list.
 * @param[in]  source  The source list
 */
template <typename Type>
void move_append(std::vector<Type>& target, std::vector<Type>& source);

/**
 * Pop an element from the stack and return its value.
 * @param      <Element>  The stack element type.
 * @param[in]  stack      The stack.
 */
template <typename Element>
Element pop(std::vector<Element>& stack);

/**
 * Create a reversed copy of a collection.
 * @param      <Collection>  The type of the collection to reverse.
 * @param[in]  source        The collection to reverse.
 */
template <typename Collection>
Collection reverse(const Collection& source);

} // namespace system
} // namespace libbitcoin

#include <bitcoin/system/impl/data/collection.ipp>

#endif