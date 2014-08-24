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


#include <vector>
#include <memory>


namespace animray {


    /// A simple compound object with no intelligence
    template<typename O,
            typename I = typename O::intersection_type>
    class compound {
        std::vector<std::unique_ptr<O>> instances;
    public:
        /// The type of objects that can be inserted
        typedef O instance_type;
        /// The type of the local coordinate system
        typedef typename instance_type::local_coord_type local_coord_type;
        /// The type of the ray output by the instance
        typedef I intersection_type;

        /// Insert a new object into the compound
        template<typename G>
        compound &insert(const G &instance) {
            instances.push_back(
                std::unique_ptr<instance_type>(new G(instance)));
            return *this;
        }

        /// Ray intersection with closest item
        template<typename R>
        fostlib::nullable< intersection_type > intersects(const R &by) const {
            fostlib::nullable< intersection_type > result;
            local_coord_type result_dot;
            for ( const auto &instance : instances ) {
                if ( result.isnull() ) {
                    result = instance->intersects(by);
                    if ( !result.isnull() ) {
                        result_dot = (result.value().from() - by.from()).dot();
                    }
                } else {
                    fostlib::nullable< intersection_type > intersection(
                        instance->intersects(by));
                    if ( !intersection.isnull() ) {
                        local_coord_type dot(
                            (intersection.value().from() - by.from()).dot());
                        if ( dot < result_dot ) {
                            result = intersection;
                            result_dot = dot;
                        }
                    }
                }
            }
            return result;
        }

        /// Occlusion check
        template<typename R>
        bool occludes(
            const R &by, const typename R::local_coord_type epsilon
        ) const {
            return std::find_if(instances.begin(), instances.end(),
                [&](const std::unique_ptr<instance_type> &instance) {
                    return instance->occludes(by, epsilon);
                }) != instances.end();
        }
    };


}


#endif // ANIMRAY_COMPOUND_HPP
