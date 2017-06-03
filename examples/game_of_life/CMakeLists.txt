set(SOURCE
    main.cpp
    gol.cpp
    gol.hpp)

add_executable(gol ${SOURCE})
target_link_libraries(gol ngame)
file(COPY res DESTINATION .)

