set(SOURCE
    main.cpp
    editor.cpp
    editor.hpp
    )
add_executable(cxxedit ${SOURCE})
target_link_libraries(cxxedit ngame)
file(COPY ProggyCleanCE.ttf DESTINATION .)
