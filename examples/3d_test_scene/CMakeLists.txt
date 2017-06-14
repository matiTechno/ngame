set(SOURCE
    main.cpp
    test_scene.cpp
    test_scene.hpp)

add_executable(3d_test ${SOURCE})
target_link_libraries(3d_test ngame)
file(COPY monkey.obj DESTINATION .)
