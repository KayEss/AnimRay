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
        const D aspect, weight;
        const D ox, oy, sz;
        const std::size_t bits;
        const unsigned int mask;
        iterations( const F &film, D x, D y, D s, std::size_t bits )
        : size( film.size() ),
                aspect( D(size.width()) / D(size.height()) ),
                weight( D(1) / std::max( size.width(), size.height() ) ),
                ox(x - s * aspect),
                oy(y - s),
                sz( s * D(2) ),
                bits( bits ), mask(  ( 0x1 << bits ) - 1 ) {
        }
        typename F::color_type scale( unsigned int v ) const {
            if ( bits < 8 )
                return v << (8-bits);
            else
                return v >> (bits-8);
        }
        typename F::color_type operator () (
            const F &,
            const typename F::extents_type::corner_type &loc,
            const typename F::color_type &
        ) const {
            const D proportion_x = D( loc.x() ) * weight;
            const D proportion_y = D( loc.y() ) * weight;
            const D x = proportion_x * sz + ox;
            const D y = proportion_y * sz + oy;
            const std::complex< D > position( x, y );
            unsigned int counter = 1;
            for ( std::complex< D > current( position );
                std::norm(current) < D(4) && counter > 0;
                current = current * current + position
            ) counter = ( counter + 1 ) & mask;
            return scale(counter);
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

    typedef double precision;
    precision centre_x = fostlib::coerce< double >(
        args.commandSwitch("cx").value("0")
    );
    precision centre_y = fostlib::coerce< double >(
        args.commandSwitch("cy").value("0")
    );
    precision radius = fostlib::coerce< double >(
        args.commandSwitch("r").value("2")
    );
    std::size_t bits = fostlib::coerce< int >(
        args.commandSwitch("bits").value("8")
    );

    out << "Centre image at " << centre_x << ", " << centre_y <<
        " with radius of " << radius <<
        " to " << bits << " bits" <<
    std::endl;

    output.transform( iterations< film_type, precision >(
        output, centre_x, centre_y, radius, bits
    ) );

    animray::targa(output_filename, output);

    return 0;
}
