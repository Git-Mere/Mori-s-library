#pragma once
#include "Logger.h"
#include "GameStateManager.h"
#include "Input.h"
#include "TextureManager.h"
#include <chrono>
#include "IProgram.h"
#include <GL/glew.h>
#include <glCheck.h>
#include "Vec2.h"
#include <GLVertexArray.h>
#include <GLShader.h>
#include "Font.h"

class Engine : public IProgram {
public:
    enum GameMode {
        Easy = 1,
        Hard
    };
    //모델 4개
    enum ModelName {
        Rect,
        Circle
    };

    //쉐이더들
    //쉐이더 추가할 때마다 여기에도 추가해줘야 함
    enum ShaderName {
        basic, // game object에 기본값으로 쓰이는 쉐이더, 기본적으로 텍스쳐를 쓰는 쉐이더
        collision, // show collision을 사용할 때만 쓰이는 쉐이더(굳이 쓸 필요 없음)
        logo, // digipen logo 그릴 떄만 쓰이는 쉐이더, 이건 이미지가 좀 예외적으로 생겨서 만들어서 써야 했음
        noimage, //텍스쳐 없이 모델만 그릴 때 사용하는 쉐이더
        noise_box, //스크린 노이즈 용 쉐이더
        bright, // 스크린 밝아지는 거 용 쉐이더
        whirl, // 빙글빙글 돌아가는 맷돌
        hit, //맞으면 표시되는 거
        Dark, //Game over
        unique,
        Dive_hole,
        Basement
    };
    Engine();
    ~Engine();

    static Engine* Instance() {
        static Engine* instance = new Engine;
        //static Engine instance;
        return instance;
    }
    static TextureManager& GetTextureManager() {
        return Instance()->texturemanager;
    }
    static Logger& GetLogger() {
        return Instance()->logger;
    }
    static GameStateManager& GetGameStateManager() {
        return Instance()->gamestatemanager;
    }
    static Input& GetInput() {
        return Instance()->input;
    }

    //윈도우 사이즈 재조정 하는 함수
    void Set_Window(float viewport_width, float viewport_height) {
        Window_Size = vec2(viewport_width, viewport_height);
        glCheck(glViewport(0, 0, static_cast<int>(viewport_width), static_cast<int>(viewport_height)));
    }

    //윈도우 사이즈 리턴하는 함수
    const vec2& Get_Window() const {
        return Window_Size;
    }
    const ivec2 Get_iWindow() const {
        int x = static_cast<int>(Window_Size.x);
        int y = static_cast<int>(Window_Size.y);
        return ivec2(x,y);
    }
    const std::vector<vec2> Get_texture_ndc() const {
        return tex_positions;
    }

    const GameMode Get_Game_mode() const {
        return gamemode;
    }
    const GLVertexArray& Get_Model(ModelName name) const {
        return models[name];
    }
    const GLShader& Get_Shader(ShaderName name) const {
        return shaders[name];
    }
    static Font& GetFont(int index) {
        return Instance()->fonts[index];
    }

    void AddFont(const std::filesystem::path& file_name);

    static bool HasGameEnded() {
        return Instance()->GetGameStateManager().HasGameEnded();
    }

    void Start(std::string window_title);
    void Stop();
    
    void Update() override;
    void Draw() override;
    void ImGuiDraw() override;
    void HandleEvent([[maybe_unused]] SDL_Window& sdl_window, [[maybe_unused]] const SDL_Event& event) override;

    void on_window_resized([[maybe_unused]]SDL_Window& sdl_window) noexcept;
    void Set_Gamemode(GameMode);
private:
    void update_timing() noexcept;
    void Createmodels();
    void CreateRectmodel();
    void CreateCirclemodel(const std::filesystem::path& mesh_file_path);
    void Createshaders();

    vec2 Window_Size;

    GameMode gamemode = Easy;

    struct
    {
        unsigned long long prev_time = 0;
        unsigned long long start_time = 0;
        float              fps = 0;
        float              ElapsedTime = 0;
        float              dt = 0;
        bool               check = false;
        int                count = 0;
    } timing;

    std::chrono::system_clock::time_point last_tick = std::chrono::system_clock::now(); 

    Logger logger;
    GameStateManager gamestatemanager;
    Input input;
    TextureManager texturemanager;

    std::vector<GLVertexArray> models;
    std::vector<GLShader> shaders;
    std::vector<vec2> tex_positions;
    std::vector<Font> fonts;
};
