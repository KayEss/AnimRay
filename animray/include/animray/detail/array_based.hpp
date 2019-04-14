/**
    Copyright 2010-2019, [Kirit Saelensminde](https://kirit.com/AnimRay)

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


#ifndef ANIMRAY_DETAIL_ARRAY_BASED
#define ANIMRAY_DETAIL_ARRAY_BASED
#pragma once


#include <fost/core>

#include <array>
#include <numeric>


namespace animray {


    namespace detail {


        /// Non-templated base class used for matching
        struct array_based_base_class {};

        /// Used for implementations that are actually just a vector
        template<typename D, std::size_t S>
        struct array_based : array_based_base_class {
            /// The type of the values
            typedef D value_type;
            /// Type to use as a parameter
            typedef const D &const_value_parameter_type;
            /// The type of the array
            using array_type = std::array<D, S>;
            /// The number of elements in the array
            static const std::size_t c_array_size = S;

            /// The actual data
            array_type array;

            /// Default construct the array members
            array_based() : array() {}

            /// Fetch a value from the array with bounds checking
            value_type at(std::size_t p) const {
                try {
                    return array.at(p);
                } catch (std::out_of_range &) {
                    throw fostlib::exceptions::out_of_range<std::size_t>(
                            "Array index was out of bounds", 0, c_array_size,
                            p);
                }
            }
            /// Fetch a value from the array with bounds checking
            value_type &at(std::size_t p) {
                try {
                    return array.at(p);
                } catch (std::out_of_range &) {
                    throw fostlib::exceptions::out_of_range<std::size_t>(
                            "Array index was out of bounds", 0, c_array_size,
                            p);
                }
            }

            /// Turn the array into a JSON array
            fostlib::json to_json() const {
                fostlib::json r;
                for (std::size_t i = 0; i < array.size(); ++i)
                    fostlib::jcursor().push_back(
                            r, fostlib::coerce<fostlib::json>(array[i]));
                return r;
            }
            /// Print the vector onto a stream
            fostlib::ostream &print_on(fostlib::ostream &o) const {
                o << "(";
                for (std::size_t i = 0; i < array.size(); ++i) {
                    if (i != 0) o << ", ";
                    o << array[i];
                }
                return o << ")";
            }

            /// Add a value to each component
            array_based operator+(const value_type v) const {
                array_based c(*this);
                for (auto &i : c.array) { i += v; }
                return c;
            }
            /// Add a value to each component
            array_based &operator+=(const array_based &v) {
                for (std::size_t i(0); i < array.size(); ++i) {
                    array[i] += v.array[i];
                }
                return *this;
            }

            /// Multiply each component
            template<typename W>
            typename std::enable_if<std::is_scalar<W>::value, array_based>::type
                    operator*(const W w) const {
                array_based c(*this);
                for (auto &i : c.array) { i = value_type(i * w); }
                return c;
            }
            /// Multiply each component by the corresponding value
            array_based operator*(const array_based &w) const {
                array_based c(*this);
                for (std::size_t i(0); i < array.size(); ++i) {
                    c.array[i] *= w.array[i];
                }
                return c;
            }

            /// Divide by a value
            template<typename W>
            typename std::enable_if<std::is_scalar<W>::value, array_based &>::type
                    operator/=(const W &s) {
                for (auto &i : array) { i /= s; }
                return *this;
            }
        };


    }


    /// Return the sum of the values
    template<typename D, std::size_t S>
    D sum(const std::array<D, S> &arr) {
        return std::accumulate(arr.begin(), arr.end(), D());
    }


    template<typename T>
    inline std::enable_if_t<
            std::is_base_of_v<detail::array_based_base_class, T>,
            std::ostream &>
            operator<<(std::ostream &o, const T &a) {
        return a.print_on(o);
    }


}


namespace fostlib {
    /// Allows coercion from array types to JSON
    template<typename T>
    struct coercer<
            fostlib::json,
            T,
            std::enable_if_t<
                    std::is_base_of_v<animray::detail::array_based_base_class, T>>> {
        fostlib::json coerce(const T &a) { return a.to_json(); }
    };
    template<typename T>
    struct coercer<
            T,
            fostlib::json,
            std::enable_if_t<
                    std::is_base_of_v<
                            animray::detail::array_based_base_class,
                            T> && T::c_array_size == 3>> {
        T coerce(const json &j) {
            return T(
                    fostlib::coerce<typename T::value_type>(j[0]),
                    fostlib::coerce<typename T::value_type>(j[1]),
                    fostlib::coerce<typename T::value_type>(j[2]));
        }
    };
    template<typename T>
    struct coercer<
            T,
            fostlib::json,
            std::enable_if_t<
                    std::is_base_of_v<
                            animray::detail::array_based_base_class,
                            T> && T::c_array_size == 4>> {
        T coerce(const json &j) {
            return T(
                    fostlib::coerce<typename T::value_type>(j[0]),
                    fostlib::coerce<typename T::value_type>(j[1]),
                    fostlib::coerce<typename T::value_type>(j[2]),
                    fostlib::coerce<typename T::value_type>(j[3]));
        }
    };
    template<typename T>
    struct coercer<
            T,
            fostlib::json,
            std::enable_if_t<
                    std::is_base_of_v<
                            animray::detail::array_based_base_class,
                            T> && (T::c_array_size > 4)>> {
        T coerce(const json &j) { return T(j); }
    };


}


#endif // ANIMRAY_DETAIL_ARRAY_BASED
