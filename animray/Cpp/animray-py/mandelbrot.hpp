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

#include <animray/mandelbrot.hpp>


namespace {


    template< typename F, typename M > inline
    boost::shared_ptr< F > generate_mandelbrot( M *m ) {
        boost::shared_ptr< F > f( new F( m->width, m->height ) );
        f->transform( *m );
        return f;
    }

    template< typename C >
    void mandelbrot(const char *name) {
        using namespace boost::python;

        class_<
            animray::mandelbrot::iterations< animray::film< C >, double >
        >(
            name,
            init<
                typename animray::film< C >::size_type,
                typename animray::film< C >::size_type,
                double , double, double, std::size_t
            >()
        )
            .add_property("width",
                &animray::mandelbrot::iterations< animray::film< C >, double >::width
            )
            .add_property("height",
                &animray::mandelbrot::iterations< animray::film< C >, double >::height
            )
            .add_property("center_x",
                &animray::mandelbrot::iterations< animray::film< C >, double >::cx
            )
            .add_property("center_y",
                &animray::mandelbrot::iterations< animray::film< C >, double >::cy
            )
            .add_property("radius",
                &animray::mandelbrot::iterations< animray::film< C >, double >::sz
            )
            .def("__call__",
                generate_mandelbrot<
                    animray::film< C >,
                    animray::mandelbrot::iterations< animray::film< C >, double >
                >
            )
        ;
    }


}
