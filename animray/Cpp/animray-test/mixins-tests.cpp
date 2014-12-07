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


#include <animray/mixins/depth-count.hpp>
#include <fost/test>


FSL_TEST_SUITE(mixins);


FSL_TEST_FUNCTION(depth_count) {
    class base {};
    static_assert(not std::is_same<animray::with_depth_count<base>::type, base>::value);
    animray::with_depth_count<base>::type counted;
    FSL_CHECK_EQ(counted.depth_count(), 1u);
}

