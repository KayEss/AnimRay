/*
    Copyright 2014-2018, Kirit Saelensminde.
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


#ifndef ANIMRAY_GEOMETRY_COLLECTION_HPP
#define ANIMRAY_GEOMETRY_COLLECTION_HPP
#pragma once


#include <vector>
#include <memory>


namespace animray {


    /// A simple collection of objects
    template<typename O, typename V = std::vector<O>>
    class collection {
      public:
        /// The type of objects that can be inserted
        typedef O instance_type;
        /// The type of the collection
        typedef V collection_type;
        /// The type of the local coordinate system
        typedef typename instance_type::local_coord_type local_coord_type;
        /// The type of the ray output by the instance
        typedef typename O::intersection_type intersection_type;

        /// The instances
        fostlib::accessors<collection_type, fostlib::lvalue> instances;

        /// Insert a new object into the compound
        template<typename G>
        collection &insert(const G &instance) {
            // Other code may depend on this being a push_back,
            // at least for collections that use std::vector
            instances().push_back(instance);
            return *this;
        }

        /// Ray intersection with closest item
        template<typename R, typename E>
        fostlib::nullable<intersection_type>
                intersects(const R &by, const E epsilon) const {
            fostlib::nullable<intersection_type> result;
            local_coord_type result_dot;
            for (const auto &instance : instances()) {
                if (result) {
                    fostlib::nullable<intersection_type> intersection(
                            instance.intersects(by, epsilon));
                    if (intersection) {
                        local_coord_type dot(
                                (intersection.value().from() - by.from()).dot());
                        if (dot < result_dot) {
                            result = std::move(intersection);
                            result_dot = dot;
                        }
                    }
                } else {
                    result = instance.intersects(by, epsilon);
                    if (result) {
                        result_dot = (result.value().from() - by.from()).dot();
                    }
                }
            }
            return result;
        }

        /// Occlusion check
        template<typename R, typename E>
        bool occludes(const R &by, const E epsilon) const {
            return std::find_if(
                           instances().begin(), instances().end(),
                           [&by, epsilon](const instance_type &instance) {
                               return instance.occludes(by, epsilon);
                           })
                    != instances().end();
        }
    };


}


#endif // ANIMRAY_GEOMETRY_COLLECTION_HPP
