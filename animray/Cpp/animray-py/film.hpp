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

    template< typename C >
    void film_py(const char *name) {
        using namespace boost::python;
        class_<
            animray::film< C >,
            boost::shared_ptr< animray::film< C > >,
            boost::noncopyable
        >(
            name, init< std::size_t, std::size_t >()
        )
            .def(init< std::size_t, std::size_t, C >())
            .add_property("width", &animray::film< C >::width)
            .add_property("height", &animray::film< C >::height)
            .def("__call__", film_get_xy< animray::film< C > >)
            .def("__call__", film_set_xy< animray::film< C > >)
        ;
    }


}
