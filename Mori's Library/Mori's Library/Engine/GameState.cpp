#include "GameState.h"
#include <fstream>
#include <string>
#include <angles.h>
#include <vec2.h>
#include <gl/GL.h>
#include <GLVertexArray.h>
#include "GameObject.h"
#include <GLShader.h>
#include "../Game/sample_logo.h"

//void GameState::load_scene(const std::filesystem::path& scene_file_path, std::vector<GameObject>& objects, CamInfo& camera, std::unordered_map<std::string, unsigned>& asset_to_index, std::vector<GLVertexArray>& models, std::vector<GLShader>& shaders)
//{
//    std::ifstream sceneFile{ scene_file_path };
//    if (!sceneFile)
//    {
//        throw std::runtime_error(std::string("ERROR: Unable to open scene file: ") + scene_file_path.string());
//    }
//
//    std::string line;
//    std::getline(sceneFile, line);
//
//    int objectCount;
//    std::istringstream{ line } >> objectCount;
//    objects.reserve(static_cast<unsigned>(objectCount));
//
//    std::string           modelName;
//    std::string           objectName;
//    std::string           shaderProgramName;
//    std::filesystem::path vertexShaderPath, fragmentShaderPath;
//    float                 rotationAngular, rotationSpeedAngular;
//    //int what_object = 0;
//
//    for (int i = 0; i < objectCount; ++i)
//    {
//        GameObject object;
//
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> modelName;
//
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> object.name;
//
//        object.model_index = load_mesh_and_get_id(std::filesystem::path("assets") / "meshes" / (modelName + ".msh"), asset_to_index, models);
//
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> shaderProgramName >> vertexShaderPath >> fragmentShaderPath;
//        object.shader_index = load_shader_and_get_id(shaderProgramName, vertexShaderPath, fragmentShaderPath, asset_to_index, shaders);
//
//        // RGB Color
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> object.color.red >> object.color.green >> object.color.blue;
//        // Width Height
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> object.scaling.x >> object.scaling.y;
//        // Orientation, Rotation Speed
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> rotationAngular >> rotationSpeedAngular;
//        object.angle_disp = util::to_radians(rotationAngular);
//        object.angle_speed = util::to_radians(rotationSpeedAngular);
//        // 2D Position
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> object.position.x >> object.position.y;
//
//        std::getline(sceneFile, line);
//        std::istringstream{ line } >> object.spritePath;
//
//        if (object.name != "Camera")
//        {
//            objects.emplace_back(std::move(object));
//            //objects.push_back(std::move(object));
//        }
//        else
//        {
//            camera.obj = object;
//            camera.camera.SetOrientation(object.angle_disp);
//            camera.camera.Position = object.position;
//        }
//    }
//}
//
//unsigned GameState::load_mesh_and_get_id(const std::filesystem::path& mesh_file_path, std::unordered_map<std::string, unsigned>& asset_to_index, std::vector<GLVertexArray>& models)
//{
//    auto [iter, is_new] = asset_to_index.insert({ mesh_file_path.string(), 0 });
//    auto& location = iter->second;
//    if (!is_new)
//    {
//        return location;
//    }
//
//    std::ifstream meshFile{ mesh_file_path };
//    if (!meshFile)
//    {
//        throw std::runtime_error{ std::string("ERROR: Unable to open mesh file: ") + mesh_file_path.string() };
//    }
//
//    std::vector<vec2>           vertex_positions;
//    std::vector<vec2>           tex_pos_vtx;
//    std::vector<color3>           clr_vtx;
//    std::vector<unsigned short> vertex_indices;
//
//    std::string       modelName;
//    float             x = 0, y = 0;
//    GLushort          index = 0;
//    std::string       line;
//    char              mode = 0;
//    GLPrimitive::Type primitive_type = GLPrimitive::Points;
//    bool three = true;
//    float red = 0, green = 0, blue = 0;
//
//    while (meshFile)
//    {
//        std::getline(meshFile, line);
//        if (line.empty())
//        {
//            continue;
//        }
//
//        std::istringstream sstr{ line };
//        sstr >> mode;
//        switch (mode) {
//        case 'n':
//            sstr >> modelName;
//            break;
//        case 'v':
//            sstr >> x >> y;
//            vertex_positions.emplace_back(x, y);
//            break;
//        case 't':
//            primitive_type = GLPrimitive::Triangles;
//            sstr >> index;
//            vertex_indices.push_back(index);
//            sstr >> index;
//            vertex_indices.push_back(index);
//            sstr >> index;
//            vertex_indices.push_back(index);
//            break;
//        case 'f':
//            primitive_type = GLPrimitive::TriangleFan;
//            if (three) {
//                sstr >> index;
//                vertex_indices.push_back(index);
//                sstr >> index;
//                vertex_indices.push_back(index);
//                sstr >> index;
//                vertex_indices.push_back(index);
//                three = false;
//            }
//            else {
//                sstr >> index;
//                vertex_indices.push_back(index);
//            }
//            break;
//        case 'c':
//            sstr >> red >> green >> blue;
//            clr_vtx.emplace_back(red, green, blue);
//            break;
//        case 'x':
//            sstr >> x >> y;
//            tex_pos_vtx.emplace_back(x, y);
//            break;
//        }
//    }
//
//    GLVertexArray model(primitive_type);
//
//    GLsizei       positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * vertex_positions.size());
//    GLsizei       tex_positions_byte_size = static_cast<GLsizei>(sizeof(vec2) * tex_pos_vtx.size());
//    GLsizei       colors_byte_size = static_cast<GLsizei>(sizeof(color3) * clr_vtx.size());
//
//    GLsizei       buffer_size = positions_byte_size + colors_byte_size + tex_positions_byte_size;
//    GLVertexBuffer       buffer(buffer_size);
//
//    glCheck(buffer.SetData(std::span(vertex_positions)));
//    glCheck(buffer.SetData(std::span(tex_pos_vtx), positions_byte_size));
//    glCheck(buffer.SetData(std::span(clr_vtx), positions_byte_size + tex_positions_byte_size));
//
//    glCheck(model.SetVertexCount(static_cast<int>(vertex_positions.size())));
//
//    GLAttributeLayout position;
//    position.component_type = GLAttributeLayout::Float;
//    position.component_dimension = GLAttributeLayout::_2;
//    position.normalized = false;
//    position.vertex_layout_location = 0; // 1st field is 0 index based
//    position.stride = sizeof(vec2);
//    position.offset = 0;
//    position.relative_offset = 0;
//
//    GLAttributeLayout tex_position;
//    tex_position.component_type = GLAttributeLayout::Float;
//    tex_position.component_dimension = GLAttributeLayout::_2;
//    tex_position.normalized = false;
//    tex_position.vertex_layout_location = 1; // 1st field is 0 index based
//    tex_position.stride = sizeof(vec2);
//    tex_position.offset = positions_byte_size;
//    tex_position.relative_offset = 0;
//
//    GLAttributeLayout color;
//    color.component_type = GLAttributeLayout::Float;
//    color.component_dimension = GLAttributeLayout::_3;
//    color.normalized = false;
//    color.vertex_layout_location = 2; // 2nd field of Vertex
//    color.stride = sizeof(color3);
//    color.offset = positions_byte_size + tex_positions_byte_size; // starts after the position bytes
//    color.relative_offset = 0;
//
//    GLIndexBuffer       index_buffer(vertex_indices);
//
//    glCheck(model.AddVertexBuffer(std::move(buffer), { position, tex_position, color }));
//    glCheck(model.SetIndexBuffer(std::move(index_buffer)));
//
//    location = static_cast<unsigned>(models.size());
//    models.emplace_back(std::move(model));
//
//    return location;
//}
//
//unsigned GameState::load_shader_and_get_id(std::string_view name, const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath
//    , std::unordered_map<std::string, unsigned>& asset_to_index, std::vector<GLShader>& shaders)
//{
//    // Insert if 1st time otherwise get what is there
//    auto [iter, is_new] = asset_to_index.insert({ std::string(name), 0 });
//    auto& location = iter->second;
//    if (!is_new)
//    {
//        return location;
//    }
//    location = static_cast<unsigned>(shaders.size());
//    shaders.emplace_back(GLShader(name, { {GLShader::VERTEX, vertex_filepath}, {GLShader::FRAGMENT, fragment_filepath} }));
//    return location;
//}