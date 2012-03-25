/*
    Copyright 2012, Kirit Saelensminde.
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


#include <metachatr/runtime.hpp>


namespace {
    class mandelbrot_impl : public metachatr::detail::lambda_impl {
        metachatr::lambda_result operator() (
            metachatr::scope scope, const metachatr::argument_tuple &args
        ) {
            throw fostlib::exceptions::not_implemented(
                "mandelbrot_impl::operator() (metachatr::scope, const metachatr::argument_tuple &)");
        }
        metachatr::jexpression as_jexpression() const {
            throw fostlib::exceptions::not_implemented(
                "mandelbrot_impl::as_jexpression()");
        }
    };
    boost::shared_ptr< metachatr::detail::lambda_impl >
        c_mandelbrot_impl( new mandelbrot_impl );
}


metachatr::lambda mandelbrot() {
    return c_mandelbrot_impl;
}

