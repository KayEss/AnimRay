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


#ifndef ANIMRAY_GEOMETRY_COLLECTION_SURFACE_HPP
#define ANIMRAY_GEOMETRY_COLLECTION_SURFACE_HPP
#pragma once


#include <animray/geometry/collection.hpp>
#include <animray/surface.hpp>


namespace animray {


    template<typename O, typename... S>
    class collection<surface<O, S...>, std::vector<surface<O, S...>>> {
        /// The instances collection type
        typedef collection<O, std::vector<O>> instances_collection_type;

    public:
        /// The surface type we're collecting over
        typedef surface<O, S...> surface_type;
        /// The type of the local coordinate system
        typedef typename O::local_coord_type local_coord_type;
        /// The intersection type
        typedef intersection<surface<O, S...>> intersection_type;

        /// Insert new geometry
        template<typename G>
        collection &insert(const G &instance) {
            instances.insert(instance.geometry());
            surfaces.push_back(instance.surface_parameters());
            return *this;
        }

        /// Calculate the intersection of the ray on the instance
        template<typename R, typename E>
        fostlib::nullable< intersection_type > intersects(
            const R &by, const E epsilon
        ) const {
            fostlib::nullable<std::pair<typename instances_collection_type::intersection_type, std::size_t>> result;
            local_coord_type result_dot;
            for ( std::size_t index(0); index != instances.instances().size(); ++index ) {
                if ( result ) {
                    fostlib::nullable< typename instances_collection_type::intersection_type >
                        intersection(instances.instances()[index].intersects(by, epsilon));
                    if ( intersection ) {
                        local_coord_type dot(
                            (intersection.value().from() - by.from()).dot());
                        if ( dot < result_dot ) {
                            result = std::make_pair(intersection.value(), index);
                            result_dot = dot;
                        }
                    }
                } else {
                    fostlib::nullable< typename instances_collection_type::intersection_type >
                        intersection(instances.instances()[index].intersects(by, epsilon));
                    if ( intersection ) {
                        result = std::make_pair(intersection.value(), index);
                        result_dot = (intersection.value().from() - by.from()).dot();
                    }
                }
            }
            if ( result ) {
                return intersection_type(result.value().first, surfaces[result.value().second]);
            } else {
                return fostlib::null;
            }
        }

        /// Occlusion check
        template<typename R>
        bool occludes(
            const R &by, const typename R::local_coord_type epsilon
        ) const {
            return instances.occludes(by, epsilon);
        }

    private:
        /// The geometry
        instances_collection_type instances;

        /// The surface parameters
        std::vector<typename surface_type::surface_parameters_type> surfaces;
    };


}


#endif // ANIMRAY_GEOMETRY_COLLECTION_SURFACE_HPP
