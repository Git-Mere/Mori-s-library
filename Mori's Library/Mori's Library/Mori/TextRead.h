#pragma once
#include "../Engine/Logger.h"
#include "../Engine/Engine.h"
#include <vector>
#include <string>

struct Sentence {
    std::vector<std::string> sentences;
    Sentence(){}
    Sentence(const std::vector<std::string>& input_sentences) : sentences(input_sentences) {}

    std::string getSentences(int index) const {
        if (index >= 0 && index < sentences.size()) {
            return sentences[index];
        }
        else {
            return "Out of Index.....";
        }
    }
};

//just read  //ex)Sentence quiz = loadSentencesFromFile<Sentence>("assets/textfile/Quiz.txt");  //use)quiz.getSentences(1)
template<typename Struct>
Struct loadSentencesFromFile(const std::string& filename) {
    std::vector<std::string> sentences;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            sentences.push_back(line);
        }
        file.close();
    }
    else {
        Engine::GetLogger().LogError("Invalid path.....");
    }

    return Struct(sentences);
}

template<typename Struct>
Struct loadQuizSentencesFromFile(const std::string& filename) {
    std::vector<std::string> sentences;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            sentences.push_back(line);
        }
        file.close();
    }
    else {
        Engine::GetLogger().LogError("Invalid path.....");
    }

    return Struct(sentences);
}