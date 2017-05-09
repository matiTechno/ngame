set(SOURCE
    main.cpp
    glad.c
    app.cpp
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_sdl_gl3.cpp
    scene.cpp
    )

add_executable(app ${SOURCE})
target_link_libraries(app -lSDL2)
