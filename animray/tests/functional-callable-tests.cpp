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


#include <animray/functional/callable.hpp>
#include <animray/point3d.hpp>
#include <animray/test.hpp>


FSL_TEST_SUITE(functional_callable);


FSL_TEST_FUNCTION(accessors) {
    typedef fostlib::accessors<int> accessor;
    FSL_CHECK(std::is_class<accessor>::value);
    FSL_CHECK(animray::detail::is_callable_impl<accessor>::value);
    FSL_CHECK(animray::is_callable<accessor>::value);
}


FSL_TEST_FUNCTION(simple) {
    class simple {};
    FSL_CHECK(std::is_class<simple>::value);
    FSL_CHECK(not animray::detail::is_callable_impl<simple>::value);
    FSL_CHECK(not animray::is_callable<simple>::value);
}


FSL_TEST_FUNCTION(array) {
    typedef animray::detail::array_based<int, 4> array;
    FSL_CHECK(std::is_class<array>::value);
    FSL_CHECK(not animray::detail::is_callable_impl<array>::value);
    FSL_CHECK(not animray::is_callable<array>::value);
}


FSL_TEST_FUNCTION(point3d) {
    typedef animray::point3d<int> point;
    FSL_CHECK(std::is_class<point>::value);
    FSL_CHECK(not animray::detail::is_callable_impl<point>::value);
    FSL_CHECK(not animray::is_callable<point>::value);
}
