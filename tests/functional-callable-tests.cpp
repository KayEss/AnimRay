/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    class simple {};
    static_assert(std::is_class<simple>::value);
    static_assert(not animray::detail::is_callable_impl<simple>::value);
    static_assert(not animray::is_callable<simple>::value);

    using array = animray::detail::array_based<int, 4>;
    static_assert(std::is_class<array>::value);
    static_assert(not animray::detail::is_callable_impl<array>::value);
    static_assert(not animray::is_callable<array>::value);

    using point = animray::point3d<int>;
    static_assert(std::is_class<point>::value);
    static_assert(not animray::detail::is_callable_impl<point>::value);
    static_assert(not animray::is_callable<point>::value);


}
