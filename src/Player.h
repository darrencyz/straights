#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <list>
#include <iostream>

#define CARD_COUNT 52

typedef std::list<Card*> CardList;
typedef std::list<Card*>::const_iterator CardListConstIter;
typedef std::list<Card*>::iterator CardListIter;

class Player {
public:
    Player(int newId, int type);    // Constructor
    virtual ~Player();              // Destructor

    // Accessors
    CardList getHand() const;
    CardList getLegalHand() const;
    CardList getDiscarded() const;
    int getOldScore() const;
    int getNewScore() const;
    int getId() const;

    // Mutators
    void setOldScore(int);
    void setNewScore(int);
    void setType(int);

    // Member Functions
    void buildLegalHand(Card**);
    void addCardToHand(Card*);
    bool hasSevenSpades() const;
    void reset();
    void playCard(Card* card);
    void discard(Card* card);
    bool isHuman() const;
    
    // Computer derived class function
    virtual Card* getLargestLegalHand() const = 0;
    virtual Card* getSmallestHand() const = 0;

private:
    Card* removeCardFromHand(Card*);
    void addCardToDiscarded(Card*);
    
    // Data Members
    CardList hand_;
    CardList legalHand_;
    CardList discarded_;
    struct Score;
    Score* score_;
    int id_;
    int type_;
};

// Score declaration
struct Player::Score {
    int oldScore;
    int newScore;
};

#endif

