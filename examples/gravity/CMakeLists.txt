set(SOURCE
    gravity.cpp
    gravity.hpp
    main.cpp)

add_executable(gravity ${SOURCE})
target_link_libraries(gravity ngame)
file(COPY points.sh gravity.sh DESTINATION .)
