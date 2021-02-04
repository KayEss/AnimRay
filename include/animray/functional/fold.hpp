/*
    Copyright 2018, Kirit Saelensminde.
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


#pragma once


namespace animray {


    /**
     * # Fold.
     *
     * Folds a binary operator over a parameter pack and return the
     * result.
     */
    template<typename Op, typename Var, typename... Pack>
    decltype(auto) foldl(Op &&op, Var i1, Var i2, Pack... ip) {
        if constexpr (sizeof...(ip)) {
            return foldl(op, op(i1, i2), ip...);
        } else {
            return op(i1, i2);
        }
    }


}
