#ifndef MODEL_H
#define MODEL_H

#include "Subject.h"
#include "Card.h"
#include "Deck.h"
#include "Gameboards.h"
#include "Computer.h"
#include <set>

#define NUM_PLAYER 4

class Model : public Subject {
public:
    Model();
    ~Model();

    // Accessors
    int getCurrentPlayer() const;
    Gameboards* getGameBoard() const;
    bool isFirstPlay() const;
    int getAILevel() const;
    
    // Public Member Functions
    void initializeNewGame(int);
    void startNextRound();
    void rageQuit(int);
    void togglePlayerType(int);
    int getFirstPlayer() const;
    Player* getPlayerByIndex(int) const;
    void letHumanPlay(int);
    bool isRoundDone() const;
    bool isGameOver() const;
    int* getWinner();
    void updateScore();
    void reset();
    std::set<Card*> getLegalCardSet() const;
    void toggleAILevel();

private:
    // Private Member Functions
    void letComputersPlay();
    void advancePlayer();
    void deal();
    
    // Data Members
    int aiLevel_;
    int currentPlayer_;
    int cardCount_;
    bool firstPlay_;
    Player* playerArray_[NUM_PLAYER];
    int winners_[NUM_PLAYER];
    Gameboards* gameBoard_;
    Deck* deck_;
};

#endif

