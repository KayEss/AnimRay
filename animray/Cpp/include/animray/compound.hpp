/*
    Copyright 2014, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

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


#ifndef ANIMRAY_COMPOUND_HPP
#define ANIMRAY_COMPOUND_HPP
#pragma once


namespace animray {


    /// Stores geometry objects of different types
    template< typename O, typename... Os >
    class compound {
    public:
        /// The type of the local coordinate system
        typedef typename O::local_coord_type local_coord_type;

        /// Intersections will be of all sorts of different types
        class intersection_type {
            boost::variant<
                typename O::intersection_type,
                typename Os::intersection_type...> intersection;
        public:
            intersection_type() {}

            template<typename I>
            intersection_type(I &&i)
            : intersection(std::move(i)) {
            }
        };

        /// The type we use to store the instances
        typedef std::tuple<O, Os...> instances_type;

        /// Stores the geometry
        fostlib::accessors<instances_type, fostlib::lvalue> instances;

        template<typename R>
        fostlib::nullable< intersection_type > intersects(const R &by) const {
            return intersection_calculation<1 + sizeof...(Os), 0>()(*this, by);
        };

        /// Calculate whether this object occludes the ray or not
        template< typename R >
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return false;
        }
    private:
        template< std::size_t left, std::size_t item >
        struct intersection_calculation {
            template<typename R>
            fostlib::nullable<intersection_type> operator () (
                const compound &geometry, const R &by
            ) {
                fostlib::nullable<intersection_type> intersection1
                    (std::get<item>(geometry.instances()).intersects(by));
                fostlib::nullable<intersection_type> intersection2
                    (intersection_calculation<left - 1, item + 1>()(geometry, by));
                if ( intersection1.isnull() ) {
                    return intersection2;
                } else if ( intersection2.isnull() ) {
                    return intersection1;
                } else {
                    throw fostlib::exceptions::not_implemented("compound::intersection_calculation operator ()");
                }
            }
        };
        template< std::size_t item >
        struct intersection_calculation< 1, item > {
            template<typename R>
            fostlib::nullable<intersection_type> operator () (
                const compound &geometry, const R &by
            ) {
                return std::get<item>(geometry.instances()).intersects(by);
            }
        };
    };


}


#endif // ANIMRAY_COMPOUND_HPP
