set(SOURCE
    animal_sim.cpp
    animal_sim.hpp
    main.cpp
    render_ext.cpp
    render_ext.hpp
    )

add_executable(animal_sim ${SOURCE})
target_link_libraries(animal_sim ngame)
