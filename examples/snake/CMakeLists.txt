set(SOURCE
    main.cpp
    snake.cpp
    snake.hpp
    menu.cpp
    menu.hpp)

add_executable(snake ${SOURCE})
target_link_libraries(snake ngame)
file(COPY laser3.mp3 ProggyCleanCE.ttf DESTINATION .)
