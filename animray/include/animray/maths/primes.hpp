/*
    Copyright 2010-2018, Kirit Saelensminde.
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


#ifndef ANIMRAY_MATHS_PRIME_BASED
#define ANIMRAY_MATHS_PRIME_BASED
#pragma once


#include <fost/core>

#include <boost/bind.hpp>


namespace animray {


    /// A simple prime number test that is fairly efficient
    template< typename I >
    bool is_prime( I v ) {
        if ( v % 2 == 0 )
            return v == 2;
        else {
            I u( std::sqrt( double(v) ) );
            for ( I i = 3; i <= u; i += 2 )
                if ( v % i == 0 )
                    return false;
            return true;
        }
    }


    namespace detail {


        /// A prime number generator with implicit state in a closure
        template< typename I >
        I next_prime( I &i ) {
            while ( !is_prime( i++ ) );
            return i - 1;
        }


    }


    /// A meta-function that returns a prime number generator
    template< typename I >
    auto prime_generator() {
        return [next=2]() mutable {
            return detail::next_prime(next);
        };
    }


    /// Returns a list of prime factors for a value
    template< typename I >
    std::vector< I > prime_factors( I v ) {
        std::vector< I > factors;
        auto gen(prime_generator<I>());
        I factor = gen();
        while ( v > 1 ) {
            if ( v % factor == 0 ) {
                factors.push_back(factor);
                v = v / factor;
            } else {
                factor = gen();
            }
        }
        return factors;
    }


}


#endif // ANIMRAY_MATHS_PRIME_BASED
