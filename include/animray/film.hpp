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


#ifndef ANIMRAY_FILM_HPP
#define ANIMRAY_FILM_HPP
#pragma once


#include <animray/extents2d.hpp>
#include <fost/exception/out_of_range.hpp>


namespace animray {


    /// A film represents a raster of pixel data in columns
    template<typename C, typename E = std::size_t>
    class film {
      public:
        /// The colour type
        typedef C color_type;
        /// The type of the extents co-ordinates
        using extents_value_type = E;
        /// The extents type
        using extents_type = extents2d<extents_value_type>;
        /// The extents size type
        using size_type = typename extents_type::size_type;
        /// The type of a single column of image data
        using column_type = std::vector<color_type>;

        /// Default constructor
        film() = default;

        /// Construct an empty targa of the given size
        film(size_type width, size_type height, const C &colour = C())
        : columns(width, column_type(height, colour)) {
            check_width_height(width, height);
        }

        /// Construct a film of a given size with a lambda telling us which
        /// colors to use
        film(size_type width,
             size_type height,
             std::function<color_type(size_type, size_type)> fn) {
            check_width_height(width, height);
            for (size_type c{}; c < width; ++c) {
                columns.emplace_back();
                column_type &col = columns.back();
                for (size_type r{}; r < height; ++r) {
                    col.push_back(fn(c, r));
                }
            }
        }
        film(size_type width,
             size_type height,
             std::function<color_type(size_type, size_type)>
                     fn) requires std::is_copy_constructible_v<C> :
        film{width, height} {
            check_width_height(width, height);
            for (size_type c{}; c < width; ++c) {
                column_type &col = columns[c];
                for (size_type r{}; r < height; ++r) { col[r] = fn(c, r); }
            }
        }

        /// The width of the image
        const size_type width() const { return columns.size(); }
        /// The height of the image
        const size_type height() const { return columns[0].size(); }
        /// Return the extents of the image
        extents_type size() const {
            return extents_type(0, 0, width() - 1, height() - 1);
        }

        /// Return a mutable row
        column_type &operator[](size_type c) { return columns.at(c); }
        /// Return a non-mutable row
        const column_type &operator[](size_type c) const {
            return columns.at(c);
        }

        /// Iterate the function across the image rows/columns
        template<typename F>
        void for_each(F fn) const {
            for (size_type c = 0; c < width(); ++c) {
                const column_type &col = columns[c];
                for (size_type r = 0; r < height(); ++r) { fn(col[r]); }
            }
        }
        /// Allow us to force iteration over the rows first
        template<typename F>
        void for_each_row(F fn) const {
            for (size_type r = 0; r < height(); ++r) {
                for (size_type c = 0; c < width(); ++c) { fn(columns[c][r]); }
            }
        }

      private:
        using columns_type = std::vector<column_type>;
        columns_type columns;
        void static check_width_height(size_type width, size_type height) {
            if (width < 1) {
                throw fostlib::exceptions::out_of_range<E>{
                        1, std::numeric_limits<E>::max(), width};
            }
            if (height < 1) {
                throw fostlib::exceptions::out_of_range<E>{
                        1, std::numeric_limits<E>::max(), height};
            }
        }
    };


}


#endif // ANIMRAY_FILM_HPP
