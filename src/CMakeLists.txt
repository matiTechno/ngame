set(SOURCE
    glad.c
    app.cpp
    scene.cpp
    shader.cpp
    texture.cpp
    buffers.cpp
    renderer2d.cpp
    pp_unit.cpp
    text.cpp
    font.cpp
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_sdl_gl3.cpp
    sound.cpp
    sprite.cpp
    font_loader.cpp
    blend.cpp
    vspace.cpp
    3d/renderer3d.cpp
    3d/model.cpp
    3d/camera.cpp
    )

add_library(ngame ${SOURCE})
target_link_libraries(ngame -lSDL2 -lSDL2_mixer)

if(NOT APPLE)
    target_link_libraries(ngame -lstdc++fs)
endif()
