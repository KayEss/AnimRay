/**
    Copyright 2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <limits>
#include <felspar/exceptions/underflow_error.hpp>
#include <felspar/exceptions/overflow_error.hpp>


namespace animray {


    template<typename I>
    concept Integral = requires {
        std::is_integral_v<I>;
    };


    template<Integral T, Integral F>
    T narrow(
            F f,
            felspar::source_location loc = felspar::source_location::current()) {
        using limits_T = std::numeric_limits<T>;
        using limits_F = std::numeric_limits<F>;
        if constexpr (std::is_same_v<T, bool>) {
            return f ? true : false;
        } else {
            if constexpr (limits_T::is_signed == limits_F::is_signed) {
                if constexpr (sizeof(T) < sizeof(F)) {
                    if (f < limits_T::min()) /*[[unlikely]]*/ {
                        throw felspar::stdexcept::underflow_error{
                                "Value is too large for target type", f,
                                std::move(loc)};
                    }
                    if (f > limits_T::max()) /*[[unlikely]]*/ {
                        throw felspar::stdexcept::overflow_error{
                                "Value is too small for the target type", f,
                                std::move(loc)};
                    }
                }
            } else if constexpr (limits_T::is_signed) {
                if constexpr (sizeof(T) <= sizeof(F)) {
                    if (f > limits_T::max()) /*[[unlikely]]*/ {
                        throw felspar::stdexcept::overflow_error{
                                "Value is too large for target type", f,
                                std::move(loc)};
                    }
                }
            } else if constexpr (limits_F::is_signed) {
                if (f < 0) /*[[unlikely]]*/ {
                    throw felspar::stdexcept::underflow_error{
                            "Value is negative going into an unsigned type", f,
                            std::move(loc)};
                }
                if constexpr (sizeof(T) < sizeof(F)) {
                    if (f > F{limits_T::max()}) /*[[unlikely]]*/ {
                        throw felspar::stdexcept::overflow_error{
                                "Value is too large for target type", f,
                                std::move(loc)};
                    }
                }
            }
            return T(f);
        }
    }


}
