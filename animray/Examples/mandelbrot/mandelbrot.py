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

import pygame
from pygame.locals import *

if not pygame.font: print 'Warning, fonts disabled'
if not pygame.mixer: print 'Warning, sound disabled'

from animray import film_gray8, mandelbrot_gray8


class mandelbrot(object):
    """
        Stores the required state to work with Mandelbrots
    """
    def __init__(self, screen, x, y, radius):
        """
            Initialise the Mandelbrot
        """
        size = screen.get_size()
        self.mandelbrot = mandelbrot_gray8(size[0], size[1], x, y, radius, 8)
        self.x, self.y, self.radius = x, y, radius
        self(screen)

    def __call__(self, screen):
        """
            Redraw the mandelbrot on the screen
        """
        film = film_gray8(self.mandelbrot.width, self.mandelbrot.height, 0)
        self.mandelbrot(film)
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

    mbrot = mandelbrot(screen, 0, 0, 2)

    to_exit = False
    while not to_exit:
        event = pygame.event.wait()
        if event.type == QUIT:
            to_exit = True
        elif event.type == VIDEORESIZE:
            screen = pygame.display.get_surface()
            screen = pygame.display.set_mode((event.w, event.h), RESIZABLE)
            mbrot = mandelbrot(screen, mbrot.x, mbrot.y, mbrot.radius)
        elif event.type == KEYDOWN:
            if event.unicode == u'+':
                mbrot = mandelbrot(screen, mbrot.x, mbrot.y, mbrot.radius * 0.9)
            elif event.unicode == u'-':
                mbrot = mandelbrot(screen, mbrot.x, mbrot.y, mbrot.radius / 0.9)
            elif event.key == K_LEFT:
                mbrot = mandelbrot(screen, mbrot.x - mbrot.radius / 10, mbrot.y, mbrot.radius)
            elif event.key == K_RIGHT:
                mbrot = mandelbrot(screen, mbrot.x + mbrot.radius / 10, mbrot.y, mbrot.radius)
            elif event.key == K_UP:
                mbrot = mandelbrot(screen, mbrot.x, mbrot.y - mbrot.radius / 10, mbrot.radius)
            elif event.key == K_DOWN:
                mbrot = mandelbrot(screen, mbrot.x, mbrot.y + mbrot.radius / 10, mbrot.radius)
            else:
                print event
        elif event.type == KEYUP:
            pass
        else:
            print event
