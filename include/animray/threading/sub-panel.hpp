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


#ifndef ANIMRAY_THREADING_SUB_PANEL_HPP
#define ANIMRAY_THREADING_SUB_PANEL_HPP
#pragma once


#include <animray/film.hpp>
#include <animray/panel.hpp>

#include <fost/insert>
#include <fost/progress>

#include <future>


namespace animray::threading {


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
            std::size_t const threads,
            typename film_type::size_type const width,
            typename film_type::size_type const height,
            Fn fn) {
        std::size_t const pdiv(detail::bigestodd(detail::gcd(width, height)));
        std::size_t const px(width / pdiv), py(height / pdiv);
        std::size_t const panels_x(width / px), panels_y(height / py);

        fostlib::json description;
        fostlib::insert(description, "panels", "x", panels_x);
        fostlib::insert(description, "panels", "y", panels_y);
        fostlib::insert(description, "size", "x", px);
        fostlib::insert(description, "size", "y", py);
        fostlib::progress progress(
                std::move(description), width / px * height / py);

        using panel_type = animray::panel<film_type>;
        using calculation_type = animray::film<std::future<panel_type>>;

        std::vector<std::pair<std::size_t, std::size_t>> futures;
        calculation_type work{
                panels_x, panels_y,
                [&fn, px, py, &progress,
                 &futures](auto const pr, auto const pc) {
                    futures.emplace_back(pr, pc);
                    return std::async(
                            std::launch::deferred,
                            [px, py, pr, pc, &fn, &progress]() {
                                ++progress;
                                return panel_type{px, py, px * pr, py * pc, fn};
                            });
                }};
        std::atomic<std::size_t> next{};
        for (std::size_t thread{}; thread != threads; ++thread) {
            std::thread{[&futures, &next, &work]() {
                for (std::size_t index = next++; index < futures.size();
                     index = next++) {
                    auto const [r, c] = futures[index];
                    work[r][c].wait();
                }
            }}.detach();
        }

        auto panels = animray::film<panel_type>{
                panels_x, panels_y, [&work](auto const r, auto const c) {
                    return work[r][c].get();
                }};
        return film_type{
                width, height, [&panels, px, py](auto const x, auto const y) {
                    return panels[x / px][y / py][x % px][y % py];
                }};
    }


}


#endif // ANIMRAY_THREADING_SUB_PANEL_HPP
