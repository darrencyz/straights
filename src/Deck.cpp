#include "Deck.h"
#include <iostream>
#include <random>

// Constructor
//
// @param seedVal: seed value for shuffling algorithm
Deck::Deck(const int seedVal) {
    seed_ = seedVal;
    prepareDeck();
}

// Destructor (call clearDeck function)
Deck::~Deck() {
    clearDeck();
}

// Accessor for a pointer to the first card object
Card** Deck::getCards() {
    return cards_;
}

// Prepare a deck by creating 52 cards
// 13 ranks per suit & 4 suit
void Deck::prepareDeck() {
    int cardCounter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            Card* card = new Card(Suit(i), Rank(j));
            cards_[cardCounter] = card;
            cardCounter++;
        }
    }
}

// Remove all cards
// Deallocate card pointers in card array (used for destructor)
void Deck::clearDeck() {
    for (int i = 0; i < CARD_COUNT; i++) {
        delete cards_[i];
    }
}

// Shuffle the deck
// Using provided shuffling algorithm (fetch seed)
void Deck::shuffle() {
    static std::mt19937 rng(seed_);
    int n = CARD_COUNT;

    while (n > 1) {
        int k = (int) (rng() % n);
        --n;
        Card *c = cards_[n];
        cards_[n] = cards_[k];
        cards_[k] = c;
    }
}

