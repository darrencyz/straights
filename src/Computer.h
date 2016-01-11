#ifndef COMPUTER_H
#define COMPUTER_H

#include "Player.h"

class Computer : public Player {
public:
    Computer(int);  // Constructor
    ~Computer();    // Destructor
    
    // AI feature (play card to maximize gamescore)
    Card* getLargestLegalHand() const;
    Card* getSmallestHand() const;
};

#endif

