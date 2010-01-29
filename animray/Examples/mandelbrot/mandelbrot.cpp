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
#include <animray/targa.hpp>

#include <complex>


namespace {
    template< typename F, typename D >
    struct iterations {
        const typename F::extents_type size;
        iterations( const F &film )
        : size( film.size() ) {
        }
        typename F::color_type operator () (
            const F &,
            const typename F::extents_type::corner_type &loc,
            const typename F::color_type &
        ) const {
            const D proportion_x = D( loc.x() ) / D( size.width() );
            const D proportion_y = D( loc.y() ) / D( size.height() );
            const D x = proportion_x * D(4) - D(2);
            const D y = proportion_y * D(4) - D(2);
            const std::complex< D > position( x, y );
            typename F::color_type counter = 1;
            for ( std::complex< D > current( position );
                std::norm(current) < D(4) && counter > 0;
                current = current * current + position
            ) ++counter;
            return counter;
        }
    };
}


FSL_MAIN(
    L"mandelbrot",
    L"Mandelbrot, Copyright 2010 Kirit Saelensminde"
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

    output.transform( iterations< film_type, double >(output) );

    animray::targa(output_filename, output);

    return 0;
}
