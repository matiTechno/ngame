set(SOURCE
    main.cpp
    snake.cpp)

add_executable(snake ${SOURCE})
target_link_libraries(snake ngame)
