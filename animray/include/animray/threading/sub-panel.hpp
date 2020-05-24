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


#ifndef ANIMRAY_THREADING_SUB_PANEL_HPP
#define ANIMRAY_THREADING_SUB_PANEL_HPP
#pragma once


#include <animray/film.hpp>
#include <animray/panel.hpp>

#include <fost/insert>
#include <fost/progress>


namespace animray {


    namespace threading {


        namespace detail {
            /// Calculate the greatest common denominator
            template<typename S>
            S gcd(S a, S b) {
                while (b != 0) {
                    S t = b;
                    b = a % b;
                    a = t;
                }
                return a;
            }
            /// Calculate the next power of 2
            template<typename S>
            S bigestodd(S n) {
                while ((n & 1) == 0 && n > S(20)) { n /= 2; }
                return n;
            }
        }


        /// A mechanism whereby the frame is rendered in a number of sub-panels
        template<typename film_type, typename Fn>
        film_type sub_panel(
                const std::size_t threads,
                const typename film_type::size_type width,
                const typename film_type::size_type height,
                Fn fn) {
            const std::size_t pdiv(
                    detail::bigestodd(detail::gcd(width, height)));
            const std::size_t px(width / pdiv), py(height / pdiv);
            const std::size_t panels_x(width / px), panels_y(height / py);
            fostlib::json description;
            fostlib::insert(description, "panels", "x", panels_x);
            fostlib::insert(description, "panels", "y", panels_y);
            fostlib::insert(description, "size", "x", px);
            fostlib::insert(description, "size", "y", py);
            fostlib::progress progress(description, width / px * height / py);

            typedef animray::panel<film_type> panel_type;
            typedef animray::film<fostlib::future<panel_type>> calculation_type;

            std::vector<fostlib::worker> thread_pool(threads);
            std::size_t worker{};

            calculation_type panels(
                    panels_x, panels_y,
                    [&thread_pool, &worker, &fn, px, py, threads, &progress](
                            const typename panel_type::size_type pr,
                            const typename panel_type::size_type pc) {
                        fostlib::future<panel_type> result =
                                thread_pool[worker].template run<panel_type>(
                                        [px, py, pr, pc, &fn, &progress]() {
                                            panel_type panel(
                                                    px, py, px * pr, py * pc,
                                                    fn);
                                            ++progress;
                                            return panel;
                                        });
                        worker = (worker + 1) % threads;
                        return result;
                    });

            return film_type(
                    width, height,
                    [&panels, px,
                     py](const typename film_type::size_type x,
                         const typename film_type::size_type y) {
                        return panels[x / px][y / py]()[x % px][y % py];
                    });
        }


    }


}


#endif // ANIMRAY_THREADING_SUB_PANEL_HPP
