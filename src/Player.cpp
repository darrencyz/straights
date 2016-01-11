#include "Player.h"
#include <map>

// Constructor
//
// @param   newId: id of a player
//          type: type of a player
Player::Player(int newId, int type) : score_(new Player::Score), id_(newId), type_(type) {

    // Initialize scores
    score_->oldScore = 0;
    score_->newScore = 0;
}

// Destructor
Player::~Player() {
    delete score_;
}

//-------------------Accessors-------------------//
// Accessor for hand_
CardList Player::getHand() const {
    return hand_;
}

// Accessor for legalHand_
CardList Player::getLegalHand() const {
    return legalHand_;
}

// Accessor for discarded_
CardList Player::getDiscarded() const {
    return discarded_;
}

// Accessor for oldScore
int Player::getOldScore() const {
    return score_->oldScore;
}

// Accessor for newScore
int Player::getNewScore() const {
    return score_->newScore;
}

// Accessor for id_
int Player::getId() const {
    return id_;
}
//------------------------------------------------//

//-------------------Mutators-------------------//
// Mutator for oldScore
void Player::setOldScore(int score) {
    score_->oldScore = score;
}

// Mutator for newScore
void Player::setNewScore(int score) {
    score_->newScore = score;
}

//Mutator for type
void Player::setType(int type) {
    type_ = type;
}
//------------------------------------------------//

// Build a legal hand based on the list of already played cards
void Player::buildLegalHand(Card** playedCards) {

    // Clear the existing legal hand
    legalHand_.clear();

    CardListConstIter it;

    // If there is no played card, 7S is the only legal play
    if (playedCards[0] == NULL) {

        for (it = hand_.begin(); it != hand_.end(); it++) {

            if ((*it)->getRank() == SEVEN && (*it)->getSuit() == SPADE) {
                legalHand_.push_back(*it);
                return;
            }
        }
        throw "No last played card and no 7S";
    }

    // Map is temporarily used to prevent from adding duplicate cards
    std::map<Card*, int> legalCardMap;

    // Go through each card in hand
    for (it = hand_.begin(); it != hand_.end(); it++) {

        // 7 of any suit is a legal play
        if ((*it)->getRank() == SEVEN) {
            
            if (legalCardMap.find(*it) == legalCardMap.end()) {

                legalHand_.push_back(*it);
                legalCardMap[*it] = 1;
            }
            continue;
        }

        for (int i = 0; i < CARD_COUNT; i++) {

            if (playedCards[i] == NULL) {
                break;
            }

            // A card with the same suit and adjacent rank as another
            // card that has already been played is a legal play
            else if ((*it)->getSuit() == playedCards[i]->getSuit()) {

                Rank rank = (*it)->getRank(); 
                Card* legalCard = NULL;

                // Compare cards to find whether ranks are adjacent or not
                switch (playedCards[i]->getRank()) {
                case ACE:
                    if (rank == TWO)
                        legalCard = *it;
                    break;
                case TWO:
                    if (rank == ACE || rank == THREE)
                        legalCard = *it;
                    break;
                case THREE:
                    if (rank == TWO || rank == FOUR)
                        legalCard = *it;
                    break;
                case FOUR:
                    if (rank == THREE || rank == FIVE)
                        legalCard = *it;
                    break;
                case FIVE:
                    if (rank == FOUR || rank == SIX)
                        legalCard = *it;
                    break;
                case SIX:
                    if (rank == FIVE || rank == SEVEN)
                        legalCard = *it;
                    break;
                case SEVEN:
                    if (rank == SIX || rank == EIGHT)
                        legalCard = *it;
                    break;
                case EIGHT:
                    if (rank == SEVEN || rank == NINE)
                        legalCard = *it;
                    break;
                case NINE:
                    if (rank == EIGHT || rank == TEN)
                        legalCard = *it;
                    break;
                case TEN:
                    if (rank == NINE || rank == JACK)
                        legalCard = *it;
                    break;
                case JACK:
                    if (rank == TEN || rank == QUEEN)
                        legalCard = *it;
                    break;
                case QUEEN:
                    if (rank == JACK || rank == KING)
                        legalCard = *it;
                    break;
                case KING:
                    if (rank == QUEEN)
                        legalCard = *it;
                    break;
                default:
                    throw "Invalid rank";
                }

                // If the legal play is not already added, add it to the list
                if (legalCard != NULL) {
                    if (legalCardMap.find(legalCard) == legalCardMap.end()) {
                        legalHand_.push_back(*it);
                        legalCardMap[legalCard] = 1;
                    }
                }
            }   
        }
    }

    // We don't need to keep cards in the temporary map
    legalCardMap.clear();
}

// Add card to the hand
//
// @param card: a card to be added
void Player::addCardToHand(Card* card) {
    hand_.push_back(card);
}

// Remove card from the hand
//
// @param card: a card to be removed
Card* Player::removeCardFromHand(Card* card) {

    CardListIter it;
    for (CardListIter it = hand_.begin(); it != hand_.end(); it++) {
        if (**it == *card) {
            hand_.erase(it);
            return *it;
        }
    }

    return NULL;
}

// Add card to the discarded
//
// @param card: a card to be discarded
void Player::addCardToDiscarded(Card* card) {
    discarded_.push_back(card);
}

// Return true if the hand has 7 of spades
bool Player::hasSevenSpades() const {

    for (CardListConstIter it = hand_.begin(); it != hand_.end(); it++) {
        if ((*it)->getSuit() == SPADE && (*it)->getRank() == SEVEN) {
            return true;
        }
    }

    return false;
}

// Reset a player
void Player::reset() {

    // Clear every list of cards and set the new score to 0
    hand_.clear();
    legalHand_.clear();
    discarded_.clear();
    score_->newScore = 0;

}

// Remove a card from the hand and update last played card
void Player::playCard(Card* card) {
    removeCardFromHand(card);
}

// Remove a card from the hand and add it to the discarded card list
void Player::discard(Card* card) {
    
    Card* removed = removeCardFromHand(card);
    addCardToDiscarded(card);
    
    // Adjust the score
    int newScore = getNewScore() + removed->getRank() + 1;
    setNewScore(newScore);
}

// Return true if the player is human
bool Player::isHuman() const {
    return type_;
}

