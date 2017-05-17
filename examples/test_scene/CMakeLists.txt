set(SOURCE
    main.cpp
    )
add_executable(test_scene ${SOURCE})
target_link_libraries(test_scene ngame)
file(COPY res DESTINATION .)
