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


#include <compare>
#include <ostream>


namespace animray {


    /// Strongly typed number not convertible to other number types
    template<typename Tag, typename Rep>
    class number {
        Rep value;

      public:
        constexpr number() : value{} {}
        constexpr number(Rep v) : value{v} {}
        template<typename Other>
        explicit constexpr number(number<Other, Rep> n)
        : value{static_cast<Rep>(n)} {}

        using tag_type = Tag;
        using representation_type = Rep;

        /// Access to underlying numeric value
        constexpr explicit operator Rep() const noexcept { return value; }

        /// Comparisons
        friend constexpr auto
                operator<=>(number const &, number const &) noexcept = default;

        /// Numeric operators
        friend constexpr number operator+(number l, number r) {
            return {l.value + r.value};
        }
        friend constexpr number operator-(number l, number r) {
            return {l.value - r.value};
        }
        friend constexpr number operator*(number l, number r) {
            return {l.value * r.value};
        }
        friend constexpr number operator/(number l, number r) {
            return {l.value / r.value};
        }

        /// Bit manipulation operators
        constexpr number operator~() const noexcept { return {~value}; }
        friend constexpr number operator&(number l, number r) {
            return {l.value & r.value};
        }
        friend constexpr number operator|(number l, number r) {
            return {l.value | r.value};
        }
    };


    template<typename Tag, typename Rep>
    auto &operator<<(std::ostream &os, number<Tag, Rep> n) {
        return os << Rep{n};
    }


}
