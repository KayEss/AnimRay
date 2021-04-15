/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/camera/ortho.hpp>
#include <animray/cli/main.hpp>
#include <animray/epsilon.hpp>
#include <animray/formats/targa.hpp>
#include <animray/geometry/planar/plane.hpp>
#include <animray/ray.hpp>


int main(int argc, char const *const argv[]) {
    auto const args = animray::cli::arguments{
            argc, argv, "white-plane-ortho.tga", 1920, 1080};

    const double size = 20.0;
    const double aspect = double(args.width) / args.height;
    const double fw = args.width > args.height ? aspect * size : size;
    const double fh = args.width > args.height ? size : size / aspect;

    using ray = animray::ray<double>;
    animray::plane<ray> plane;
    using film_type = animray::film<animray::rgb<uint8_t>>;
    animray::ortho_camera<ray> camera(fw, fh, args.width, args.height, -9, 1);
    film_type output(
            args.width, args.height,
            [=, &plane](
                    const film_type::size_type x, const film_type::size_type y) {
                ray r(camera(x, y));
                ray intersection(plane.intersects(r, 0.0).value());
                ray light(intersection.from, ray::end_type(5.0, 5.0, -5.0));
                const double costheta =
                        dot(light.direction, intersection.direction);
                return animray::rgb<uint8_t>(50 + 205 * costheta);
            });
    animray::targa(args.output_filename, output);

    return 0;
}
