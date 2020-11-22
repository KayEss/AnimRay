/**
    Copyright 2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/targa.hpp>
#include <animray/threading/sub-panel.hpp>


namespace animray {


    template<typename film_type, typename P>
    inline film_type cli_render_frame(
            std::filesystem::path filename,
            std::optional<std::size_t> const frame,
            std::size_t const threads,
            typename film_type::size_type const width,
            typename film_type::size_type const height,
            P const pixels) {
        std::promise<film_type> promise;
        auto result = promise.get_future();
        std::thread{[threads, width, height, &pixels,
                     promise = std::move(promise)]() mutable {
            promise.set_value(animray::threading::sub_panel<film_type>(
                    threads, width, height, pixels));
        }}.detach();
        fostlib::cli::monitor(
                std::cout, result,
                [frame](fostlib::progress::reading const &current) {
                    fostlib::stringstream out;
                    out << ']';
                    if (frame) out << " f" << *frame;
                    if (current.done() && current.work()) {
                        out << " " << current.done() << "/" << *current.work();
                    }
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
                });
        auto rendered = result.get();
        if (frame) {
            filename.replace_extension(fostlib::coerce<std::filesystem::path>(
                    fostlib::coerce<fostlib::string>(*frame) + ".tga"));
        }
        animray::targa(filename, rendered);
        return rendered;
    }


    template<typename film_type, typename P>
    inline film_type cli_render(
            std::filesystem::path filename,
            std::size_t const threads,
            typename film_type::size_type const width,
            typename film_type::size_type const height,
            P pixels) {
        return cli_render_frame<film_type>(
                filename, {}, threads, width, height, std::move(pixels));
    }


}
