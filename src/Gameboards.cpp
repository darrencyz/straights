#include "Gameboards.h"
#include <iostream>
#include <string>

// Constructor
Gameboards::Gameboards() : lastPlayedCard_(NULL), playedCardsIndex_(0) {

}

// Destructor
Gameboards::~Gameboards() {

}

//-------------------Accessors-------------------//
// Access last played card
Card* Gameboards::getLastPlayedCard() const {
    return lastPlayedCard_;
}

// Access array of played cards
Card** Gameboards::getPlayedCards() {
    return playedCards_;
}

// Access the index of the played cards array
int Gameboards::getPlayedCardIndex() const {
    return playedCardsIndex_;
}

// Access array of played spades
Card** Gameboards::getSpades() {
    return spades_;
}

// Access array of played hearts
Card** Gameboards::getHearts() {
    return hearts_;
}

// Access array of played clubs
Card** Gameboards::getClubs() {
    return clubs_;
}

// Access array of played diamonds
Card** Gameboards::getDiamonds() {
    return diamonds_;
}
//------------------------------------------------//


// Add card to the board
//
// @param newCard: a new card to be added
void Gameboards::addCard(Card* newCard) {

    // Add card to its corresponding list
    switch (newCard->getSuit()) {
        case SPADE:
            spades_[newCard->getRank()] = newCard;
            break;
        case HEART:
            hearts_[newCard->getRank()] = newCard;
            break;
        case CLUB:
            clubs_[newCard->getRank()] = newCard;
            break;
        case DIAMOND:
            diamonds_[newCard->getRank()] = newCard;
            break;
        default:
            throw "Invalid suit";
    }
  
    // Update the last played card and update list of played cards
    lastPlayedCard_ = newCard;
    playedCards_[playedCardsIndex_] = newCard;
    playedCardsIndex_++;
    if (playedCardsIndex_ >= CARD_COUNT) {
        playedCardsIndex_ = 0;
    }
}

// Clear everything in the board
// Deallocate all pointers in the card arrays (data members)
void Gameboards::clearBoard() {
    for (int i = 0; i < RANK_COUNT; i++) {
        spades_[i] = NULL;
        hearts_[i] = NULL;
        diamonds_[i] = NULL;
        clubs_[i] = NULL;
    }

    for (int i = 0; i < CARD_COUNT; i++) {
        playedCards_[i] = NULL;
    }

    lastPlayedCard_ = NULL;
    playedCardsIndex_ = 0;
}

// Convert numeric rank to its string representation
//
// @param index: numeric rank
std::string Gameboards::convertRank(int index) const {
    std::string ranks[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    return ranks[index];
}

