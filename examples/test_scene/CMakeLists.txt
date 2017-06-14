set(SOURCE
    main.cpp
    test_scene.cpp
    test_scene.hpp
    menu.cpp
    menu.hpp
    res/pp_unit.sh
    res/wave.sh)

add_executable(test_scene ${SOURCE})
target_link_libraries(test_scene ngame)
file(COPY res DESTINATION .)
