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


#ifndef ANIMRAY_FUNCTIONAL_CALLABLE_HPP
#define ANIMRAY_FUNCTIONAL_CALLABLE_HPP
#pragma once


#include <type_traits>


namespace animray {


    namespace detail {
        template < class T >
        class HasMember_op_call
        {
        private:
            using Yes = char[2];
            using  No = char[1];

            struct Fallback { int operator()(); };
            struct Derived : T, Fallback { };

            template < class U >
            static No& test ( decltype(U::operator()())* );
            template < typename U >
            static Yes& test ( U* );

        public:
            static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes);
        };

        template < class T >
        struct is_callable_impl
        : public std::integral_constant<bool, HasMember_op_call<T>::RESULT>
        { };
    }

    /// Returns the type or the value true if the item is callable
    template<typename T>
    struct is_callable
        : std::conditional<
            std::is_class<T>::value,
            detail::is_callable_impl<T>,
            std::is_function<T>
        >::type
    {};


}


#endif // ANIMRAY_FUNCTIONAL_CALLABLE_HPP
