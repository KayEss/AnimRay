/*
    Copyright 2014-2015, Kirit Saelensminde.
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


#ifndef ANIMRAY_ANIMATION_ANIMATION_HPP
#define ANIMRAY_ANIMATION_ANIMATION_HPP
#pragma once


#include <utility>
#include <functional>


namespace animray {


    /// Wrapper for an attribute to make it animatable
    template<typename V>
    class animatable {
        V starting_value;

      public:
        /// The type of value that is animated
        typedef V value_type;

        /// Pass on constructor arguments
        template<typename... A>
        animatable(A &&... a) : starting_value(std::forward<A>(a)...) {}

        /// Return the value
        template<typename T>
        value_type operator()(const T &) const {
            return starting_value;
        }
    };


    /// Base for attaching animations to attributes
    template<typename T>
    class animate : public T {
      public:
        /// Pass constructor arguments to superclass
        template<typename... A>
        animate(A &&... args) : T(std::forward<A>(args)...) {}

        /// Strip the frame out of the ray type
        template<typename R>
        auto operator()(const R &ray) const {
            return T::operator()(ray.frame());
        }
    };


    /// Specialisation for animating using a functor
    template<typename T, typename F>
    class animate<std::function<T(F)>> {
        std::function<T(F)> function;

      public:
        /// Default construct return a default T
        animate<std::function<T(F)>>() : function([](F) { return T(); }) {}
        /// Initialise with the lambda to use
        animate<std::function<T(F)>>(std::function<T(F)> f) : function(f) {}

        /// The type of the value of R
        /// TODO: We don't really want this here
        typedef typename T::value_type value_type;

        /// Strip the frame out of the ray type
        template<typename R>
        auto operator()(const R &ray) const {
            return function(ray.frame());
        }
        /// We already have a frame number
        auto operator()(const F f) const { return function(f); }
    };


}


#endif // ANIMRAY_ANIMATION_ANIMATION_HPP
