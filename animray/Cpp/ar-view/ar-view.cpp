/*
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
*/


#include <fost/main>
#include <animray/metachatr.hpp>


#include <GL/glew.h>
#ifdef __APPLE__
#   include <GLUT/glug.h>
#else
#   include <GL/glut.h>
#endif


#define GLUT_MAIN_PRIVATE_CALLER( exe_name, banner_text ) \
    int main( int argc, char *argv[ ] ) { \
        glutInit(&argc, argv); \
        fostlib::arguments args( argc, argv ); \
        return fostlib::main_exec( config_settings, std::cout, args, main_body ); \
    } \
    int main_body

#define GLUT_MAIN( exe_name, banner_text ) \
        FSL_MAIN_PRIVATE_COMMON( exe_name, banner_text ) \
        FSL_MAIN_PRIVATE_SIMPLEMAIN() \
        GLUT_MAIN_PRIVATE_CALLER( exe_name, banner_text )


namespace {
    void render(void) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glutSwapBuffers();
    }

    void animate(void) {
    }
}


GLUT_MAIN(
    L"ar-view",
    L"AnimRay Viewer, Copyright 2012 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("AnimRay Viewer");
    glutDisplayFunc(&render);
    glutIdleFunc(&animate);

    glewInit();
    if ( !GLEW_VERSION_2_0 )
        throw std::runtime_error("OpenGL 2.0 not available");

    glutMainLoop();

    return 0;
}

