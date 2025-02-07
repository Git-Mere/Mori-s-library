#include <filesystem>
#include "Animation.h"
#include "Engine.h"
#include <iostream>

Animation::Animation(const std::filesystem::path& animation_file) : current_command(0) {
	if (animation_file.extension() != ".anm") {
		throw std::runtime_error(animation_file.generic_string() + " is not a .anm file");
	}
	std::ifstream in_file(animation_file);
	if (in_file.is_open() == false) {
		throw std::runtime_error("Failed to load " + animation_file.generic_string());
	}

	std::string command;
	while (in_file.eof() == false) {
		in_file >> command;
		if (command == "PlayFrame") {
			int frame;
			float target_time;
			in_file >> frame;
			in_file >> target_time;

			commands.push_back(new PlayFrame(frame, target_time));
		}
		else if (command == "YPlayFrame") {
			int y_value;
			in_file >> y_value;
			static_cast<PlayFrame*>(commands[commands.size() - 1])->Set_y_value(y_value);
		}
		else if (command == "Loop") {
			int loop_to_frame;
			in_file >> loop_to_frame;
			commands.push_back(new Loop(loop_to_frame));
		}
		else if (command == "End") {
			commands.push_back(new End());
		}
		else {
			Engine::GetLogger().LogError(command + " in " + animation_file.generic_string());
		}
	}
	Reset();
}


Animation::Animation() : Animation("assets/sprite/None.anm") { }


Animation::~Animation() {
	for (Command* command : commands) {
		delete command;
	}
	commands.clear();
}

ivec2 Animation::CurrentFrame() {
	return current_frame->Frame();
}

void Animation::Reset()
{
	current_command = 0;
	ended = false;
	current_frame = static_cast<PlayFrame*>(commands[current_command]);
}

bool Animation::Ended() {
	return ended;
}

void Animation::Set_current(int index)
{
	current_command = index;
}

void Animation::Update(float dt) {
	current_frame->Update(dt);
	if (current_frame->Ended() == true) {
		current_frame->ResetTime();
		current_command++;
		if (commands[current_command]->Type() == CommandType::PlayFrame) {
			current_frame = static_cast<PlayFrame*>(commands[current_command]);
		}
		else if (commands[current_command]->Type() == CommandType::Loop) {
			Loop* loop_data = static_cast<Loop*>(commands[current_command]);
			current_command = loop_data->LoopIndex();
			if (commands[current_command]->Type() == CommandType::PlayFrame) {
				current_frame = static_cast<PlayFrame*>(commands[current_command]);
			}
			else {
				Engine::GetLogger().LogError("Loop does not go to PlayFrame");
				Reset();
			}
		}
		else if (commands[current_command]->Type() == CommandType::End) {
			ended = true;
		}
	}
}

Animation::PlayFrame::PlayFrame(int frame, double duration) : frame(ivec2(frame, 1)), target_time(duration), timer(0)
{
}

void Animation::PlayFrame::Update(float dt)
{
	if (timer <= target_time) {
		timer += dt;
	}
}

bool Animation::PlayFrame::Ended()
{
	if (timer > target_time) { return true; }
	return false;
}

void Animation::PlayFrame::ResetTime()
{
	timer = 0;
}

void Animation::PlayFrame::Set_y_value(int value)
{
	frame.y = value;
}

ivec2 Animation::PlayFrame::Frame()
{
	return frame;
}

Animation::Loop::Loop(int loop_index) : loop_index(loop_index)
{
}

int Animation::Loop::LoopIndex()
{
	return loop_index;
}
