set(SOURCE
    main.cpp
    flock.cpp
    flock.hpp
    )
add_executable(flock ${SOURCE})
target_link_libraries(flock ngame)
