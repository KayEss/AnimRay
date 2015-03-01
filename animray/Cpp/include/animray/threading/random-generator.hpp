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


    inline namespace random {


        /// Wrapper for a std:: random engine so it can be used as a template
        /// argument and will be initialised properly and be thread safe
        template<
            typename E = std::mt19937,
            typename D = std::random_device
        >
        struct engine {
            /// The type of the engine
            typedef E engine_type;

            /// The engine itself
            thread_local static E e;
        };


        template< typename E, typename D >
        thread_local E engine<E, D>::e{D{}()};


        /// Wrapper for a distribution so it can be used as a template
        /// argument and produce numbers in a thread safe manner
        template< typename D, const typename D::param_type *P,
            typename E = engine<> >
        struct distribution {
            static typename D::result_type value() {
                thread_local D d(*P);
                return d(E::e);
            }
        };


        /// Template wrapper to store distribution arguments so that they
        /// can be used as arguments for a template parameter.
        inline namespace parameters {


            /// One unit
            template< typename D >
            struct zero_to_one {
                static const typename D::param_type parameter;
            };
            template<>
            const std::uniform_real_distribution<float>::param_type
                zero_to_one<std::uniform_real_distribution<float>>::
                    parameter{0.f, 1.f};
            template<>
            const std::uniform_real_distribution<double>::param_type
                zero_to_one<std::uniform_real_distribution<double>>::
                    parameter{-0., 1.};
            template<>
            const std::uniform_real_distribution<long double>::param_type
                zero_to_one<std::uniform_real_distribution<long double>>::
                    parameter{-0.l, 1.l};


            /// One unit distributed about the zero point
            template< typename D >
            struct plus_minus_half {
                static const typename D::param_type parameter;
            };
            template<>
            const std::uniform_real_distribution<float>::param_type
                plus_minus_half<std::uniform_real_distribution<float>>::
                    parameter{-0.5f, 0.5f};
            template<>
            const std::uniform_real_distribution<double>::param_type
                plus_minus_half<std::uniform_real_distribution<double>>::
                    parameter{-0.5, 0.5};
            template<>
            const std::uniform_real_distribution<long double>::param_type
                plus_minus_half<std::uniform_real_distribution<long double>>::
                    parameter{-0.5l, 0.5l};


        }


    }


}

