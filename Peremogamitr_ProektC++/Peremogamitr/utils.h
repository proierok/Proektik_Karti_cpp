#ifndef UTILS_H
#define UTILS_H

#include "storage.h"
#include <iomanip>

namespace FlashCardApp {

    inline std::ostream& operator<<(std::ostream& os, const SimpleCard& card) {
        os << "Card[" << card.getId() << "]: " << card.getQuestion()
            << " -> " << card.getAnswer();
        return os;
    }

    inline std::string operator+(const std::string& prefix, const SimpleCard& card) {
        return prefix + " | " + card.getQuestion();
    }

    inline void demoLambdaFilter(const Repository<FlashcardBase>& repo, int minDifficulty) {
        auto all = repo.getAll();
        std::cout << "\n=== Cards with difficulty >= " << minDifficulty << " ===\n";

        bool found = false;
        for (const auto& card : all) {
            if (auto simple = std::dynamic_pointer_cast<SimpleCard>(card)) {
                if (simple->getDifficulty() >= minDifficulty) {
                    card->display();
                    std::cout << std::endl;
                    found = true;
                }
            }
        }
        if (!found) std::cout << "No cards with this difficulty level.\n";
    }

    inline void showStatistics(const Repository<FlashcardBase>& repo) {
        auto all = repo.getAll();
        std::cout << "\n========== STATISTICS ==========\n";
        std::cout << "Total cards: " << all.size() << std::endl;

        int simpleCount = 0, exampleCount = 0;
        int totalDifficulty = 0;

        for (const auto& card : all) {
            if (std::dynamic_pointer_cast<SimpleCard>(card)) {
                simpleCount++;
                auto simple = std::dynamic_pointer_cast<SimpleCard>(card);
                totalDifficulty += simple->getDifficulty();
            }
            else if (std::dynamic_pointer_cast<ExampleCard>(card)) {
                exampleCount++;
            }
        }

        std::cout << "Simple cards: " << simpleCount << std::endl;
        std::cout << "Example cards: " << exampleCount << std::endl;

        if (simpleCount > 0) {
            std::cout << "Average difficulty: "
                << (totalDifficulty / (float)simpleCount) << std::endl;
        }

        if (all.size() > 0) {
            std::cout << "\nCard type distribution:\n";
            int barLength = (simpleCount * 40) / all.size();
            std::cout << "Simple:  [";
            for (int i = 0; i < barLength; i++) std::cout << "#";
            for (int i = barLength; i < 40; i++) std::cout << " ";
            std::cout << "] " << simpleCount << "\n";

            barLength = (exampleCount * 40) / all.size();
            std::cout << "Example: [";
            for (int i = 0; i < barLength; i++) std::cout << "#";
            for (int i = barLength; i < 40; i++) std::cout << " ";
            std::cout << "] " << exampleCount << "\n";
        }
        std::cout << "================================\n";
    }

    inline void demonstrateCopyConstructor() {
        std::cout << "\n=== Copy Constructor Demo ===\n";
        SimpleCard original("Hello", "Bonjour", 2);
        SimpleCard copy(original);

        std::cout << "Original: ";
        original.display();
        std::cout << std::endl;

        std::cout << "Copy:     ";
        copy.display();
        std::cout << std::endl;
        std::cout << "==============================\n";
    }

}

#endif