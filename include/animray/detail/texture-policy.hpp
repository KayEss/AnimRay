/**
    Copyright 2010-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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
        result_type value;
        /// Call the functor
        const result_type &operator()() const { return value; }
    };


    /// Identiry functor
    template<typename V>
    struct identity {
        /// The result type
        using result_type = V;
        /// The argument type
        using arg1_type = V;
        /// The identity function
        const result_type &operator()(const result_type &v) const { return v; }
    };


    /// A functor that calls coerce -- (TODO maybe to be generalised)
    template<typename T, typename F>
    struct coercer {
        /// The result type
        using result_type = T;
        /// The argument type
        using arg1_type = F;
        /// The coercion function
        result_type operator()(const F &f) const { return convert_to<T>(f); }
    };


    /// A unary functor that calls another unary functor
    template<typename R, typename A1>
    struct apply {
        /// The result type
        using result_type = R;
        /// The argument type
        using arg1_type = A1;
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
        using result_type = R;
        /// The argument type
        using arg1_type = A1;
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
        using color_type = C;
        /// The location co-ordinate type
        using location_type = L;
        /// The functor type
        using functor_type = F;
        /// The colour conversion functor type
        using color_conversion_functor_type =
                coercer<C, typename F::result_type>;
        /// The location mapping type
        using location_mapping_functor_type = apply<typename F::result_type, L>;
        /// Texture constructor functor type
        using texture_constructor_arg1_type = F;
    };


    /// Specialisation of the policy where there a constant value in the texture
    template<typename C, typename L>
    struct texture_policy<C, L, const_value<C>> {
        /// The colour type
        using color_type = C;
        /// The location co-ordinate type
        using location_type = L;
        /// The functor type
        using functor_type = const_value<C>;
        /// The colour conversion functor type
        using color_conversion_functor_type = identity<C>;
        /// The location mapping type
        using location_mapping_functor_type = apply_without_arguments<C, L>;
        /// Texture constructor functor type
        using texture_constructor_arg1_type = C;
    };


    namespace detail {
        /// A location mapper that splits the x and y for a binary functor
        template<typename R, typename A1>
        struct location_mapper_binary_op {
            /// The result type
            using result_type = R;
            /// The argument type
            using arg1_type = A1;
            /// Convert the extents to x, y and call f
            template<typename F>
            R operator()(const F &f, const A1 &l) const {
                return f(l.x, l.y);
            }
        };
    }
    /// Specialisation of the policy for binary functions
    template<typename C, typename S, typename R>
    struct texture_policy<C, animray::point2d<S>, std::function<R(S, S)>> {
        /// The colour type
        using color_type = C;
        /// The location co-ordinate type
        using location_type = animray::point2d<S>;
        /// The functor type
        using functor_type = std::function<R(S, S)>;
        /// The colour conversion functor type
        using color_conversion_functor_type = coercer<C, R>;
        /// The location mapping type
        using location_mapping_functor_type =
                detail::location_mapper_binary_op<R, animray::point2d<S>>;
        /// Texture constructor functor type
        using texture_constructor_arg1_type = R (*)(S, S);
    };


}


#endif // ANIMRAY_DETAIL_TEXTURE_POLICY_BASED
