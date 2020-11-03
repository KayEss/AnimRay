/**
    Copyright 2014-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/intersection.hpp>
#include <animray/shader.hpp>
#include <animray/emission.hpp>
#include <animray/maths/fold.hpp>

#include <optional>
#include <tuple>
#include <variant>


namespace animray {


    /**
     * # Compound
     *
     * This geometry type stores one of each of a number of different
     * geometries.
     */


    /// Stores geometry objects of different types
    template<typename O, typename... Os>
    class compound;


    /// Partial specialisation of the intersection type for compound
    template<typename O, typename... Os>
    class intersection<compound<O, Os...>> {
      public:
        /// The type of the local coordinate system
        using local_coord_type = typename std::common_type<
                typename O::local_coord_type,
                typename Os::local_coord_type...>::type;
        /// The type of the strike location
        using end_type = typename std::common_type<
                typename O::intersection_type::end_type,
                typename Os::intersection_type::end_type...>::type;
        /// The type of the strike location
        using direction_type = typename std::common_type<
                typename O::intersection_type::direction_type,
                typename Os::intersection_type::direction_type...>::type;

        /// The wrapped intersection
        using intersection_type = std::variant<
                typename O::intersection_type,
                typename Os::intersection_type...>;

        end_type from;
        direction_type direction;
        intersection_type wrapped_intersection;

        intersection(intersection_type i)
        : from{std::visit([](auto i) { return i.from; }, i)},
          direction{std::visit([](auto i) { return i.direction; }, i)},
          wrapped_intersection{std::move(i)} {}
    };


    /// Stores geometry objects of different types
    template<typename O, typename... Os>
    class compound {
      public:
        /// The type we use to store the instances
        using instances_type = std::tuple<O, Os...>;
        /// The type of the local coordinate system
        using local_coord_type = typename O::local_coord_type;
        /// The intersection type
        using intersection_type = intersection<compound>;

        /// Stores the geometry
        instances_type instances;

        template<typename... G>
        compound(G... gs) : instances{std::forward<G>(gs)...} {}

        /// Forward the intersection check to the geometry instances.
        /// Return the closest intersection, `null` if none are found.
        template<typename R, typename E>
        std::optional<intersection_type>
                intersects(const R &by, const E epsilon) const {
            using mid_type = std::pair<
                    std::optional<local_coord_type>,
                    std::optional<intersection_type>>;
            return std::apply(
                           [&by, epsilon](const auto &...geom) {
                               auto const dot = [&](auto i) -> mid_type {
                                   if (i) {
                                       return mid_type{
                                               (i->from - by.from).dot(),
                                               std::move(*i)};
                                   } else {
                                       return {std::nullopt, std::nullopt};
                                   }
                               };
                               return foldl(
                                       [](auto i1, auto i2) -> mid_type {
                                           if (not i1.first) {
                                               return i2;
                                           } else if (not i2.first) {
                                               return i1;
                                           } else if (*i1.first < *i2.first) {
                                               return i1;
                                           } else {
                                               return i2;
                                           }
                                       },
                                       dot(geom.intersects(by, epsilon))...);
                           },
                           instances)
                    .second;
        };

        /// Calculate whether this object occludes the ray or not
        template<typename R>
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return std::apply(
                    [&by, epsilon](const auto &...geom) -> bool {
                        return (geom.occludes(by, epsilon) || ...);
                    },
                    instances);
            return false;
        }
    };


    template<
            typename C,
            typename O,
            typename RI,
            typename RL,
            typename G,
            typename... Os>
    struct surface_interaction<C, intersection<compound<O, Os...>>, RI, RL, G> {
        surface_interaction() = default;
        C operator()(
                const RI &observer,
                const RL &light,
                const intersection<compound<O, Os...>> &intersection,
                const C &incident,
                const G &geometry) const {
            return std::visit(
                    [&](const auto &inter) {
                        return shader(
                                observer, light, inter, incident, geometry);
                    },
                    intersection.wrapped_intersection);
        }
    };


    /**
     * Returns the emission characteristics for the light that was
     * struck by the intersection.
     */
    template<typename C, typename O, typename RI, typename G, typename... Os>
    struct surface_emission<C, RI, intersection<compound<O, Os...>>, G> {
        surface_emission() = default;
        C operator()(
                const RI &observer,
                const intersection<compound<O, Os...>> &intersection,
                const G &geometry) const {
            return std::visit(
                    [&](const auto &inter) {
                        return emission<C>(observer, inter, geometry);
                    },
                    intersection.wrapped_intersection);
        }
    };


}


#endif // ANIMRAY_COMPOUND_HPP
