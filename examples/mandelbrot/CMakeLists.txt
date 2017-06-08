set(SOURCE
    main.cpp
    mandelbrot.cpp
    mandelbrot.hpp
    compute.sh)

add_executable(mandelbrot ${SOURCE})
target_link_libraries(mandelbrot ngame)
file(COPY compute.sh DESTINATION .)
