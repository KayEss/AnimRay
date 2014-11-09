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


#include <animray/maths/primes.hpp>
#include <fost/test>


FSL_TEST_SUITE( factors );


FSL_TEST_FUNCTION( is_prime ) {
    FSL_CHECK( animray::is_prime(2) );
    FSL_CHECK( animray::is_prime(3) );
    FSL_CHECK( !animray::is_prime(4) );
    FSL_CHECK( animray::is_prime(5) );
    FSL_CHECK( !animray::is_prime(6) );
    FSL_CHECK( animray::is_prime(7) );
    FSL_CHECK( !animray::is_prime(8) );
    FSL_CHECK( !animray::is_prime(9) );
    FSL_CHECK( !animray::is_prime(10) );
    FSL_CHECK( animray::is_prime(11) );
    FSL_CHECK( !animray::is_prime(12) );
    FSL_CHECK( animray::is_prime(13) );
    FSL_CHECK( !animray::is_prime(14) );
    FSL_CHECK( !animray::is_prime(15) );
    FSL_CHECK( !animray::is_prime(16) );
    FSL_CHECK( animray::is_prime(17) );
    FSL_CHECK( !animray::is_prime(18) );
    FSL_CHECK( animray::is_prime(19) );
    FSL_CHECK( !animray::is_prime(20) );
}


FSL_TEST_FUNCTION( prime_generator ) {
    boost::function0< int > gen( animray::prime_generator< int >() );
    FSL_CHECK_EQ( gen(), 2 );
    FSL_CHECK_EQ( gen(), 3 );
    FSL_CHECK_EQ( gen(), 5 );
    FSL_CHECK_EQ( gen(), 7 );
    FSL_CHECK_EQ( gen(), 11 );
    FSL_CHECK_EQ( gen(), 13 );
}


FSL_TEST_FUNCTION( prime_factors ) {
    std::vector< int > p10 = animray::prime_factors(10);
    FSL_CHECK_EQ( p10.size(), 2u );
    FSL_CHECK_EQ( p10[0], 2 );
    FSL_CHECK_EQ( p10[1], 5 );

    std::vector< int > p17 = animray::prime_factors(17);
    FSL_CHECK_EQ( p17.size(), 1u );
    FSL_CHECK_EQ( p17[0], 17 );

    std::vector< int > p600 = animray::prime_factors(600);
    FSL_CHECK_EQ( p600.size(), 6u );
    FSL_CHECK_EQ( p600[0], 2 );
    FSL_CHECK_EQ( p600[1], 2 );
    FSL_CHECK_EQ( p600[2], 2 );
    FSL_CHECK_EQ( p600[3], 3 );
    FSL_CHECK_EQ( p600[4], 5 );
    FSL_CHECK_EQ( p600[5], 5 );
}

