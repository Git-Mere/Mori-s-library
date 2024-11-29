
#pragma once
#include <vector>
#include <vec2.h>

class Animation {
public:
	Animation();
	Animation(const std::filesystem::path& animation_file);
	~Animation();

	void Update(float dt);
	ivec2 CurrentFrame();
	void Reset();
	bool Ended();
	void Set_current(int index);
private:
	enum class CommandType {
		PlayFrame,
		Loop,
		End,
	};

	class Command {
	public:
		virtual ~Command() {}
		virtual CommandType Type() = 0;
	};

	class End : public Command {
	public:
		virtual CommandType Type() override { return CommandType::End; }
	private:
	};

	class Loop : public Command {
	public:
		Loop(int loop_index);
		virtual CommandType Type() override { return CommandType::Loop; }
		int LoopIndex();
	private:
		int loop_index;
	};

	class PlayFrame : public Command {
	public:
		PlayFrame(int frame, double duration);
		virtual CommandType Type() override { return CommandType::PlayFrame; }
		void Update(float dt);
		bool Ended();
		void ResetTime();
		void Set_y_value(int value);
		ivec2 Frame();
	private:
		ivec2 frame;
		double target_time;
		double timer;
	};

	int current_command;
	std::vector<Command*> commands;
	bool ended;
	PlayFrame* current_frame;
};

