/**
    Copyright 1995-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_MATRIX_HPP
#define ANIMRAY_MATRIX_HPP
#pragma once


#include <animray/point3d.hpp>


namespace animray {


    /// This 4D matrix class represents transformations in 3D space
    template<typename D>
    class matrix : private detail::array_based<D, 16> {
        typedef detail::array_based<D, 16> superclass;
        using superclass::at;

      public:
        using value_type = typename superclass::value_type;
        using array_type = typename superclass::array_type;
        using const_value_parameter_type =
                typename superclass::const_value_parameter_type;
        static const std::size_t c_array_size = superclass::c_array_size;

        /// Construct an identity transform matrix
        matrix() {
            // Set the values on the leading diagonal to 1
            superclass::array[0] = D(1);
            superclass::array[5] = D(1);
            superclass::array[10] = D(1);
            superclass::array[15] = D(1);
        }

        /// Allows us to fetch values from rows then columns
        class row_proxy {
            friend class matrix;
            matrix &m;
            const std::size_t r;
            row_proxy(matrix &m, std::size_t r) : m(m), r(r) {}

          public:
            value_type &operator[](std::size_t c) { return m.at(r * 4 + c); }
        };
        friend row_proxy;
        class const_row_proxy {
            friend class matrix;
            const matrix &m;
            const std::size_t r;
            const_row_proxy(const matrix &m, std::size_t r) : m(m), r(r) {}

          public:
            value_type operator[](std::size_t c) { return m.at(r * 4 + c); }
            point3d<matrix::value_type>
                    operator*(const point3d<matrix::value_type> &by) {
                return point3d<matrix::value_type>(
                        m.at(r * 4) * by.array()[0],
                        m.at(r * 4 + 1) * by.array()[1],
                        m.at(r * 4 + 2) * by.array()[2],
                        m.at(r * 4 + 3) * by.array()[3]);
            }
        };
        friend row_proxy;

        /// Allow a row to be fetched from the matrix
        const_row_proxy operator[](std::size_t r) const {
            return const_row_proxy(*this, r);
        }
        /// Allow a row to be fetched from the matrix
        row_proxy operator[](std::size_t r) { return row_proxy(*this, r); }

        /// Compare for equality
        bool operator==(const matrix &r) const {
            return superclass::array == r.array;
        }
        /// Compare for inequality
        bool operator!=(const matrix &r) const {
            return superclass::array != r.array;
        }

        /// Fetch a column as a vector
        point3d<value_type> column(std::size_t col) const {
            return point3d<value_type>(
                    at(col), at(col + 4), at(col + 8), at(col + 12));
        }

        /// Multiply two matrixes
        matrix operator*(const matrix &r) const {
            matrix result;
            const point3d<value_type> cols[4] = {
                    r.column(0), r.column(1), r.column(2), r.column(3)};
            for (std::size_t row(0); row < 4; ++row) {
                const_row_proxy my_row((*this)[row]);
                row_proxy res_row(result[row]);
                for (std::size_t col(0); col < 4; ++col) {
                    res_row[col] = sum((my_row * cols[col]).array());
                }
            }
            return result;
        }
        /// Multiply by a vector
        point3d<value_type> operator*(const point3d<value_type> v) const {
            return point3d<value_type>(
                    sum(((*this)[0] * v).array()),
                    sum(((*this)[1] * v).array()),
                    sum(((*this)[2] * v).array()),
                    sum(((*this)[3] * v).array()));
        }

        /// Multiply by another matrix
        matrix &operator*=(const matrix &r) {
            matrix result(*this * r);
            return *this = result;
        }

        fostlib::ostream &print_on(fostlib::ostream &o) const {
            for (std::size_t r = 0; r < 4; ++r) {
                for (std::size_t c = 0; c < 4; ++c) {
                    o << superclass::array[r * 4 + c];
                }
                o << '\n';
            }
            return o;
        }
    };


}


#endif // ANIMRAY_MATRIX_HPP
