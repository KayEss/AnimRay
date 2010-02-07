#!/usr/local/bin/python
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

import os, sys
import pygame
from pygame.locals import *

if not pygame.font: print 'Warning, fonts disabled'
if not pygame.mixer: print 'Warning, sound disabled'


def generate(screen, x, y, radius):
    """
    """
    from animray import film_gray8, mandelbrot

    size = screen.get_size()
    film = film_gray8(size[0], size[1], 0)
    mandelbrot(film, x, y, radius, 8)

    return film


def draw(screen, cx, cy, r):
    """
    """
    film = generate(screen, cx, cy, r)
    background = pygame.Surface(screen.get_size())
    for x in range(0, film.width):
        for y in range(0, film.height):
            g = film(x, y)
            background.set_at((x, y), pygame.Color(g, g, g))
    screen.blit(background, (0, 0))
    pygame.display.flip()


def main(*args, **kwargs):
    """
    """
    pygame.init()
    screen = pygame.display.set_mode((300, 300), RESIZABLE)
    pygame.display.set_caption('AnimRay Mandelbrot')

    center_x, center_y = 0, 0
    radius = 2
    draw(screen, center_x, center_y, radius)

    to_exit = False
    while not to_exit:
        event = pygame.event.wait()
        if event.type == QUIT or \
                (event.type == KEYDOWN and event.key==K_ESCAPE):
            to_exit = True
        elif event.type == VIDEORESIZE:
            screen = pygame.display.get_surface()
            screen = pygame.display.set_mode((event.w, event.h), RESIZABLE)
            draw(screen, center_x, center_y, radius)
        else:
            print event
