/**
    Copyright 2020-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#pragma once


#include <animray/cli/main.hpp>
#include <animray/formats/targa.hpp>
#include <animray/threading/sub-panel.hpp>
#include <iostream>


namespace animray {


    template<typename film_type, typename P>
    inline film_type cli_render_frame(
            cli::arguments const &args,
            std::optional<std::size_t> const frame,
            std::size_t const threads,
            P const pixels) {
        threading::sub_panel_progress progress{args.width, args.height};
        std::promise<film_type> promise;
        auto result = promise.get_future();
        std::thread{[threads, &args, &pixels, &progress,
                     promise = std::move(promise)]() mutable {
            promise.set_value(animray::threading::sub_panel<film_type>(
                    progress, threads, args.width, args.height, pixels));
        }}.detach();
        auto filename = args.output_filename;
        if (frame) {
            filename.replace_extension(std::to_string(*frame) + ".tga");
        }
        auto const print = [&]() {
            std::cout << filename << ' ' << args.width << 'x' << args.height
                      << ' ' << progress.count.load() << '/'
                      << progress.count_limit << " (" << progress.panel_size_x
                      << 'x' << progress.panel_size_y << ")\r" << std::flush;
        };
        do {
            print();
        } while (result.wait_for(std::chrono::milliseconds{100})
                 != std::future_status::ready);
        print();
        std::cout << '\n';
        auto rendered = result.get();
        animray::targa(filename, rendered);
        return rendered;
    }


    template<typename film_type, typename P>
    inline film_type cli_render(
            cli::arguments const &args, std::size_t const threads, P pixels) {
        return cli_render_frame<film_type>(
                args, {}, threads, std::move(pixels));
    }


}
