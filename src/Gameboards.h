#ifndef GAMEBOARDS_H
#define GAMEBOARDS_H

#include "Card.h"

#define CARD_COUNT 52
#define RANK_COUNT 13

class Gameboards {
public:
    Gameboards();   // constructor
    ~Gameboards();  // destructor

    // Accessors
    Card* getLastPlayedCard() const;
    Card** getPlayedCards();
    int getPlayedCardIndex() const;
    Card** getSpades();
    Card** getHearts();
    Card** getClubs();
    Card** getDiamonds();

    // Member Functions
    void addCard(Card*);
    void clearBoard();

private:
    // Private Member Function
    std::string convertRank(int) const;
    
    // Data Members
    Card* spades_[SUIT_COUNT];
    Card* hearts_[SUIT_COUNT];
    Card* clubs_[SUIT_COUNT];
    Card* diamonds_[SUIT_COUNT];
    Card* lastPlayedCard_;
    Card* playedCards_[CARD_COUNT];
    int playedCardsIndex_;
};

#endif

