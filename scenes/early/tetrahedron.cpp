/**
    Copyright 2014-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <fost/main>
#include <fost/progress-cli>
#include <fost/unicode>
#include <animray/library/lights/block.hpp>
#include <animray/animation/procedural/affine.hpp>
#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/camera/movie.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/intersection.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/geometry/planar/triangle.hpp>
#include <animray/scene.hpp>
#include <animray/targa.hpp>
#include <animray/threading/sub-panel.hpp>

#include <thread>


FSL_MAIN("animray", "AnimRay. Copyright 2010-2020 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    const std::size_t threads(
            fostlib::coerce<fostlib::nullable<int>>(args.commandSwitch("t"))
                    .value_or(std::thread::hardware_concurrency()));
    const std::size_t samples(
            fostlib::coerce<int>(args.commandSwitch("ss").value_or("2")));
    const std::size_t frames(
            fostlib::coerce<int>(args.commandSwitch("frames").value_or("2")));

    /// ### Output handling
    /// Filenames
    const int width = fostlib::coerce<int>(args[1].value_or("96"));
    const int height = fostlib::coerce<int>(args[2].value_or("54"));
    auto const output_filename = fostlib::coerce<fostlib::fs::path>(
            args[3].value_or("tetrahedron.tga"));

    /// Screen aspect and pixel density
    auto const aspect = static_cast<double>(width) / height;
    auto const fw = width > height ? aspect * 0.024 : 0.024;
    auto const fh = width > height ? 0.024 : 0.024 / aspect;

    /// ## Set up the geometry
    using world = float;
    std::size_t const angle{20};

    /// Start with the apexes of the triangles for the shape.
    animray::point3d<world> constexpr top(0, 0, 1), bottom(0, 0, -1),
            north(0, 1, 0), south(0, -1, 0), east(1, 0, 0), west(-1, 0, 0);
    /// Then put them together into the triangles we require
    using triangle = animray::triangle<animray::ray<world>>;
    auto const tetrahedron = animray::animation::affine{
            animray::rotate_z<world>, 40_deg, 1_deg * angle, frames,
            animray::animation::affine{
                    animray::rotate_y<world>, 0, 2_deg * angle, frames,
                    animray::collection{animray::make_array(
                            triangle{top, north, east},
                            triangle{top, east, south},
                            triangle{top, south, west},
                            triangle{top, west, north},
                            triangle{bottom, north, east},
                            triangle{bottom, east, south},
                            triangle{bottom, south, west},
                            triangle{bottom, west, north})}}};

    auto const scene = animray::scene{
            tetrahedron, animray::library::lights::narrow_block<world>,
            animray::rgb<float>{20, 70, 100}};

    for (std::size_t frame{}; frame != frames * 360 / angle; ++frame) {
        animray::movable<
                animray::stacatto_movie<animray::pinhole_camera<
                        animray::ray<world>, animray::flat_jitter_camera<world>>>,
                typename animray::with_frame<
                        animray::ray<world>, std::size_t>::type>
                camera(fw, fh, width, height, 0.05);
        camera(animray::rotate_x<world>(-15_deg));
        camera(animray::translate<world>(0.0, 0.0, -4));
        camera.instance.frame = frame;

        typedef animray::film<animray::rgb<uint8_t>> film_type;

        fostlib::worker worker;
        fostlib::meter tracking;
        fostlib::future<film_type> result(worker.run<film_type>(
                [threads, samples, width, height, &scene, &camera]() {
                    return animray::threading::sub_panel<film_type>(
                            threads, width, height,
                            [samples, &scene, &camera](
                                    const film_type::size_type x,
                                    const film_type::size_type y) {
                                animray::rgb<float> photons;
                                for (std::size_t sample{}; sample != samples;
                                     ++sample) {
                                    photons += scene(camera, x, y) /= samples;
                                }
                                const float exposure = 1.4f;
                                photons /= exposure;
                                return animray::rgb<uint8_t>(
                                        uint8_t(photons.red() > 255
                                                        ? 255
                                                        : photons.red()),
                                        uint8_t(photons.green() > 255
                                                        ? 255
                                                        : photons.green()),
                                        uint8_t(photons.blue() > 255
                                                        ? 255
                                                        : photons.blue()));
                            });
                }));
        fostlib::cli::monitor(
                out, tracking, result,
                [frame](const fostlib::meter::reading &current) {
                    fostlib::stringstream out;
                    out << "] f" << frame << " " << current.done() << "/"
                        << current.work().value_or(0);
                    if (current.meta().size()
                        && not current.meta()[0].isnull()) {
                        fostlib::json meta(current.meta()[0]);
                        out << " ("
                            << fostlib::json::unparse(
                                       meta["panels"]["x"], false)
                            << "x"
                            << fostlib::json::unparse(
                                       meta["panels"]["y"], false)
                            << " of size "
                            << fostlib::json::unparse(meta["size"]["x"], false)
                            << "x"
                            << fostlib::json::unparse(meta["size"]["y"], false)
                            << ")";
                    }
                    return out.str();
                },
                [](const fostlib::meter::reading &) { return "["; });
        auto filename = output_filename;
        filename.replace_extension(fostlib::coerce<fostlib::fs::path>(
                fostlib::coerce<fostlib::string>(frame) + ".tga"));
        animray::targa(filename, result());
    }

    return 0;
}
