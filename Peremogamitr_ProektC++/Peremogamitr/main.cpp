#include "models.h"
#include "storage.h"
#include <windows.h>

using namespace FlashCardApp;

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Repository<FlashcardBase> repo("data.txt");
    repo.loadFromFile();
    ActionStack undoStack;

    int choice;

    do {
        std::cout << "\n========================================\n";
        std::cout << "     КАРТОЧКИ ДЛЯ ИЗУЧЕНИЯ ЯЗЫКОВ      \n";
        std::cout << "========================================\n";
        std::cout << " 1. Добавить обычную карточку\n";
        std::cout << " 2. Добавить карточку с примером\n";
        std::cout << " 3. Показать все карточки\n";
        std::cout << " 4. Редактировать карточку\n";
        std::cout << " 5. Удалить карточку\n";
        std::cout << " 6. Поиск\n";
        std::cout << " 7. Сортировка по вопросу\n";
        std::cout << " 8. Сохранить\n";
        std::cout << " 0. Выход\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string q, a;
            int diff;
            std::cout << "Вопрос: "; std::getline(std::cin, q);
            std::cout << "Ответ: "; std::getline(std::cin, a);
            std::cout << "Сложность (1-5): "; std::cin >> diff; std::cin.ignore();
            if (diff < 1) diff = 1;
            if (diff > 5) diff = 5;
            auto card = std::make_shared<SimpleCard>(q, a, diff);
            repo.add(card);
            undoStack.push("Добавлена: " + q);
            std::cout << "Добавлено! ID: " << card->getId() << "\n";
        }
        else if (choice == 2) {
            std::string q, a, ex;
            std::cout << "Вопрос: "; std::getline(std::cin, q);
            std::cout << "Ответ: "; std::getline(std::cin, a);
            std::cout << "Пример: "; std::getline(std::cin, ex);
            auto card = std::make_shared<ExampleCard>(q, a, ex);
            repo.add(card);
            undoStack.push("Добавлена с примером: " + q);
            std::cout << "Добавлено! ID: " << card->getId() << "\n";
        }
        else if (choice == 3) {
            auto all = repo.getAll();
            if (all.empty()) {
                std::cout << "Нет карточек\n";
            }
            else {
                std::cout << "\n=== ВСЕ КАРТОЧКИ ===\n";
                for (auto& card : all) {
                    card->display();
                    std::cout << "\n";
                }
                std::cout << "Всего: " << all.size() << "\n";
            }
        }
        else if (choice == 4) {
            int id;
            std::string newQ, newA;
            std::cout << "ID: "; std::cin >> id; std::cin.ignore();
            auto card = repo.getById(id);
            if (!card) {
                std::cout << "Не найдено\n";
            }
            else {
                std::cout << "Новый вопрос: "; std::getline(std::cin, newQ);
                std::cout << "Новый ответ: "; std::getline(std::cin, newA);
                if (!newQ.empty()) card->setQuestion(newQ);
                if (!newA.empty()) card->setAnswer(newA);
                std::cout << "Обновлено\n";
            }
        }
        else if (choice == 5) {
            int id;
            std::cout << "ID: "; std::cin >> id;
            if (repo.removeById(id)) {
                undoStack.push("Удален ID " + std::to_string(id));
                std::cout << "Удалено\n";
            }
            else {
                std::cout << "Не найдено\n";
            }
        }
        else if (choice == 6) {
            std::string kw;
            std::cout << "Поиск: "; std::cin >> kw;
            auto all = repo.getAll();
            bool found = false;
            for (auto& card : all) {
                if (card->getQuestion().find(kw) != std::string::npos ||
                    card->getAnswer().find(kw) != std::string::npos) {
                    card->display();
                    std::cout << "\n";
                    found = true;
                }
            }
            if (!found) std::cout << "Ничего не найдено\n";
        }
        else if (choice == 7) {
            repo.sortByQuestion();
            std::cout << "Отсортировано!\n";
        }
        else if (choice == 8) {
            repo.saveToFile();
            std::cout << "Сохранено в data.txt\n";
        }

    } while (choice != 0);

    std::cout << "До свидания!\n";
    return 0;
}