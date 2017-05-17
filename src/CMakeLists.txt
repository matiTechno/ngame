set(SOURCE
    main.cpp
    glad.c
    app.cpp
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_sdl_gl3.cpp
    scene.cpp
    sound.cpp
    shader.cpp
    texture.cpp
    buffers.cpp
    renderer2d.cpp
    pp_unit.cpp
    )

add_executable(app ${SOURCE})
target_link_libraries(app -lSDL2 -lSDL2_mixer -lstdc++fs)
