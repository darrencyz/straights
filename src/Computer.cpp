#include "Computer.h"

// Constructor
//
// @param newId: id for a player
Computer::Computer(int newId) : Player(newId, 0) {

}

// Destructor
Computer::~Computer() {

}

// Computer Derived class easy AI plays first legal card
// Computer Derived class hard AI plays with largest rank and discards card that has smalles rank
// This is since the score is based on the sum of ranks of discarded cards

// Return the legal card that has the largest rank
Card* Computer::getLargestLegalHand() const {
    
    CardList hand = Player::getLegalHand();
    
    // If there is no legal hand, then the computer must discard
    if (hand.empty()) {
        return NULL;
    }
    
    Card* maxCard = hand.front();
    CardListConstIter it;
    
    // Find the legal card with the largest rank
    int max = (int) maxCard->getRank();
    for (it = hand.begin(); it != hand.end(); it++) {
        if ((*it)->getRank() > max) {
            max = (*it)->getRank();
            maxCard = *it;
        }
    }
    
    return maxCard;
}

// Return the card that has the smallest rank
Card* Computer::getSmallestHand() const {
    
    CardList hand = Player::getHand();
    if (hand.empty()) {
        return NULL;
    }
    
    Card* minCard = hand.front();
    CardListConstIter it;
    
    // Find the card with the largest rank
    int min = (int) minCard->getRank();
    for (it = hand.begin(); it != hand.end(); it++) {
        if ((*it)->getRank() < min) {
            min = (*it)->getRank();
            minCard = *it;
        }
    }
    
    return minCard;
}

