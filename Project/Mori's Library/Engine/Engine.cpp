
#pragma once
#include "Engine.h"
#include <iostream>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>
#include <GL/glew.h>
#include <GLVertexArray.h>
#include <color3.h>


Engine::Engine() :
#ifdef _DEBUG				
    logger(Logger::Severity::Debug, true, last_tick)
#else 						
    logger(Logger::Severity::Event, false, last_tick)
#endif
{
    timing.start_time = SDL_GetTicks64();
    timing.prev_time = timing.start_time;
    Createmodels();
    Createshaders();
}

Engine::~Engine() {}

void Engine::AddFont(const std::filesystem::path& file_name)
{
    fonts.push_back(file_name);
}

void Engine::Start(std::string window_title) {
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    srand(seed);
    
    //logger.LogEvent("Engine Started");//
    //window.Start(window_title);
    //last_test = last_tick;
}

void Engine::Stop() {
    //logger.LogEvent("Engine Stopped");
}

void Engine::Update() {
    update_timing();
    
    //if (timing.dt > 1.0 / TargetFPS) {
        gamestatemanager.Update(timing.dt);
        input.Update();
        //OpenGLApplication.Update();
    //}
        glCheck(glEnable(GL_BLEND));
        glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Engine::Draw()
{

}

void Engine::ImGuiDraw()
{
}

void Engine::HandleEvent([[maybe_unused]]SDL_Window& sdl_window, [[maybe_unused]] const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
    
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        {
            on_window_resized(sdl_window);
        }
        break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        {
            on_window_resized(sdl_window);
        }
        break;
        }
        break;
    case SDL_KEYDOWN:
        input.on_key_pressed(event);
        break;
    case SDL_KEYUP:
        input.on_key_released(event);
        break;
    
    }
}

void Engine::on_window_resized([[maybe_unused]] SDL_Window& sdl_window) noexcept
{
    int width = 0, height = 0;
    SDL_GL_GetDrawableSize(&sdl_window, &width, &height);
    Window_Size = vec2(static_cast<float>(width), static_cast<float>(height));
    glCheck(glViewport(0, 0, width, height));
}

void Engine::Set_Gamemode(GameMode value)
{
    gamemode = value;
}

void Engine::update_timing() noexcept
{
    const auto current_time = SDL_GetTicks64();
    const auto delta_ms = current_time - timing.prev_time;
    timing.dt = delta_ms / 1'000.0f;
    timing.ElapsedTime += delta_ms / 1'000.0f;

    timing.prev_time = current_time;
    const auto elapsed_ms = current_time - timing.start_time;

    timing.count++;
    if (elapsed_ms > 1'000)
    {
        timing.fps = timing.count / (elapsed_ms / 1'000.0f);
        timing.start_time = current_time;
        timing.count = 0;
    }
}

void Engine::Createmodels()
{
    CreateRectmodel();
    CreateCirclemodel("assets/meshes/circle.msh");
}

void Engine::CreateRectmodel()
{
    GLVertexArray model(GLPrimitive::Triangles);

    std::vector<vec2> positions = { vec2{-0.5f, -0.5f}, vec2{0.5f, -0.5f}, vec2{0.5f, 0.5f}, vec2{-0.5f, 0.5f} };
    tex_positions = { vec2{0.0f, 0.0f}, vec2{1.0f, 0.0f}, vec2{1.0f, 1.0f}, vec2{0.0f, 1.0f} };
    std::vector<color3> colors = { color3{0, 0, 0}, color3{0, 0, 0}, color3{0, 0, 0}, color3{0, 0, 0} };
    std::vector<unsigned> indices{ 0, 1, 2, 2, 3, 0 };

    GLVertexBuffer    position_buffer(std::span{ positions });
    GLVertexBuffer    tex_position_buffer(std::span{ tex_positions });
    GLVertexBuffer    colors_buffer(std::span{ colors });

    glCheck(model.SetVertexCount(static_cast<int>(positions.size())));

    GLAttributeLayout position_coor;
    position_coor.component_type = GLAttributeLayout::Float;
    position_coor.component_dimension = GLAttributeLayout::_2;
    position_coor.normalized = false;
    position_coor.vertex_layout_location = 0; // 1st field is 0 index based
    position_coor.stride = sizeof(vec2);
    position_coor.offset = 0;
    position_coor.relative_offset = 0;

    GLAttributeLayout tex_position;
    tex_position.component_type = GLAttributeLayout::Float;
    tex_position.component_dimension = GLAttributeLayout::_2;
    tex_position.normalized = false;
    tex_position.vertex_layout_location = 1; // 1st field is 0 index based
    tex_position.stride = sizeof(vec2);
    tex_position.offset = 0;
    tex_position.relative_offset = 0;

    GLAttributeLayout glcolor;
    glcolor.component_type = GLAttributeLayout::Float;
    glcolor.component_dimension = GLAttributeLayout::_3;
    glcolor.normalized = false;
    glcolor.vertex_layout_location = 2; // 2nd field of Vertex
    glcolor.stride = sizeof(color3);
    glcolor.offset = 0; // starts after the position bytes
    glcolor.relative_offset = 0;

    model.AddVertexBuffer(std::move(position_buffer), { position_coor });
    model.AddVertexBuffer(std::move(tex_position_buffer), { tex_position });
    model.AddVertexBuffer(std::move(colors_buffer), { glcolor });

    GLIndexBuffer       index_buffer(indices);
    glCheck(model.SetIndexBuffer(std::move(index_buffer)));

    models.emplace_back(std::move(model));
}

void Engine::CreateCirclemodel(const std::filesystem::path& mesh_file_path)
{
    GLVertexArray model;

    std::ifstream meshFile{ mesh_file_path };
    if (!meshFile)
    {
        throw std::runtime_error{ std::string("ERROR: Unable to open mesh file: ") + mesh_file_path.string() };
    }

    std::vector<vec2>           vertex_positions;
    std::vector<unsigned short> vertex_indices;

    std::string       modelName;
    float             x = 0, y = 0;
    GLushort          index = 0;
    std::string       line;
    char              mode = 0;
    GLPrimitive::Type primitive_type = GLPrimitive::Points;
    bool three = true;

    while (meshFile)
    {
        std::getline(meshFile, line);
        if (line.empty())
        {
            continue;
        }

        std::istringstream sstr{ line };
        sstr >> mode;
        switch (mode) {
        case 'n':
            sstr >> modelName;
            break;
        case 'v':
            sstr >> x >> y;
            vertex_positions.emplace_back(x, y);
            break;
        case 't':
            primitive_type = GLPrimitive::Triangles;
            sstr >> index;
            vertex_indices.push_back(index);
            sstr >> index;
            vertex_indices.push_back(index);
            sstr >> index;
            vertex_indices.push_back(index);
            break;
        case 'f':
            primitive_type = GLPrimitive::TriangleFan;
            if (three) {
                sstr >> index;
                vertex_indices.push_back(index);
                sstr >> index;
                vertex_indices.push_back(index);
                sstr >> index;
                vertex_indices.push_back(index);
                three = false;
            }
            else {
                sstr >> index;
                vertex_indices.push_back(index);
            }
            break;
        }
    }

    GLVertexBuffer    position_buffer(std::span{ vertex_positions });

    glCheck(model.SetVertexCount(static_cast<int>(vertex_positions.size())));
    model.SetPrimitivePattern(primitive_type);

    GLAttributeLayout position_coor;
    position_coor.component_type = GLAttributeLayout::Float;
    position_coor.component_dimension = GLAttributeLayout::_2;
    position_coor.normalized = false;
    position_coor.vertex_layout_location = 0; // 1st field is 0 index based
    position_coor.stride = sizeof(vec2);
    position_coor.offset = 0;
    position_coor.relative_offset = 0;

    model.AddVertexBuffer(std::move(position_buffer), { position_coor });

    GLIndexBuffer       index_buffer(vertex_indices);
    glCheck(model.SetIndexBuffer(std::move(index_buffer)));

    models.emplace_back(std::move(model));
}



void Engine::Createshaders()
{
    shaders.emplace_back(GLShader("basic", { {GLShader::VERTEX, "assets/shaders/basic.vert"}, {GLShader::FRAGMENT, "assets/shaders/basic.frag"} }));
    shaders.emplace_back(GLShader("collision", { {GLShader::VERTEX, "assets/shaders/collision.vert"}, {GLShader::FRAGMENT, "assets/shaders/collision.frag"} }));
    shaders.emplace_back(GLShader("logo", { {GLShader::VERTEX, "assets/shaders/Logo.vert"}, {GLShader::FRAGMENT, "assets/shaders/Logo.frag"} }));
    shaders.emplace_back(GLShader("noimage", { {GLShader::VERTEX, "assets/shaders/noimage.vert"}, {GLShader::FRAGMENT, "assets/shaders/noimage.frag"} }));
    shaders.emplace_back(GLShader("noise_box", { {GLShader::VERTEX, "assets/shaders/noise.vert"}, {GLShader::FRAGMENT, "assets/shaders/noise.frag"} }));
    shaders.emplace_back(GLShader("bright", { {GLShader::VERTEX, "assets/shaders/bright.vert"}, {GLShader::FRAGMENT, "assets/shaders/bright.frag"} }));
    shaders.emplace_back(GLShader("whirl", { {GLShader::VERTEX, "assets/shaders/whirl.vert"}, {GLShader::FRAGMENT, "assets/shaders/whirl2.frag"} }));
    shaders.emplace_back(GLShader("hit", { {GLShader::VERTEX, "assets/shaders/hit.vert"}, {GLShader::FRAGMENT, "assets/shaders/hit.frag"} }));
    shaders.emplace_back(GLShader("Dark", { {GLShader::VERTEX, "assets/shaders/Dark.vert"}, {GLShader::FRAGMENT, "assets/shaders/Dark.frag"} }));
    shaders.emplace_back(GLShader("unique", { {GLShader::VERTEX, "assets/shaders/unique.vert"}, {GLShader::FRAGMENT, "assets/shaders/unique.frag"} }));
    shaders.emplace_back(GLShader("Dive_hole", { {GLShader::VERTEX, "assets/shaders/Dive_hole.vert"}, {GLShader::FRAGMENT, "assets/shaders/Dive_hole.frag"} }));
    shaders.emplace_back(GLShader("basement", { {GLShader::VERTEX, "assets/shaders/basement.vert"}, {GLShader::FRAGMENT, "assets/shaders/basement.frag"} }));

    //
}
