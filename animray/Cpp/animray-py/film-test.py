# -*- coding: utf-8 -*-
"""
    Copyright 2010, Kirit Saelensminde.
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
"""

import _animray

def basic_test(cls):
    f1 = cls(120, 80, 123)

    assert f1.width == 120, f1.width
    assert f1.height == 80, f1.height
    assert f1(30, 40) == 123, f1(30, 40)
    f1(30, 40, 127); assert f1(30, 40) == 127, f1(30, 40)

    f2 = cls(100, 100)
    assert f2(10, 10) == 0, f2(10, 10)

[basic_test(k) for k in [
    _animray.film_gray8, _animray.film_gray16
]]
