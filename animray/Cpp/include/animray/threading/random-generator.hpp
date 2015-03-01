/*
    Copyright 2015, Kirit Saelensminde.
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


#pragma once


#include <random>


namespace animray {


    /// Wrapper for a std:: random engine so it can be used as a template
    /// argument and will be initialised properly and be thread safe
    template< typename E = std::mt19937 >
    struct random_engine {
        /// The type of the engine
        typedef E engine_type;

        /// The engine itself
        thread_local static E engine;
    };


    template< typename E >
    thread_local E random_engine<E>::engine{std::random_device{}()};


    /// Wrapper for a distribution so it can be used as a template
    /// argument and produce numbers in a thread safe manner
    template< typename D, typename D::param_type *P,
        typename E = random_engine<> >
    struct random_distribution {
        static typename D::result_type value() {
            thread_local D distribution(*P);
            return distribution(E::engine);
        }
    };


}

