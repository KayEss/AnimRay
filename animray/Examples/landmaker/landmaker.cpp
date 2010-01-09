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
#include <animray/targa>
#include <boost/lambda/bind.hpp>


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
        if ( (px-x) * (px-x) + (py-y) * (py-y) < r * r )
            return 0xff;
        else
            return current;
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
    film_type output(width, height, 0x80);
    output.for_each( boost::lambda::bind(
        &circle< film_type >,
        50, 25, 10,
        boost::lambda::_1, boost::lambda::_2, boost::lambda::_3
    ) );

    animray::targa(output_filename, output);

    return 0;
}
