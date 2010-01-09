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


#include <fost/unicode>
#include <fost/main>
#include <fost/coerce/ints.hpp>
#include <animray/targa>
#include <boost/random.hpp>


namespace {
    template< typename F >
    typename F::color_type circle(
        typename F::size_type px,
        typename F::size_type py,
        typename F::size_type r,
        typename F::size_type x,
        typename F::size_type y,
        const typename F::color_type &current
    ) {
        if ( current < 0xff && (px-x) * (px-x) + (py-y) * (py-y) < r * r )
            return current + 1;
        else
            return current;
    }

    template< typename F >
    class do_elevation {
        F &film;
        boost::mt19937 rng;
        void operator () (
            const animray::extents2d< int > &size,
            typename F::size_type radius
        ) {
            // Create a random number handler across the range we need
            boost::uniform_int<>
                uix(size.lower_left().x(), size.top_right().x()),
                uiy(size.lower_left().y(), size.top_right().y())
            ;
            boost::variate_generator<boost::mt19937&,
                boost::uniform_int<>
            > rx( rng, uix ), ry( rng, uiy );

            // Calculate the position and extents of the circle
            typename F::size_type x(rx()), y(ry());
            animray::extents2d< int > location(
                x - radius/2, y - radius/2,
                x + radius/2, y + radius/2
            );

            // Raise the land covered by the circle
            film.for_each( boost::lambda::bind(
                &circle< F >, x, y, radius,
                boost::lambda::_1, boost::lambda::_2, boost::lambda::_3
            ) );

            // Now recursivley do some more
            if ( radius > 2 )
                for ( std::size_t s = 0; s < 3; ++s )
                    (*this)(location, radius / 2 );
        }
        public:
            do_elevation( F &film )
            : film( film ) {
                (*this)(
                    fostlib::coerce< animray::extents2d< int > >(film.size()),
                    std::max(film.width(), film.height()) / 2
                );
            }
    };

    template< typename F >
    void elevate( F &film ) {
        do_elevation< F > functor(film);
    }
}


FSL_MAIN(
    L"landmaker",
    L"LandMaker, Copyright 2010 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("out.tga"))
    ;
    int width = fostlib::coerce< int >( args[2].value("100") );
    int height = fostlib::coerce< int >( args[3].value("100") );
    out << "Creating image " << output_filename
        <<", size " << width << " x " << height << std::endl
    ;

    typedef animray::film< uint8_t > film_type;
    film_type output(width, height);

    elevate(output);

    animray::targa(output_filename, output);

    return 0;
}
