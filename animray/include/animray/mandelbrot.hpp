/*
    Copyright 2010-2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_MANDELBROT_HPP
#define ANIMRAY_MANDELBROT_HPP
#pragma once


#include <fost/core>
#include <complex>
#include <functional>


namespace animray {


    namespace mandelbrot {


        /// A film transformation functor implementing the mandelbrot
        template<typename F, typename D>
        struct transformer {
            const typename F::size_type width, height;
            const D center_x, center_y, diameter, per_pixel;
            const std::size_t bits;
            typedef std::function<typename F::color_type(
                    unsigned int, std::size_t)>
                    colour_constructor;
            colour_constructor cons;

            transformer(
                    typename F::size_type width,
                    typename F::size_type height,
                    D x,
                    D y,
                    D s,
                    std::size_t bits,
                    colour_constructor fn =
                            [](unsigned int d, std::size_t bits) {
                                // Scale to 0-255 range
                                if (bits < 8) {
                                    return typename F::color_type(
                                            d << (8 - bits));
                                } else {
                                    return typename F::color_type(
                                            d >> (bits - 8));
                                }
                            })
            : width(width),
              height(height),
              center_x(x),
              center_y(y),
              diameter(s),
              per_pixel(s / std::min(width, height)),
              bits(bits),
              cons(fn) {}

            typedef typename F::color_type result_type;
            typedef typename F::size_type arg1_type;
            typedef typename F::size_type arg2_type;

            typename F::color_type operator()(
                    const typename F::size_type lx,
                    const typename F::size_type ly) const {
                const D x = (D(lx) - D(width) / D(2)) * per_pixel;
                const D y = (D(ly) - D(height) / D(2)) * per_pixel;
                const std::complex<D> position(x + center_x, y + center_y);
                const unsigned int mask = (1u << bits) - 1u;
                unsigned int counter = 1;
                for (std::complex<D> current(position);
                     std::norm(current) < D(4) && counter > 0;
                     current = current * current + position) {
                    counter = (counter + 1) & mask;
                }
                return cons(counter, bits);
            }
        };


    }


}


#endif // ANIMRAY_MANDELBROT_HPP
