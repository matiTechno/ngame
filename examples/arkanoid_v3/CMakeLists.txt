set(SOURCE
    main.cpp
    sc_master.cpp
    sc_level.cpp
    )

add_executable(arkanoid_v3 ${SOURCE})
target_link_libraries(arkanoid_v3 ngame)
file(COPY res DESTINATION .)
