#include "Controller.h"
#include "Model.h"

Controller::Controller(Model* model) : model_(model) {

}

Controller::~Controller() {

}

// When AI level button is clicked, change the difficulty of AI
void Controller::aiLevelBtnClicked() {
    model_->toggleAILevel();
}

// When Start button is clicked initailize game
void Controller::startBtnClicked(int seedVal) {
    model_->initializeNewGame(seedVal);
}

// When Player button is clicked toggles between human and computer type
void Controller::playerBtnClicked(int index) {
    model_->togglePlayerType(index);
}

// When Rage button is clicked makes human player a computer player
void Controller::rageQuitBtnClicked(int index) {
    model_->rageQuit(index);
}

// When Quit button is clicked resets game to pre-game state
void Controller::quitBtnClicked() {
    model_->reset();
}

// When Card buttons are clicked plays the card
void Controller::cardBtnClicked(int index) {
    model_->letHumanPlay(index);
}

