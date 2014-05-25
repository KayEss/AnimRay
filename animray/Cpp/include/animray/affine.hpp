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


#ifndef ANIMRAY_AFFINE_HPP
#define ANIMRAY_AFFINE_HPP
#pragma once


#include <animray/matrix.hpp>


namespace animray {


    template<typename W>
    std::pair<matrix<W>, matrix<W>> translate(W tx, W ty, W tz) {
        matrix<W> f, b;
        f[0][3] = tx; b[0][3] = -tx;
        f[1][3] = ty; b[1][3] = -ty;
        f[2][3] = tz; b[2][3] = -tz;
        return std::make_pair(f, b);
    }


}


#endif // ANIMRAY_AFFINE_HPP
