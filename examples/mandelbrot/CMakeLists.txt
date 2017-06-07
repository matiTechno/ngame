set(SOURCE
    main.cpp
    mandelbrot.cpp)

add_executable(mandelbrot ${SOURCE})
target_link_libraries(mandelbrot ngame)
file(COPY compute.sh render.sh DESTINATION .)
