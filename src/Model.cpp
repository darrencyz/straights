#include "Model.h"
#include <iostream>

// Constructor
Model::Model() 
    :   aiLevel_(0),
        currentPlayer_(0), 
        cardCount_(0), 
        firstPlay_(false), 
        gameBoard_(new Gameboards()),
        deck_(new Deck(0)) {

    // Initialize all players as computers
    for (int i = 0; i < NUM_PLAYER; i++) {
        playerArray_[i] = new Computer(i + 1);
    }
}

// Destructor
Model::~Model() {

    for (int i = 0; i < NUM_PLAYER; i++) {
        delete playerArray_[i];
    }

    delete deck_;
    delete gameBoard_;
}

// Return the index of the current player
int Model::getCurrentPlayer() const {
    return currentPlayer_;
}

// Return the pointer to the game board
Gameboards* Model::getGameBoard() const {
    return gameBoard_;
}

// Return true if no one has played yet
bool Model::isFirstPlay() const {
    return firstPlay_;
}

// Return the ai level
int Model::getAILevel() const {
    return aiLevel_;
}

// Initialize a new game and start a new round
void Model::initializeNewGame(int seedVal) {

    // Update a seed value for shuffling
    deck_->setSeedVal(seedVal);

    // Reset old scores and winners
    for (int i = 0; i < NUM_PLAYER; i++) {
        playerArray_[i]->setOldScore(0);
        winners_[i] = 0;
    }

    // Start a new round
    startNextRound();
}

// Start a next round
void Model::startNextRound() {

    // Reset deck and game board
    deck_->shuffle();
    gameBoard_->clearBoard();

    // Reset players
    for (int i = 0; i < NUM_PLAYER; i++) {
        playerArray_[i]->reset();
    }

    // Distribute cards
    deal();
    cardCount_ = 0;

    // Find the first player
    currentPlayer_ = getFirstPlayer();

    // This flag is used to let view know that no one has played yet
    firstPlay_ = true;

    notify();

    firstPlay_ = false;
    letComputersPlay();
}

// Increment the index for the current player
void Model::advancePlayer() {

    if (currentPlayer_ == 3) {
        currentPlayer_ = 0;
    }
    else {
        currentPlayer_++;
    }
}

// Change a human player to a computer player
void Model::rageQuit(int index) {

    if (!(playerArray_[index]->isHuman())) {
        return;
    }

    playerArray_[index]->setType(0);
    letComputersPlay();
}

// Toggle the type of a player
void Model::togglePlayerType(int playerIndex) {

    if (playerArray_[playerIndex]->isHuman()) {
        playerArray_[playerIndex]->setType(0);
    }
    else {
        playerArray_[playerIndex]->setType(1);
    }
}

// Return the index of a player who has Spades of Seven
int Model::getFirstPlayer() const {

    for (int i = 0; i < NUM_PLAYER; i++) {
        if (playerArray_[i]->hasSevenSpades()) {
            return i;
        }
    }

    // This should not happen
    return -1;
}

// Return the player with a given index
Player* Model::getPlayerByIndex(int index) const {

    // Return NULL if the index is not valid
    if (index < 0 || index >= NUM_PLAYER) {
        return NULL;
    }

    return playerArray_[index];
}

// Let computer players play until a human player's turn
void Model::letComputersPlay() {

    while (!(playerArray_[currentPlayer_]->isHuman()) && !isRoundDone() &&!isGameOver()) {

        // Build a legal hand
        Player* computer = playerArray_[currentPlayer_];
        computer->buildLegalHand(gameBoard_->getPlayedCards());

        // Let the computer play
        //
        // EASY: Computer plays or discards the first card
        // HARD: Computer plays a card with the largest rank
        //       or discard a card with the smallest rank

        Card* nextCard = NULL;

        // Easy
        if (aiLevel_ == 0) {
            CardList legalHand = computer->getLegalHand();
            if (!legalHand.empty()) {
                nextCard = legalHand.front(); 
            }
        }
        // Hard
        else {
            nextCard = computer->getLargestLegalHand();
        }

        // If there is a legal card, play it
        if (nextCard != NULL) {
            computer->playCard(nextCard);
            gameBoard_->addCard(nextCard);
        }
        // Otherwise, discard a card
        else {
            // Easy
            if (aiLevel_ == 0) {
                CardList hand = computer->getHand();
                computer->discard(hand.front());
            }
            // Hard
            else {
                computer->discard(computer->getSmallestHand());
            }
        }

        // Increment the index for the current player and the counter
        advancePlayer();
        cardCount_++;
    }

    notify();
}

// Let a human player play with a given card
void Model::letHumanPlay(int nextPlayIndex) {
    
    Player* human = playerArray_[currentPlayer_];

    CardList hand = human->getHand();
    CardListIter it;

    // Retrieve a pointer to the next play
    Card* nextPlay = NULL;
    int handCount = 0;
    for (it = hand.begin(); it != hand.end(); it++) {
        if (handCount == nextPlayIndex) {
            nextPlay = *it;
            break;
        }

        handCount++;
    }

    // Human player's legal hand is already built by now
    // since we build it when the player's hand is drawn in the view
    // @see getLegalCardSet() in this class
    CardList legalHand = human->getLegalHand();

    bool proceedToDiscard = true;

    // Check legal hand if the card is a valid play
    for (it = legalHand.begin(); it != legalHand.end(); it++) {

        // Play the card if it's valid
        if ((**it) == *nextPlay) {
            human->playCard(*it);
            gameBoard_->addCard(*it);
            proceedToDiscard = false;
            break;
        }
    }

    // Otherwise, discard the card
    if (proceedToDiscard) {

        // Player tried to discard but there is legal hand
        if (legalHand.size() > 0) {
            return;
        }

        human->discard(nextPlay);
    }

    // Increment the index for the current player and the counter
    advancePlayer();
    cardCount_++;

    letComputersPlay();
}

// Return true if the round is done
bool Model::isRoundDone() const {
    return !(cardCount_ < CARD_COUNT);
}

// Return true if the game is over
bool Model::isGameOver() const {

    for (int i = 0; i < NUM_PLAYER; i++) {
        if (playerArray_[i]->getOldScore() >= 80) {
            return true;
        }
    }

    return false;
}

// Find winners and return an array of players' id
int* Model::getWinner() {

    // Find the minimum score among the players
    int min = playerArray_[0]->getOldScore();
    for (int i = 1; i < NUM_PLAYER; i++) {
        if (playerArray_[i]->getOldScore() <= min) {
            min = playerArray_[i]->getOldScore();
        }
    }

    // Find the players with the minimum score
    for (int j = 0; j < NUM_PLAYER; j++) {
        if (playerArray_[j]->getOldScore() == min) {
            winners_[j] = playerArray_[j]->getId();
        }
    }

    return winners_;
}

// Update scores of all players
void Model::updateScore() {

    for (int i = 0; i < NUM_PLAYER; i++) {

        int newScore = playerArray_[i]->getOldScore()
                        + playerArray_[i]->getNewScore();
        // Adjust scores
        playerArray_[i]->setOldScore(newScore);
        playerArray_[i]->setNewScore(0);
    }
}

// Reset players, game board and deck
void Model::reset() {
    
    // Reset players
    for (int i = 0; i < NUM_PLAYER; i++) {
        playerArray_[i]->reset();
        playerArray_[i]->setType(0);
        playerArray_[i]->setOldScore(0);
        winners_[i] = 0;
    }

    // Reset AI level
    aiLevel_ = 0;

    // Reset game board
    gameBoard_->clearBoard();
   
    // Re-initialize deck
    delete deck_;
    deck_ = new Deck(0);
}

// Return the set of legal cards
std::set<Card*> Model::getLegalCardSet() const {

    Player* player = playerArray_[currentPlayer_];

    // Build a legal hand
    player->buildLegalHand(gameBoard_->getPlayedCards());
    CardList legalHand = player->getLegalHand();

    // Create a set from the legal hand
    std::set<Card*> legalCards(legalHand.begin(), legalHand.end());
  
    return legalCards;
}

// Toggle the difficulty of AI
void Model::toggleAILevel() {
    
    if (aiLevel_ == 0) {
        aiLevel_ = 1;
    }
    else {
        aiLevel_ = 0;
    }
}

// Hand out cards
void Model::deal() {

    Card** allCards = deck_->getCards();
    
    int cardCounter = 0;
    for (int i = 0; i < NUM_PLAYER; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            playerArray_[i]->addCardToHand(allCards[cardCounter]);
            cardCounter++;
        }
    }
}

