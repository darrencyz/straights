#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <cstdlib>

#define CARD_COUNT 52
#define RANK_COUNT 13

class Deck {
public:
    Deck(const int seedVal);        // constructor
    ~Deck();                        // destructor

    // Accessor
    Card** getCards();
    
    // Mutator
    void setSeedVal(int);

    // Member Functions
    void clearDeck();
    void shuffle();

private:
    void prepareDeck();
    
    // Data Members
    int seed_;
    Card* cards_[CARD_COUNT];
};

#endif

