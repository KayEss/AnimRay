/*
    Copyright 2014, Kirit Saelensminde.
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


#include <fost/main>
#include <fost/progress-cli>
#include <fost/unicode>
#include <animray/animation/procedural/affine.hpp>
#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/camera/movie.hpp>
#include <animray/movable.hpp>
#include <animray/intersection.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/geometry/planar/triangle.hpp>
#include <animray/scene.hpp>
#include <animray/light/ambient.hpp>
#include <animray/light/collection.hpp>
#include <animray/light/point.hpp>
#include <animray/targa.hpp>
#include <animray/threading/sub-panel.hpp>


FSL_MAIN(
    "animray",
    "AnimRay. Copyright 2010-2014 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    const std::size_t threads(
        fostlib::coerce<fostlib::nullable<int>>(args.commandSwitch("t")).value(
            boost::thread::hardware_concurrency()));
    const std::size_t samples(fostlib::coerce<int>(
        args.commandSwitch("ss").value("2")));
    const std::size_t frames(fostlib::coerce<int>(
        args.commandSwitch("frames").value("2")));

    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("tetrahedron.tga"));
    const int width = fostlib::coerce< int >( args[2].value("96") );
    const int height = fostlib::coerce< int >( args[3].value("54") );

    typedef double world;
    const world aspect = double(width) / height;
    const world fw = width > height ? aspect * 0.024 : 0.024;
    const world fh = width > height ? 0.024 : 0.024 / aspect;

    typedef animray::triangle<animray::ray<world>> triangle;
    typedef animray::scene<
        animray::animation::affine<
            animray::matrix<world>,
            animray::rotate_z,
            animray::animation::affine<
                animray::matrix<world>,
                animray::rotate_y,
                animray::collection<triangle>
            >
        >,
        animray::light<
            std::tuple<
                animray::light<void, float>,
                animray::light<
                    std::vector<
                        animray::light<animray::point3d<world>, animray::rgb<float>>>,
                    animray::rgb<float>>
            >, animray::rgb<float>
        >,
        animray::rgb<float>>
            scene_type;
    scene_type scene;
    scene.background(animray::rgb<float>(20, 70, 100));

    animray::point3d<world> top(0, 0, 1), bottom(0, 0, -1),
        north(0, 1, 0), south(0, -1, 0), east(1, 0, 0), west(-1, 0, 0);

    scene.geometry().instance().instance()
        .insert(triangle(top, north, east))
        .insert(triangle(top, east, south))
        .insert(triangle(top, south, west))
        .insert(triangle(top, west, north))
        .insert(triangle(bottom, north, east))
        .insert(triangle(bottom, east, south))
        .insert(triangle(bottom, south, west))
        .insert(triangle(bottom, west, north));

    const std::size_t angle(20);
    scene.geometry()
        (40_deg, 1_deg * angle, frames);
    scene.geometry().instance()
        (0, 2_deg * angle, frames);

    std::get<0>(scene.light()).color(50);
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(-3.0, 5.0, -5.0),
            animray::rgb<float>(0x40, 0xa0, 0x40)));
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(-5.0, -3.0, -5.0),
            animray::rgb<float>(0xa0, 0x40, 0x40)));
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(3.0, -3.0, -5.0),
            animray::rgb<float>(0x40, 0x40, 0xa0)));

    for ( std::size_t frame{}; frame != frames * 360 / angle; ++frame ) {
        animray::movable<
                animray::stacatto_movie<
                    animray::pinhole_camera<
                        animray::ray<world>,
                        animray::flat_jitter_camera<world>
                    >
                >,
                typename animray::with_frame<animray::ray<world>>::type
            > camera(fw, fh, width, height, 0.05);
        camera
            (animray::rotate_x<world>(-15_deg))
            (animray::translate<world>(0.0, 0.0, -4))
            .instance().frame(frame);

        typedef animray::film<animray::rgb<uint8_t>> film_type;

        fostlib::worker worker;
        fostlib::meter tracking;
        fostlib::future<film_type> result(worker.run<film_type>(
            [threads, samples, width, height, &scene, &camera] () {
                return animray::threading::sub_panel<film_type>(
                    threads, width, height,
                    [samples, &scene, &camera](
                        const film_type::size_type x, const film_type::size_type y
                    ) {
                        animray::rgb<float> photons;
                        for ( std::size_t sample{}; sample != samples; ++sample ) {
                            photons += scene(camera, x, y) /= samples;
                        }
                        const float exposure = 1.4f;
                        photons /= exposure;
                        return animray::rgb<uint8_t>(
                            uint8_t(photons.red() > 255 ? 255 : photons.red()),
                            uint8_t(photons.green() > 255 ? 255 : photons.green()),
                            uint8_t(photons.blue() > 255 ? 255 : photons.blue()));
                    });
                }));
        fostlib::cli::monitor(out, tracking, result,
            [frame](const fostlib::meter::reading &current) {
                fostlib::stringstream out;
                out << "] f"  << frame << " "
                    << current.done() << "/" << current.work().value(0);
                if ( current.meta().size() && not current.meta()[0].isnull() ) {
                    fostlib::json meta(current.meta()[0]);
                    out << " (" << fostlib::json::unparse(meta["panels"]["x"], false)
                        << "x" << fostlib::json::unparse(meta["panels"]["y"], false)
                        << " of size " << fostlib::json::unparse(meta["size"]["x"], false)
                        << "x" << fostlib::json::unparse(meta["size"]["y"], false)
                        << ")";
                }
                return out.str();
            },
            [](const fostlib::meter::reading &) {
                return "[";
            });
        boost::filesystem::wpath filename(output_filename);
        filename.replace_extension(
            fostlib::coerce<boost::filesystem::wpath>(
                fostlib::coerce<fostlib::string>(frame) + ".tga"));
        animray::targa(filename, result());
    }

    return 0;
}

