/**
    Copyright 2010-2018, Kirit Saelensminde.
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


#ifndef ANIMRAY_DETAIL_TEXTURE_POLICY_BASED
#define ANIMRAY_DETAIL_TEXTURE_POLICY_BASED
#pragma once


#include <fost/core>
#include <animray/point2d.hpp>

#include <functional>


namespace animray {


    /// A functor which holds a single value
    template<typename V>
    struct const_value {
        /// The value type
        typedef V result_type;
        /// Construct the functor with a provided value
        const_value(const result_type &v) : value(v) {}
        /// Stores the value
        fostlib::accessors<result_type> value;
        /// Call the functor
        const result_type &operator()() const { return value(); }
    };


    /// Identiry functor
    template<typename V>
    struct identity {
        /// The result type
        typedef V result_type;
        /// The argument type
        typedef V arg1_type;
        /// The identity function
        const result_type &operator()(const result_type &v) const { return v; }
    };


    /// A functor that calls coerce -- (TODO maybe to be generalised)
    template<typename T, typename F>
    struct coercer {
        /// The result type
        typedef T result_type;
        /// The argument type
        typedef F arg1_type;
        /// The coercion function
        result_type operator()(const F &f) const {
            return fostlib::coerce<T>(f);
        }
    };


    /// A unary functor that calls another unary functor
    template<typename R, typename A1>
    struct apply {
        /// The result type
        typedef R result_type;
        /// The argument type
        typedef A1 arg1_type;
        /// The functor itself
        template<typename F>
        result_type operator()(F f, const arg1_type &a1) const {
            return f(a1);
        }
    };


    /// A unary functor that calls another nullary functor dropping the argument
    template<typename R, typename A1>
    struct apply_without_arguments {
        /// The result type
        typedef R result_type;
        /// The argument type
        typedef A1 arg1_type;
        /// The functor itself
        template<typename F>
        result_type operator()(F f, const arg1_type &) const {
            return f();
        }
    };


    /// A policy class used to implement a texture.
    template<typename C, typename L, typename F>
    struct texture_policy {
        /// The colour type
        typedef C color_type;
        /// The location co-ordinate type
        typedef L location_type;
        /// The functor type
        typedef F functor_type;
        /// The colour conversion functor type
        typedef coercer<C, typename F::result_type> color_conversion_functor_type;
        /// The location mapping type
        typedef apply<typename F::result_type, L> location_mapping_functor_type;
        /// Texture constructor functor type
        typedef F texture_constructor_arg1_type;
    };


    /// Specialisation of the policy where there a constant value in the texture
    template<typename C, typename L>
    struct texture_policy<C, L, const_value<C>> {
        /// The colour type
        typedef C color_type;
        /// The location co-ordinate type
        typedef L location_type;
        /// The functor type
        typedef const_value<C> functor_type;
        /// The colour conversion functor type
        typedef identity<C> color_conversion_functor_type;
        /// The location mapping type
        typedef apply_without_arguments<C, L> location_mapping_functor_type;
        /// Texture constructor functor type
        typedef C texture_constructor_arg1_type;
    };


    namespace detail {
        /// A location mapper that splits the x and y for a binary functor
        template<typename R, typename A1>
        struct location_mapper_binary_op {
            /// The result type
            typedef R result_type;
            /// The argument type
            typedef A1 arg1_type;
            /// Convert the extents to x, y and call f
            template<typename F>
            R operator()(const F &f, const A1 &l) const {
                return f(l.x(), l.y());
            }
        };
    }
    /// Specialisation of the policy for binary functions
    template<typename C, typename S, typename R>
    struct texture_policy<C, animray::point2d<S>, std::function<R(S, S)>> {
        /// The colour type
        typedef C color_type;
        /// The location co-ordinate type
        typedef animray::point2d<S> location_type;
        /// The functor type
        typedef std::pointer_to_binary_function<S, S, R> functor_type;
        /// The colour conversion functor type
        typedef coercer<C, R> color_conversion_functor_type;
        /// The location mapping type
        typedef detail::location_mapper_binary_op<R, animray::point2d<S>>
                location_mapping_functor_type;
        /// Texture constructor functor type
        typedef R (*texture_constructor_arg1_type)(S, S);
    };


}


#endif // ANIMRAY_DETAIL_TEXTURE_POLICY_BASED
