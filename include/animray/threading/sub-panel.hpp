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


#ifndef ANIMRAY_THREADING_SUB_PANEL_HPP
#define ANIMRAY_THREADING_SUB_PANEL_HPP
#pragma once


#include <animray/film.hpp>
#include <animray/panel.hpp>
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


    /// Progress tracking for sub-panel rendering
    class sub_panel_progress {
        std::size_t pdiv;

      public:
        sub_panel_progress(std::size_t const w, std::size_t const h)
        : pdiv{detail::bigestodd(detail::gcd(w, h))},
          panel_size_x{w / pdiv},
          panel_size_y{h / pdiv},
          panel_count_x{w / panel_size_x},
          panel_count_y{h / panel_size_y},
          count_limit{panel_count_x * panel_count_y} {}

        std::size_t panel_size_x, panel_size_y, panel_count_x, panel_count_y,
                count_limit;
        std::atomic<std::uint64_t> count{};
    };


    /// A mechanism whereby the frame is rendered in a number of sub-panels
    template<typename film_type, typename Fn>
    film_type sub_panel(
            sub_panel_progress &progress,
            std::size_t const threads,
            typename film_type::size_type const width,
            typename film_type::size_type const height,
            Fn fn) {
        using panel_type = animray::panel<film_type>;
        using calculation_type = animray::film<std::future<panel_type>>;

        std::vector<std::pair<std::size_t, std::size_t>> futures;
        calculation_type work{
                progress.panel_count_x, progress.panel_count_y,
                [&fn, &progress, &futures](auto const pr, auto const pc) {
                    futures.emplace_back(pr, pc);
                    return std::async(
                            std::launch::deferred, [pr, pc, &fn, &progress]() {
                                auto r = panel_type{
                                        progress.panel_size_x,
                                        progress.panel_size_y,
                                        progress.panel_size_x * pr,
                                        progress.panel_size_y * pc, fn};
                                ++progress.count;
                                return r;
                            });
                }};
        std::atomic<std::size_t> next{};
        std::vector<std::thread> joins;
        joins.reserve(threads);
        for (std::size_t thread{}; thread != threads; ++thread) {
            joins.emplace_back([&futures, &next, &work]() {
                for (std::size_t index = next++; index < futures.size();
                     index = next++) {
                    auto const [r, c] = futures[index];
                    work[r][c].wait();
                }
            });
        }
        for (auto &th : joins) { th.join(); }
        auto panels = animray::film<panel_type>{
                progress.panel_count_x, progress.panel_count_y,
                [&work](auto const r, auto const c) {
                    return work[r][c].get();
                }};
        return film_type{
                width, height,
                [&panels, &progress](auto const x, auto const y) {
                    return panels[x / progress.panel_size_x]
                                 [y / progress.panel_size_y]
                                 [x % progress.panel_size_x]
                                 [y % progress.panel_size_y];
                }};
    }


}


#endif // ANIMRAY_THREADING_SUB_PANEL_HPP
