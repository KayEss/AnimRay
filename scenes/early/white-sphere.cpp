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


#include <animray/cli/main.hpp>
#include <animray/formats/targa.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>


int main(int argc, char const *const argv[]) {
    auto const args =
            animray::cli::arguments{argc, argv, "white-sphere.tga", 1920, 1080};

    /**
     * The sphere has a radius of two units, so we want to make sure we
     * can fit it into the frame. This calculates the scaling factor to apply.
     */
    const double limit = std::min(args.width, args.height) / 2.0;

    using ray = animray::ray<double>;
    auto constexpr const sphere = animray::unit_sphere_at_origin<ray>{};
    auto constexpr const light_position = ray::end_type{4.0, 4.0, -5.0};
    auto constexpr const ambient = animray::luma<>{50};

    auto const output = animray::film<animray::luma<>>{
            args.width, args.height, [=](auto const x, auto const y) {
                /**
                 * Map the current pixel to a position in the -1 to +1 range
                 * based on the `limit` which tells us the scaling factor to
                 * use.
                 */
                auto const cx = (double(x) + 0.5 - args.width / 2.0) / limit;
                auto const cy = -(double(y) + 0.5 - args.height / 2.0) / limit;
                /**
                 * Fire the rays parallel towards the sphere. This causes us
                 * to use an orthographic projection of the geometry to
                 * the camera co-ordinate system.
                 */
                ray r({cx, cy, -10.0}, ray::end_type{cx, cy, -9.0});
                /**
                 * Calculate the intersections and amount of light.
                 */
                auto const intersection = sphere.intersects(r);
                if (intersection) {
                    ray light(intersection->from, light_position);
                    if (sphere.occludes(light)) {
                        return ambient;
                    } else {
                        auto const costheta =
                                dot(light.direction, intersection->direction);
                        return ambient + animray::luma<>{205 * costheta};
                    }
                } else {
                    return animray::luma<>{0};
                }
            }};
    animray::targa(args.output_filename, output);

    return 0;
}
