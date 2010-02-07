/*
    Copyright 1995-2010, Kirit Saelensminde.
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


#ifndef ANIMRAY_RENDER_HPP
#define ANIMRAY_RENDER_HPP
#pragma once


#include <animray/raster.hpp>
#include <animray/texture.hpp>


namespace animray {


    /// Render a texture to a raster
    template<
        typename T,
        typename R = raster< typename T::color_type >
    > class render {
        T texture;
        public:
            /// The type of the texture to be rendered
            typedef T texture_type;

            /// Construct a render command from a texture functor
            render(
                const typename texture_type::functor_type &f
            ) : texture( f ) {
            }
    };


}


#endif // ANIMRAY_RENDER_HPP
