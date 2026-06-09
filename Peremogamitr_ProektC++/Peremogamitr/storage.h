#ifndef STORAGE_H
#define STORAGE_H

#include "models.h"

namespace FlashCardApp {

    template <typename T>
    class Repository {
    private:
        std::vector<std::shared_ptr<T>> items;
        std::string filename;

    public:
        explicit Repository(const std::string& fname) : filename(fname) {}

        void add(std::shared_ptr<T> item) {
            if (!item) throw CardException("Нельзя добавить пустую карточку");
            items.push_back(item);
        }

        std::shared_ptr<T> getById(int id) {
            for (auto& card : items) {
                if (card->getId() == id) return card;
            }
            return nullptr;
        }

        bool removeById(int id) {
            for (size_t i = 0; i < items.size(); i++) {
                if (items[i]->getId() == id) {
                    items.erase(items.begin() + i);
                    return true;
                }
            }
            return false;
        }

        void updateById(int id, const std::string& newQ, const std::string& newA) {
            auto card = getById(id);
            if (!card) throw CardException("Карточка не найдена");
            card->setQuestion(newQ);
            card->setAnswer(newA);
        }

        std::vector<std::shared_ptr<T>> getAll() const { return items; }

        size_t size() const { return items.size(); }

        void sortByQuestion() {
            for (size_t i = 0; i < items.size(); i++) {
                for (size_t j = i + 1; j < items.size(); j++) {
                    if (items[i]->getQuestion() > items[j]->getQuestion()) {
                        std::swap(items[i], items[j]);
                    }
                }
            }
        }

        void saveToFile() {
            std::ofstream out(filename);
            if (!out) throw CardException("Не удалось открыть файл");

            for (const auto& card : items) {
                if (auto simple = std::dynamic_pointer_cast<SimpleCard>(card)) {
                    out << "SIMPLE|" << card->getId() << "|"
                        << card->getQuestion() << "|"
                        << card->getAnswer() << "|"
                        << simple->getDifficulty() << "\n";
                }
                else if (auto example = std::dynamic_pointer_cast<ExampleCard>(card)) {
                    out << "EXAMPLE|" << card->getId() << "|"
                        << card->getQuestion() << "|"
                        << card->getAnswer() << "|"
                        << example->getExample() << "\n";
                }
            }
            out.close();
        }

        void loadFromFile() {
            items.clear();
            std::ifstream in(filename);
            if (!in) return;

            std::string line;
            while (std::getline(in, line)) {
                std::string type, q, a, extra;
                int id;

                size_t pos = 0;
                size_t next = line.find('|');
                type = line.substr(0, next);
                pos = next + 1;

                next = line.find('|', pos);
                id = std::stoi(line.substr(pos, next - pos));
                pos = next + 1;

                next = line.find('|', pos);
                q = line.substr(pos, next - pos);
                pos = next + 1;

                next = line.find('|', pos);
                a = line.substr(pos, next - pos);
                pos = next + 1;

                extra = line.substr(pos);

                if (type == "SIMPLE") {
                    int diff = std::stoi(extra);
                    auto card = std::make_shared<SimpleCard>(q, a, diff);
                    card->setIdFromThis(id);
                    items.push_back(card);
                }
                else if (type == "EXAMPLE") {
                    auto card = std::make_shared<ExampleCard>(q, a, extra);
                    card->setIdFromThis(id);
                    items.push_back(card);
                }
            }
            in.close();
        }
    };

    class ActionStack {
    private:
        struct Node {
            std::string data;
            std::unique_ptr<Node> next;
            Node(const std::string& d) : data(d), next(nullptr) {}
        };
        std::unique_ptr<Node> top;

    public:
        ActionStack() : top(nullptr) {}

        void push(const std::string& item) {
            auto newNode = std::make_unique<Node>(item);
            newNode->next = std::move(top);
            top = std::move(newNode);
        }

        std::string pop() {
            if (isEmpty()) return "";
            std::string result = top->data;
            top = std::move(top->next);
            return result;
        }

        bool isEmpty() const { return top == nullptr; }
    };

} 

#endif