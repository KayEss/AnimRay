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


#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/color/rgb.hpp>
#include <animray/movable.hpp>
#include <animray/geometry/planar/triangle.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/light/ambient.hpp>
#include <animray/light/collection.hpp>
#include <animray/light/point.hpp>
#include <animray/ray.hpp>
#include <animray/scene.hpp>
#include <fost/log>
#include <fost/test>


FSL_TEST_SUITE(triangle);


namespace {
    template<typename G>
    void cases(const G &g) {
        FSL_CHECK(g.occludes(
                animray::ray<double>(
                        animray::point3d<double>(1, 1, 1),
                        animray::unit_vector<double>(0, 0, -1)),
                0));
        FSL_CHECK_EQ(
                g.intersects(
                         animray::ray<double>(
                                 animray::point3d<double>(1, 1, 1),
                                 animray::unit_vector<double>(0, 0, -1)),
                         0)
                        .value()
                        .direction,
                animray::unit_vector<double>(0, 0, 1));
        FSL_CHECK(g.occludes(
                animray::ray<double>(
                        animray::point3d<double>(0, 0, 1),
                        animray::point3d<double>(1, 1, 0)),
                0));
        FSL_CHECK(g.occludes(
                animray::ray<double>(
                        animray::point3d<double>(0, 0, -1),
                        animray::point3d<double>(1, 1, 0)),
                0));
        FSL_CHECK_EQ(
                g.intersects(
                         animray::ray<double>(
                                 animray::point3d<double>(0, 0, -1),
                                 animray::point3d<double>(1, 1, 0)),
                         0)
                        .value()
                        .direction,
                animray::unit_vector<double>(0, 0, -1));
        FSL_CHECK(not g.occludes(
                animray::ray<double>(
                        animray::point3d<double>(-1, 1, -1),
                        animray::unit_vector<double>(0, 0, -1)),
                0));
        FSL_CHECK(not g.occludes(
                animray::ray<double>(
                        animray::point3d<double>(5, 5, 1),
                        animray::unit_vector<double>(0, 0, -1)),
                0));
    }
}


FSL_TEST_FUNCTION(single) {
    cases(animray::triangle<animray::ray<double>>(
            animray::point3d<double>(0, 0, 0),
            animray::point3d<double>(5.f, 0, 0),
            animray::point3d<double>(0, 3.f, 0)));
}


FSL_TEST_FUNCTION(in_collection) {
    animray::collection<animray::triangle<animray::ray<double>>> right;
    right.insert(animray::triangle<animray::ray<double>>(
            animray::point3d<double>(0, 0, 0),
            animray::point3d<double>(5.f, 0, 0),
            animray::point3d<double>(0, 3.f, 0)));
    cases(right);
}


FSL_TEST_FUNCTION(full_scene) {
    typedef double world;
    typedef animray::triangle<animray::ray<world>> triangle;
    typedef animray::scene<
            animray::collection<triangle>,
            animray::light<
                    std::tuple<
                            animray::light<void, float>,
                            animray::light<
                                    std::vector<animray::light<
                                            animray::point3d<world>,
                                            animray::rgb<float>>>,
                                    animray::rgb<float>>>,
                    animray::rgb<float>>,
            animray::rgb<float>>
            scene_type;
    scene_type scene;
    scene.background = animray::rgb<float>(20, 70, 100);

    animray::point3d<world> top(0, 0, 1), bottom(0, 0, -1), north(1, 0, 0),
            south(-1, 0, 0), east(1, 0, 0), west(-1, 0, 0);

    scene.geometry.insert(triangle(
            animray::point3d<double>(0, 5, 0),
            animray::point3d<double>(-5, -5, 0),
            animray::point3d<double>(5, -5, 0)));

    std::get<0>(scene.light).color = 50;
    std::get<1>(scene.light)
            .push_back(
                    animray::light<animray::point3d<world>, animray::rgb<float>>(
                            animray::point3d<world>(-5.0, 5.0, -5.0),
                            animray::rgb<float>(0x40, 0xa0, 0x40)));
    std::get<1>(scene.light)
            .push_back(
                    animray::light<animray::point3d<world>, animray::rgb<float>>(
                            animray::point3d<world>(-5.0, -5.0, -5.0),
                            animray::rgb<float>(0xa0, 0x40, 0x40)));
    std::get<1>(scene.light)
            .push_back(
                    animray::light<animray::point3d<world>, animray::rgb<float>>(
                            animray::point3d<world>(5.0, -5.0, -5.0),
                            animray::rgb<float>(0x40, 0x40, 0xa0)));

    animray::movable<
            animray::pinhole_camera<
                    animray::ray<world>, animray::flat_jitter_camera<world>>,
            animray::ray<world>>
            camera(0.035, 0.024, 300, 200, 0.05);
    camera(animray::translate<world>(0.0, 0.0, -8.0));

    animray::rgb<float> c(scene(camera, 170, 95));
    FSL_CHECK_ERROR(c.red(), 213.445, 1e-2);
    FSL_CHECK_ERROR(c.green(), 214.031, 1e-2);
    FSL_CHECK_ERROR(c.blue(), 216.171, 1e-2);
}
