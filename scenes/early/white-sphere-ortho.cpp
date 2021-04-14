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
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>


int main(int argc, char const *const argv[]) {
    auto const args = animray::cli::arguments{
            argc, argv, "white-sphere-ortho.tga", 1920, 1080};
    double const exposure = args.switch_value('e', 1.0);

    double const aspect = double(args.width) / args.height;
    double const fw = args.width > args.height ? aspect * 2.0 : 2.0;
    double const fh = args.width > args.height ? 2.0 : 2.0 / aspect;

    using ray = animray::ray<double>;
    animray::unit_sphere_at_origin<ray> sphere;
    using film_type = animray::film<animray::luma<std::uint8_t>>;
    animray::ortho_camera<ray> camera(fw, fh, args.width, args.height, -9, 1);
    film_type output(
            args.width, args.height,
            [=, &sphere](
                    const film_type::size_type x, const film_type::size_type y) {
                ray r(camera(x, y));
                auto intersection(sphere.intersects(r, 0.0));
                if (intersection) {
                    ray light(
                            intersection->from, ray::end_type(2.0, 2.0, -5.0));
                    if (sphere.occludes(light, 1e-9)) {
                        return animray::luma<uint8_t>(50);
                    } else {
                        const double costheta =
                                dot(light.direction, intersection->direction);
                        return animray::luma<uint8_t>(std::clamp<float>(
                                (50 + 205 * costheta) * exposure, 0, 255));
                    }
                } else {
                    return animray::luma<uint8_t>(0);
                }
            });
    animray::targa(args.output_filename, output);

    return 0;
}
