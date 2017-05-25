set(SOURCE
    main.cpp
    sc_master.cpp
    sc_level.cpp
    proto/objects.cpp
    proto/collisions.cpp
    # temp, only to use emitter class
    ../test_scene/test_scene.cpp
    ../test_scene/menu.cpp
    )

add_executable(arkanoid_v3 ${SOURCE})
target_link_libraries(arkanoid_v3 ngame)
file(COPY res DESTINATION .)
