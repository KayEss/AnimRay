/*
    Copyright 2010, Kirit Saelensminde.
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


#include <fost/python>

#include <animray/film.hpp>
#include <animray/mandelbrot.hpp>


namespace {
    template< typename F > inline
    typename F::color_type film_get_xy(
        F const *f, typename F::size_type x, typename F::size_type y
    ) {
        return (*f)[x][y];
    }
    template< typename F > inline
    typename F::color_type film_set_xy(
        F *f, typename F::size_type x, typename F::size_type y,
        typename F::color_type c
    ) {
        return (*f)[x][y] = c;
    }

    template< typename F, typename M > inline
    boost::shared_ptr< F > generate_mandelbrot( M *m ) {
        boost::shared_ptr< F > f( new F( m->width, m->height ) );
        f->transform( *m );
        return f;
    }
}
BOOST_PYTHON_MODULE( _animray ) {
    using namespace boost::python;
    fostlib::python_string_registration();
    fostlib::python_json_registration();

    class_<
        animray::film< uint8_t >,
        boost::shared_ptr< animray::film< uint8_t > >,
        boost::noncopyable
    >(
        "film_gray8",
        init< std::size_t, std::size_t >()
    )
        .def(init< std::size_t, std::size_t, uint8_t >())
        .add_property("width",
            fostlib::accessors_getter<
                animray::film< uint8_t >,
                const animray::film< uint8_t >::size_type,
                &animray::film< uint8_t >::width
            >
        )
        .add_property("height",
            fostlib::accessors_getter<
                animray::film< uint8_t >,
                const animray::film< uint8_t >::size_type,
                &animray::film< uint8_t >::height
            >
        )
        .def("__call__", film_get_xy< animray::film< uint8_t > >)
        .def("__call__", film_set_xy< animray::film< uint8_t > >)
    ;

    class_<
        animray::mandelbrot::iterations< animray::film< uint8_t >, double >
    >(
        "mandelbrot_gray8",
        init<
            animray::film< uint8_t >::size_type, animray::film< uint8_t >::size_type,
            double , double, double, std::size_t
        >()
    )
        .add_property("width",
            &animray::mandelbrot::iterations< animray::film< uint8_t >, double >::width
        )
        .add_property("height",
            &animray::mandelbrot::iterations< animray::film< uint8_t >, double >::height
        )
        .add_property("center_x",
            &animray::mandelbrot::iterations< animray::film< uint8_t >, double >::cx
        )
        .add_property("center_y",
            &animray::mandelbrot::iterations< animray::film< uint8_t >, double >::cy
        )
        .add_property("radius",
            &animray::mandelbrot::iterations< animray::film< uint8_t >, double >::sz
        )
        .def("__call__",
            generate_mandelbrot<
                animray::film< uint8_t >,
                animray::mandelbrot::iterations< animray::film< uint8_t >, double >
            >
        )
    ;
}
