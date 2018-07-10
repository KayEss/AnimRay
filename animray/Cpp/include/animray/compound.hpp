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


#ifndef ANIMRAY_COMPOUND_HPP
#define ANIMRAY_COMPOUND_HPP
#pragma once


#include <fost/core>
#include <animray/intersection.hpp>
#include <animray/shader.hpp>
#include <animray/emission.hpp>
#include <tuple>


namespace animray {


    /// Stores geometry objects of different types
    template< typename O, typename... Os >
    class compound;


    /// Partial specialisation of the intersection type for compound
    template< typename O, typename... Os >
    class intersection< compound<O, Os...> > {
    public:
        /// The type of the local coordinate system
        typedef typename O::local_coord_type local_coord_type;
        /// The type of the strike location
        typedef typename O::intersection_type::end_type end_type;
        /// The type of the strike location
        typedef typename O::intersection_type::direction_type direction_type;

        /// The wrapped intersection
        fostlib::accessors<
            boost::variant<
                typename O::intersection_type,
                typename Os::intersection_type...
            >, fostlib::lvalue> wrapped_intersection;

        intersection() {}

        template<typename I>
        intersection(I &&i)
        : wrapped_intersection(std::move(i)) {
        }

        end_type from() const {
            return boost::apply_visitor(end_forwarder(), wrapped_intersection());
        }

        direction_type direction() const {
            return boost::apply_visitor(dir_forwarder(), wrapped_intersection());
        }
    private:
        struct end_forwarder : public boost::static_visitor<end_type>{
            template<typename I>
            end_type operator () (const I &inter) const {
                return inter.from();
            }
        };
        struct dir_forwarder : public boost::static_visitor<direction_type>{
            template<typename I>
            direction_type operator () (const I &inter) const {
                return inter.direction();
            }
        };
    };


    /// Stores geometry objects of different types
    template< typename O, typename... Os >
    class compound {
    public:
        /// The type we use to store the instances
        typedef std::tuple<O, Os...> instances_type;
        /// The type of the local coordinate system
        typedef typename O::local_coord_type local_coord_type;
        /// The intersection type
        typedef intersection<compound> intersection_type;

        /// Stores the geometry
        fostlib::accessors<instances_type, fostlib::lvalue> instances;

        /// Forward the intersection check to the geometry instances
        template<typename R, typename E>
        fostlib::nullable< intersection_type > intersects(
            const R &by, const E epsilon
        ) const {
            return intersection_calculation<1 + sizeof...(Os), 0>()
                (*this, by, epsilon);
        };

        /// Calculate whether this object occludes the ray or not
        template< typename R >
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return occlusion_calculation<1 + sizeof...(Os), 0>()(*this, by, epsilon);
        }

    private:
        template< std::size_t left, std::size_t item >
        struct intersection_calculation {
            template<typename R, typename E>
            fostlib::nullable<intersection_type> operator () (
                const compound &geometry, const R &by, const E epsilon
            ) const {
                fostlib::nullable<intersection_type> intersection1
                    (std::get<item>(geometry.instances()).intersects(by, epsilon));
                fostlib::nullable<intersection_type> intersection2
                    (intersection_calculation<left - 1, item + 1>()
                        (geometry, by, epsilon));
                if ( not intersection1 ) {
                    return intersection2;
                } else if ( not intersection2 ) {
                    return intersection1;
                } else {
                    if ( (intersection1.value().from() - by.from()).dot() <
                            (intersection2.value().from() - by.from()).dot() ) {
                        return intersection1;
                    }  else {
                        return intersection2;
                    }
                }
            }
        };
        template< std::size_t item >
        struct intersection_calculation< 1, item > {
            template<typename R, typename E>
            fostlib::nullable<intersection_type> operator () (
                const compound &geometry, const R &by, const E epsilon
            ) const {
                return std::get<item>(geometry.instances()).intersects(by, epsilon);
            }
        };

        template< std::size_t left, std::size_t item >
        struct occlusion_calculation {
            template<typename R>
            bool operator () (
                const compound &geometry, const R &by,
                const local_coord_type epsilon
            ) const {
                return std::get<item>(geometry.instances()).occludes(by, epsilon) ||
                    occlusion_calculation<left - 1, item + 1>()(geometry, by, epsilon);
            }
        };
        template< std::size_t item >
        struct occlusion_calculation< 1, item > {
            template<typename R>
            bool operator () (
                const compound &geometry, const R &by,
                const local_coord_type epsilon
            ) const {
                return std::get<item>(geometry.instances()).occludes(by, epsilon);
            }
        };
    };


    template<typename C, typename O, typename RI, typename RL,
        typename G, typename... Os>
    struct surface_interaction<C, intersection<compound<O, Os...>>, RI, RL, G> {
        struct forwarder : public boost::static_visitor<C>{
            const RI &observer;
            const RL &light;
            const C &incident;
            const G &geometry;
            forwarder(const RI &observer, const RL &light,
                    const C &incident, const G &geometry)
            : observer(observer), light(light), incident(incident),
                    geometry(geometry) {
            }

            template<typename I>
            C operator () (const I &inter) const {
                return shader(observer, light, inter, incident, geometry);
            }
        };
        surface_interaction() {}
        C operator() (
            const RI &observer, const RL &light,
            const intersection< compound<O, Os...> > &intersection,
            const C &incident, const G &geometry
        ) const {
            return boost::apply_visitor(
                forwarder(observer, light, incident, geometry),
                intersection.wrapped_intersection());
        }
    };


    template<typename C, typename O, typename RI,
        typename G, typename... Os>
    struct surface_emission<C, RI, intersection<compound<O, Os...>>, G> {
        struct forwarder : public boost::static_visitor<C>{
            const RI &observer;
            const G &geometry;
            forwarder(const RI &observer, const G &geometry)
            : observer(observer), geometry(geometry) {
            }

            template<typename I>
            C operator () (const I &inter) const {
                return emission<C>(observer, inter, geometry);
            }
        };
        surface_emission() {}
        C operator() (
            const RI &observer,
            const intersection< compound<O, Os...> > &intersection,
            const G &geometry
        ) const {
            return boost::apply_visitor(
                forwarder(observer, geometry),
                intersection.wrapped_intersection());
        }
    };


}


#endif // ANIMRAY_COMPOUND_HPP
