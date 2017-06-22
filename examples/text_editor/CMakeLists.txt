set(SOURCE
    main.cpp
    editor.cpp
    editor.hpp
    )
add_executable(editor ${SOURCE})
target_link_libraries(editor ngame)
file(COPY ProggyCleanCE.ttf DESTINATION .)
