// deck.cpp
#include "deck.h"
#include <QRandomGenerator>

Deck::Deck(QObject *parent) : QObject(parent), currentIndex(0)
{
    initializeDeck();
    shuffleDeck();
}

void Deck::initializeDeck()
{
    // Clear existing cards
    cards.clear();

    // Add 40 cards to the deck
    for (int color = 0; color < 4; ++color) {
        for (int number = 0; number < 10; ++number) {
            carte* card = new carte(nullptr, color, number);
            cards.append(card);
        }
    }
}

void Deck::shuffleDeck()
{
    // Shuffle the deck using Fisher-Yates algorithm
    int n = cards.size();
    for (int i = n - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        cards.swapItemsAt(i, j);
    }

    // Reset the current index
    currentIndex = 0;
}

carte* Deck::dealCard()
{
    if (currentIndex < cards.size()) {
        return cards.at(currentIndex++);
    } else {
        return nullptr; // Deck is empty
    }
}
bool Deck::isEmpty() const
{
    return currentIndex >= cards.size();
}
