/*
    Copyright 2018, Kirit Saelensminde.
    <https://kirit.com/AnimRay>

    This file is part of AnimRay.

    AnimRay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AnimRay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AnimRay.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include <tuple>


namespace animray {


    namespace detail {
        template<typename T1, typename T2, std::size_t... I>
        constexpr decltype(auto)
                zipper(T1 &&t1, T2 &&t2, std::index_sequence<I...>) {
            return std::make_tuple(
                    std::make_pair(std::get<I>(t1), std::get<I>(t2))...);
        }
    }


    /**
     * # Zip.
     *
     * Zips two tuples of the same size into a single tuple of pairs.
     */
    template<typename T1, typename T2>
    constexpr decltype(auto) zip(T1 &&t1, T2 &&t2) {
        constexpr const std::size_t size1 =
                std::tuple_size<std::decay_t<T1>>::value;
        constexpr const std::size_t size2 =
                std::tuple_size<std::decay_t<T2>>::value;
        static_assert(
                size1 == size2,
                "Tuples that are to be zipped must be same size");
        return detail::zipper(
                std::forward<T1>(t1), std::forward<T2>(t2),
                std::make_index_sequence<size1>{});
    }


}
