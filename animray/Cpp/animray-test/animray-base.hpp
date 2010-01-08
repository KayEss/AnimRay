/*
    Copyright 1991-2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifdef FOST_OS_WINDOWS
    #define ANIMRAY_BASE_DECLSPEC __declspec(dllexport)
    #define ANIMRAY_BASE_EXTERN extern
#else
    #define ANIMRAY_BASE_DECLSPEC
    #define ANIMRAY_BASE_EXTERN
#endif
