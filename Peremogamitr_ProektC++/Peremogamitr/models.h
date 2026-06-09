#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <algorithm>

namespace FlashCardApp {

    class CardException {
    private:
        std::string msg;
    public:
        explicit CardException(const std::string& m) : msg(m) {}
        std::string what() const { return msg; }
    };

    class FlashcardBase {
    private:
        int id;
        std::string question;
        std::string answer;
        static int nextId;

    public:
        explicit FlashcardBase(const std::string& q = "", const std::string& a = "")
            : id(nextId++), question(q), answer(a) {
        }

        virtual ~FlashcardBase() {}

        virtual void display() const = 0;

        int getId() const { return id; }
        std::string getQuestion() const { return question; }
        std::string getAnswer() const { return answer; }

        void setQuestion(const std::string& q) { question = q; }
        void setAnswer(const std::string& a) { answer = a; }

        void setIdFromThis(int newId) { this->id = newId; }

        static int getNextId() { return nextId; }
    };

    int FlashcardBase::nextId = 1;

    class SimpleCard : public FlashcardBase {
    private:
        int difficulty;

    public:
        explicit SimpleCard(const std::string& q = "", const std::string& a = "", int d = 3)
            : FlashcardBase(q, a), difficulty(d) {
        }

        void display() const override {
            std::cout << "[ID:" << getId() << "] Вопрос: " << getQuestion()
                << " | Ответ: " << getAnswer() << " | Сложность: " << difficulty;
        }

        int getDifficulty() const { return difficulty; }
    };

    class ExampleCard : public FlashcardBase {
    private:
        std::string exampleSentence;

    public:
        explicit ExampleCard(const std::string& q = "", const std::string& a = "",
            const std::string& example = "")
            : FlashcardBase(q, a), exampleSentence(example) {
        }

        void display() const override {
            std::cout << "[ID:" << getId() << "] Вопрос: " << getQuestion()
                << " | Ответ: " << getAnswer() << " | Пример: " << exampleSentence;
        }

        std::string getExample() const { return exampleSentence; }
    };

}

#endif