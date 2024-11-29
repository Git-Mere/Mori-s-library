//Junseop's ddong
void Player::Play_Dash_Animation(int animation_number, float dt) {
    player_move_animation = false;
    dash_animation_count += dt;
    switch (animation_number) {
    case 1:
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Down));
        if (!dash_sound) {
            PlaySoundEffect(Dash);
            dash_sound = true;
        }
        if (dash_animation_count >= 0.5f) {
            dash_animation_count = 0.0f;
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Down));
            player_move_animation = true;
            dash_sound = false;

        }
        break;

    case 2:
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Up));
        if (!dash_sound) {
            PlaySoundEffect(Dash);
            dash_sound = true;
        }
        if (dash_animation_count >= 0.5f) {
            dash_animation_count = 0.0f;
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Up));
            player_move_animation = true;
            dash_sound = false;
        }
        break;

    case 3:
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Left));
        if (!dash_sound) {
            PlaySoundEffect(Dash);
            dash_sound = true;
        }
        if (dash_animation_count >= 0.5f) {
            dash_animation_count = 0.0f;
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Left));
            player_move_animation = true;
            dash_sound = false;
        }
        break;

    case 4:
        GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Dash_Right));
        if (!dash_sound) {
            PlaySoundEffect(Dash);
            dash_sound = true;
        }
        if (dash_animation_count >= 0.5f) {
            dash_animation_count = 0.0f;
            GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Move_Right));
            player_move_animation = true;
            dash_sound = false;
        }
        break;
    }
}

unsigned GameObject::load_mesh_and_get_id(const std::filesystem::path& mesh_file_path)
{
    auto [iter, is_new] = asset_to_index.insert({ mesh_file_path.string(), 0 });
    auto& location = iter->second;
    if (!is_new)
    {
        return location;
    }
    std::vector<GLVertexArray>* tmp = new std::vector<GLVertexArray>();


    std::ifstream meshFile{ mesh_file_path };
    if (!meshFile)
    {
        throw std::runtime_error{ std::string("ERROR: Unable to open mesh file: ") + mesh_file_path.string() };
    }

    std::vector<vec2>           vertex_positions;
    std::vector<vec2>           tex_pos_vtx;
    std::vector<color3>           clr_vtx;
    std::vector<unsigned short> vertex_indices;

    std::string       modelName;
    float             x = 0, y = 0;
    GLushort          index = 0;
    std::string       line;
    char              mode = 0;
    GLPrimitive::Type primitive_type = GLPrimitive::Points;
    bool three = true;
    float red = 0, green = 0, blue = 0;

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
        case 'c':
            sstr >> red >> green >> blue;
            clr_vtx.emplace_back(red, green, blue);
            break;
        case 'x':
            sstr >> x >> y;
            tex_pos_vtx.emplace_back(x, y);
            break;
        }
    }

    GLVertexArray model(primitive_type);

    GLVertexBuffer    position_buffer(std::span{ vertex_positions });
    GLVertexBuffer    colors_buffer(std::span{ clr_vtx });
    GLVertexBuffer    tex_buffer(std::span{ tex_pos_vtx });

    position_vtx.push_back(tex_pos_vtx);

    glCheck(model.SetVertexCount(static_cast<int>(vertex_positions.size())));

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
    model.AddVertexBuffer(std::move(colors_buffer), { glcolor });
    model.AddVertexBuffer(std::move(tex_buffer), { tex_position });

    GLIndexBuffer       index_buffer(vertex_indices);
    glCheck(model.SetIndexBuffer(std::move(index_buffer)));


    location = static_cast<unsigned>(tmp->size());
    tmp->emplace_back(std::move(model));
    models = tmp;
    return location;
}

void GameObject::omit_tex_coordinate()
{
    std::vector<vec2>  tex_pos_vtx = position_vtx[0];
    float frame = GetGOComponent<Sprite>()->get_frame();
    //if (frame == 0) {
    //    frame = 1;
    //}
    float how = 1.0f / GetGOComponent<Sprite>()->get_Numframe();
    tex_pos_vtx[0].x = how * frame - how;
    tex_pos_vtx[1].x = how * frame;
    tex_pos_vtx[2].x = how * frame;
    tex_pos_vtx[3].x = how * frame - how;
    //for (auto& a : tex_pos_vtx) {
    //    a.x *= how;
    //}

    GLVertexBuffer    tex_buffer(std::span{ tex_pos_vtx });

    GLAttributeLayout tex_position;
    tex_position.component_type = GLAttributeLayout::Float;
    tex_position.component_dimension = GLAttributeLayout::_2;
    tex_position.normalized = false;
    tex_position.vertex_layout_location = 1; // 1st field is 0 index based
    tex_position.stride = sizeof(vec2);
    tex_position.offset = 0;
    tex_position.relative_offset = 0;

    models->at(0).AddVertexBuffer(std::move(tex_buffer), { tex_position });
}

unsigned GameObject::load_shader_and_get_id(std::string_view shader_name, const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath
)
{
    // Insert if 1st time otherwise get what is there
    auto [iter, is_new] = asset_to_index.insert({ std::string(shader_name), 0 });
    auto& location = iter->second;
    if (!is_new)
    {
        return location;
    }

    std::vector<GLShader>* tmp = new std::vector<GLShader>();

    location = static_cast<unsigned>(tmp->size());
    tmp->emplace_back(GLShader(name, { {GLShader::VERTEX, vertex_filepath}, {GLShader::FRAGMENT, fragment_filepath} }));
    shaders = tmp;
    return location;
}

std::span<const float, 3 * 3> GameObject::to_span(const mat3& m)
{
    return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
}
std::span<const float, 3> GameObject::to_span(const color3& c)
{
    return std::span<const float, 3>(&c.elements[0], 3);
}

Math::ivec2 Sprite::GetFrameTexel(int idx) const
{
    if (idx < 0 || idx >= frame_texels.size()) {
        Engine::GetLogger().LogError("Invalid index");
        return { 0,0 };
    }
    return frame_texels[idx];
}

void Sprite::load_shader(std::string_view shader_name, const std::filesystem::path& vertex_filepath, const std::filesystem::path& fragment_filepath)
{
    shader = (GLShader(shader_name, { {GLShader::VERTEX, vertex_filepath}, {GLShader::FRAGMENT, fragment_filepath} }));
}