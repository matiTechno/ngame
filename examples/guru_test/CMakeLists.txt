set(SOURCE
    main.cpp
    testscene.cpp
    testscene.hpp
    )
add_executable(guru_test ${SOURCE})
target_link_libraries(guru_test ngame)
