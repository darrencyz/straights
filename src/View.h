#ifndef VIEW_H
#define VIEW_H

#include <gtkmm.h>
#include "Observer.h"
#include "CardGui.h"
#define NUM_PLAYER 4

class Controller;
class Model;

class View : public Gtk::Window, public Observer {
public:
    // Constructor
    View(Controller*, Model*);
    // Destructor
    virtual ~View();
    // Update Function
    virtual void update();

private:
    bool isGameStarted_;

    // Model and Controller
    Controller* controller_;
    Model* model_;

    // cardGui holds pixbufs for card images
    CardGui* cardGui;

    // 4 main frames used in view
    Gtk::Frame *tableFrame, *handFrame, *cmdFrame;

    // HBoxes are used to align table, played cards and a player's hand
    Gtk::HBox *main, *clubBox, *diamondBox, *heartBox, *spadeBox, *hand;

    // VBoxes are used to align components of two main vertical sections, table, players, GameStart
    Gtk::VBox *leftV, *rightV, *table, *gameInit;

    // Images used in table and hand (buttons)
    Gtk::Image* card[NUM_PLAYER][RANK_COUNT];
    Gtk::Image* buttonImg[RANK_COUNT];

    // Buttons for operating a player's hand commands
    Gtk::Button* cardButton[RANK_COUNT];

    // Buttons for game options
    Gtk::Button *startBtn, *quitBtn, *aiLevelBtn;

    // Entry for seed value
    Gtk::Entry* seedInput;
    
    // Player components
    Gtk::Frame* playerFrame[NUM_PLAYER];
    Gtk::Button* playerButton[NUM_PLAYER];
    Gtk::VBox* playerVBox[NUM_PLAYER];
    Gtk::Label* scoreLabel[NUM_PLAYER];
    Gtk::Label* discardLabel[NUM_PLAYER];

    // Event handlers
    void aiLevelBtnClicked();
    void startBtnClicked();
    void quitBtnClicked();
    void playerBtnClicked(int);
    void cardBtnClicked(int);

    // Helper functions
    void drawPlayedCards();
    void drawEmptyCards();
    void drawHumanHand();
    void showDialog(const Glib::ustring&, const Glib::ustring&);
    void showFirstPlayer();
    void showScores();
    void showWinners();
    void resetScore();
    void toggleHumanPlayerButton();
    void highlightCurrentPlayer();
    void updateScoreLabel();
    void updateDiscardLabel();
};

#endif

