#include "Observer.h"
#include "View.h"
#include "Controller.h"
#include "Model.h"
#include <string>

// Constructor
View::View(Controller* c, Model* m) 
    :   isGameStarted_(false), 
        controller_(c), 
        model_(m), 
        cardGui(new CardGui()), 
        tableFrame(new Gtk::Frame("Card on Table")), 
        handFrame(new Gtk::Frame("Player Hand")), 
        cmdFrame(new Gtk::Frame("Game Options - Seed / AI Level")),
        main(new Gtk::HBox()),
        clubBox(new Gtk::HBox()),
        diamondBox(new Gtk::HBox()),
        heartBox(new Gtk::HBox()),
        spadeBox(new Gtk::HBox()),
        hand(new Gtk::HBox()),
        leftV(new Gtk::VBox()),
        rightV(new Gtk::VBox()),
        table(new Gtk::VBox()),
        gameInit(new Gtk::VBox()),
        startBtn(new Gtk::Button("Start Game")), 
        quitBtn(new Gtk::Button("Quit Game")),
        aiLevelBtn(new Gtk::Button("Easy AI")),
        seedInput(new Gtk::Entry()) {

    set_title("Straights");
    set_border_width(10);

    // Initializes an array containing card images
    for(int i = 0; i < NUM_PLAYER; i++){
        for(int j = 0; j < RANK_COUNT; j++){
            card[i][j] = new Gtk::Image(cardGui->getEmptyCardImg());
        }
    }

    // Add 13 ranks of each suit into each HBox t(shape)
    for(int i = 0; i < RANK_COUNT; i++){
        clubBox->pack_start(*card[0][i], Gtk::PACK_EXPAND_WIDGET, 3);
        diamondBox->pack_start(*card[1][i]);
        heartBox->pack_start(*card[2][i]);
        spadeBox->pack_start(*card[3][i]);
    }

    // tableFrame has cards played on table 4 HBox which contain 13 ranks of each suit
    table->pack_start(*clubBox, Gtk::PACK_EXPAND_WIDGET, 4);
    table->pack_start(*diamondBox, Gtk::PACK_EXPAND_WIDGET, 4);
    table->pack_start(*heartBox, Gtk::PACK_EXPAND_WIDGET, 4);
    table->pack_start(*spadeBox, Gtk::PACK_EXPAND_WIDGET, 4);
    tableFrame->add(*table);

    // Initialize player's hand
    for(int i = 0; i < RANK_COUNT; i++){

        // Initialize button images
        buttonImg[i] = new Gtk::Image(cardGui->getEmptyCardImg());

        // Initialize buttons and set images
        cardButton[i] = new Gtk::Button();
        cardButton[i]->set_sensitive(false);
        cardButton[i]->set_image(*buttonImg[i]);

        // Link card buttons to "clicked" events
        (*cardButton[i]).signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &View::cardBtnClicked), i));
 
        // Add each button to hand HBox
        hand->pack_start(*cardButton[i], Gtk::PACK_EXPAND_WIDGET, 1);
    }

    // Frame handFrame has the hand HBox
    handFrame->add(*hand);
    
    // handFrame is then added in the left main VBox
    leftV->pack_start(*tableFrame, Gtk::PACK_EXPAND_WIDGET, 50);
    leftV->pack_start(*handFrame, Gtk::PACK_EXPAND_WIDGET, 5);

    // leftV contains cards on table and player hands
    main->pack_start(*leftV);
    
    // Set seed entry
    seedInput->set_text("0");
    seedInput->set_alignment(Gtk::ALIGN_CENTER);

    // Link buttons in command Frame to "clicked" events
    aiLevelBtn->signal_clicked().connect(sigc::mem_fun(*this, &View::aiLevelBtnClicked));
    startBtn->signal_clicked().connect(sigc::mem_fun(*this, &View::startBtnClicked));
    quitBtn->signal_clicked().connect(sigc::mem_fun(*this, &View::quitBtnClicked));

    // Add above entry and buttons to command frame
    gameInit->pack_start(*seedInput, Gtk::PACK_EXPAND_WIDGET, 1);
    gameInit->pack_start(*aiLevelBtn, Gtk::PACK_EXPAND_WIDGET, 1);
    gameInit->pack_start(*startBtn, Gtk::PACK_EXPAND_WIDGET, 1);
    gameInit->pack_start(*quitBtn, Gtk::PACK_EXPAND_WIDGET, 1);
    cmdFrame->add(*gameInit);

    // Initialize players' components
    for(int i = 0; i < NUM_PLAYER; i++){

        // Initialize frames, buttons and labels
        Glib::ustring label = Glib::ustring::compose("Player %1", i + 1);
        playerFrame[i] = new Gtk::Frame(label);
        playerButton[i] = new Gtk::Button();
        scoreLabel[i] = new Gtk::Label();
        discardLabel[i] = new Gtk::Label();
        playerVBox[i] = new Gtk::VBox();

        // Set player's labels
        playerButton[i]->set_label("Computer");

        // Link player buttons to "clicked" events
        (*playerButton[i]).signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &View::playerBtnClicked), i));

        // Set score and discard labels
        scoreLabel[i]->set_label("Score: 0");
        discardLabel[i]->set_label("Discarded cards: 0");

        // Add players' components into Vbox, and then into player frames
        playerVBox[i]->pack_start(*playerButton[i], Gtk::PACK_EXPAND_WIDGET, 3);
        playerVBox[i]->pack_start(*scoreLabel[i], Gtk::PACK_EXPAND_WIDGET, 3);
        playerVBox[i]->pack_start(*discardLabel[i], Gtk::PACK_EXPAND_WIDGET, 3);
        playerFrame[i]->add(*playerVBox[i]);
    }
   
    // Add command frame and player's frames to right VBox
    rightV->pack_start(*cmdFrame, Gtk::PACK_EXPAND_WIDGET, 3);
    for(int i = 0; i < NUM_PLAYER; i++){
        rightV->pack_start(*playerFrame[i], Gtk::PACK_EXPAND_WIDGET, 3);
    }
    
    // rightV contains player list and game initialization command
    main->pack_start(*rightV);

    // Main HBox which divides program into two sections
    add(*main);

    // Show this window
    show_all();
   
    // Register this view as an observer of model
    model_->subscribe(this);
}

// Destructor
View::~View() { 
   
    // Delete everything that is dynamically created

    for (int i = 0; i < NUM_PLAYER; i++) {
        delete playerFrame[i];
        delete playerButton[i];
        delete playerVBox[i];
        delete scoreLabel[i];
        delete discardLabel[i];
    }

    for (int i = 0; i < RANK_COUNT; i++) {
        delete buttonImg[i];
        delete cardButton[i];
    }

    for (int i = 0; i < NUM_PLAYER; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            delete card[i][j];
        }
    }

    delete cardGui;
    delete tableFrame;
    delete handFrame;
    delete cmdFrame;
    delete main;
    delete clubBox;
    delete diamondBox;
    delete heartBox;
    delete spadeBox;
    delete hand;
    delete leftV;
    delete rightV;
    delete table;
    delete gameInit;
    delete startBtn;
    delete quitBtn;
    delete aiLevelBtn;
}

// Update view
void View::update() {

    // Show played cards
    drawPlayedCards();

    // Show human player's hand
    drawHumanHand();

    // Disable players' buttons on someone else's turn
    toggleHumanPlayerButton();

    // Highlight the frame of the current player
    highlightCurrentPlayer();

    // Update each player's discard label
    updateDiscardLabel();

    // If no one has played yet, show whose turn it is
    if (model_->isFirstPlay()) {
        showFirstPlayer();
        return;
    }

    // After each round, show scores and discarded cards
    if (model_->isRoundDone() && !(model_->isGameOver())) {

		// Show scores in a dialog box first
       	showScores();

        // Draw empty cards before we start the next round
        drawEmptyCards();

        // Update scores and if one of players has more than 80 points,
        // the game is done
        model_->updateScore();
        updateScoreLabel();
       
        // Show winners if the game is done
        if (model_->isGameOver()) {

			// Show winners in a dialog box and reset the game
           	showWinners();
            quitBtnClicked();
        }
        // Otherwise, proceed to the next round
        else {
        	model_->startNextRound();
        }
    }
}

// **************
// Event handlers
// **************

// Handles a click event of an AI level button
void View::aiLevelBtnClicked() {

    controller_->aiLevelBtnClicked();

    // Update the label of AI level button
    if (model_->getAILevel() == 0) {
        aiLevelBtn->set_label("Easy AI");
    }
    else {
        aiLevelBtn->set_label("Hard AI");
    }
}

// Handles a click event of a start game button
void View::startBtnClicked() {

    // This is when a user clicks start button 
    // after the game is already started
    if (isGameStarted_) {
        quitBtnClicked();
    }
    
    // Read seed value from the entry
    std::string seedStr = seedInput->get_text();

    // Convert seed value to an integer
    int seedVal = std::atoi(seedStr.c_str());

    // If invalid seed value is entered by a user, 
    // set the text to what std::atoi returned
    seedInput->set_text(Glib::ustring::compose("%1", seedVal));

    // Once the game starts, human players can invoke rage quit
    // Computer players' buttons get disabled
    for (int i = 0; i < NUM_PLAYER; i++) {

        Player* player = model_->getPlayerByIndex(i);
        if (player->isHuman()) {
            playerButton[i]->set_label("Rage Quit!");
        }
        else {
            playerButton[i]->set_sensitive(false);
        }
    }

    isGameStarted_ = true;
    controller_->startBtnClicked(seedVal);
}

// Handles a click event of a quit button
void View::quitBtnClicked() {

    // Set game started flag to false
    isGameStarted_ = false;

    // Reset labels and frames of players
    for (int i = 0; i < NUM_PLAYER; i++) {
        playerButton[i]->set_label("Computer");
        playerButton[i]->set_sensitive(true);
        playerFrame[i]->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    }

    // Reset AI level button label
    aiLevelBtn->set_label("Easy AI");

    // Resets table and hand frames with empty cards
    // Resets score and discard labels on player frames
    drawEmptyCards();
    resetScore();

    // Quit game and initialize pre-game state
    controller_->quitBtnClicked();
}

// Handles a click event of a player button
void View::playerBtnClicked(int index) {
    
    // If a round is finished, clicking the button invokes rage quit
    if (isGameStarted_) {
        
        // Change the label and disable the button
        playerButton[index]->set_label("Computer");
        playerButton[index]->set_sensitive(false);

        controller_->rageQuitBtnClicked(index);
        return;
    }

    // Otherwise, player button is clicked
    controller_->playerBtnClicked(index);

    // Now that the type is changed, we need to update the label
    Player* player = model_->getPlayerByIndex(index);
    if (player->isHuman()) {
        playerButton[index]->set_label("Human");
    }
    else {
        playerButton[index]->set_label("Computer");
    }
}

// Handles a click event of a card button
void View::cardBtnClicked(int index) {

    // Play the card
    controller_->cardBtnClicked(index);
}

// ****************
// Helper functions
// ****************

// Show images of played cards
void View::drawPlayedCards() {
    
    Gameboards* gameBoard = model_->getGameBoard();
    Card** playedCards = gameBoard->getPlayedCards();

    // Draw played card on the table with correct suit and rank
    for (int i = 0; i < CARD_COUNT; i++) {
        if (playedCards[i] != NULL) {
            Suit suit = playedCards[i]->getSuit();
            Rank rank = playedCards[i]->getRank();

            Glib::RefPtr<Gdk::Pixbuf> pixbuf
                = cardGui->getCardImg(suit, rank);
            card[(int) suit][(int) rank]->set(pixbuf);
        }
    }
}

// Draw empty cards when a player quits the game
void View::drawEmptyCards() {

    // Draw empty cards in the table
    for (int i = 0; i < NUM_PLAYER; i++) {
        for (int j = 0; j < RANK_COUNT; j++) {
            card[i][j]->set(cardGui->getEmptyCardImg());
        }
    }
    
    // Draw empty cards in player's hand and disable buttons
    for (int i = 0; i < RANK_COUNT; i++) {
       buttonImg[i]->set(cardGui->getEmptyCardImg());
       cardButton[i]->set_sensitive(false);
    }
}

// Draw cards in the current player's hand
void View::drawHumanHand() {

    int currentPlayer = model_->getCurrentPlayer();
    Player* human = model_->getPlayerByIndex(currentPlayer);

    // If the player is not a human, do nothing
    if (!(human->isHuman())) {
        return;
    }

    CardList hand = human->getHand();
    CardListIter it;
    std::set<Card*> legalCard = model_->getLegalCardSet();
    int legalHandSize = human->getLegalHand().size();
  
    // Use a suit and rank of each card to get the correct card image,
    // then update a button's image
    int buttonCount = 0;
    for (it = hand.begin(); it != hand.end(); it++) {

        Suit s = (*it)->getSuit();
        Rank r = (*it)->getRank();
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = cardGui->getCardImg(s, r);
        buttonImg[buttonCount]->set(pixbuf);

        bool sensitivity = true;

        // If there is at least one legal card,
        // disable buttons containing a non-legal card image
        if (legalCard.find(*it) == legalCard.end() && legalHandSize > 0) {
            sensitivity = false;
        }

        cardButton[buttonCount]->set_sensitive(sensitivity);

        buttonCount++;
    }

    // If there are less than 12 cards in a hand,
    // the rest of them will be empty cards
    while (buttonCount < RANK_COUNT) {

        buttonImg[buttonCount]->set(cardGui->getEmptyCardImg());
        cardButton[buttonCount]->set_sensitive(false);

        buttonCount++;
    }
}

// Show a dialog box with a given message
void View::showDialog(const Glib::ustring& title, const Glib::ustring& msg) {
    
    // Create a message dialog
    Gtk::MessageDialog msgDialog(msg, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);

    // Set title and show the dialog
    msgDialog.set_title(title);
    msgDialog.run();
}

// Print the first player in a dialog box
void View::showFirstPlayer() {
	
	int firstPlayerId = model_->getFirstPlayer() + 1;
    Glib::ustring title = ":: New Round ::";
    Glib::ustring msg = Glib::ustring::compose(
    						"A new round begins. It's player %1's turn to play", 
    						firstPlayerId
    					);
    
    // Show messages in the dialog box
    showDialog(title, msg);
}

// Print the scores in a dialog box
void View::showScores() {
	
	Glib::ustring title = ":: Score ::";
    Glib::ustring msg = "";

    // Compose messages for each player
    for (int i = 0; i < NUM_PLAYER; i++) {

    	Player* player = model_->getPlayerByIndex(i);
        int id = player->getId();
        int oldScore = player->getOldScore();
        int newScore = player->getNewScore();

        // Show discarded cards of each player
        msg+= Glib::ustring::compose("Player %1's discards:", id);
    	CardList discarded = player->getDiscarded();
        CardListIter it;

        for (it = discarded.begin(); it != discarded.end(); it++) {
        	if ((*it) != NULL) {
            	// We use ostream operator to store stringstream of each card and
            	// add its string representation to the message
 				std::stringstream cardBuffer;
                cardBuffer << **it;
                msg += " " + cardBuffer.str();
            }
        }

        msg += Glib::ustring::compose(
            		"\nPlayer %1's score: %2 + %3 = %4\n", 
            		id, 
            		oldScore, 
            		newScore, 
            		oldScore + newScore
            	);
    }

    // Show messages in the dialog box
    showDialog(title, msg);
}

// Print the winners in a dialog box
void View::showWinners() {
	
	Glib::ustring title = ":: Winner ::";
    Glib::ustring msg = "";

    // Compose messages
    int* winners = model_->getWinner();
    for (int i = 0; i < NUM_PLAYER; i++) {
    	if (winners[i] != 0) {
			msg += Glib::ustring::compose("Player %1 wins!\n", winners[i]);
		}
	}

    // Show messages in the dialog box
    showDialog(title, msg);
}

//Resets score and discard labels to 0 on player Frames
void View::resetScore(){

    for(int i = 0; i < NUM_PLAYER; i++){
        scoreLabel[i]->set_label("Score: 0");
        discardLabel[i]->set_label("Discarded cards: 0");
    }
}

// Toggle sensitivity of a human player's button
void View::toggleHumanPlayerButton() {

    int currentPlayer = model_->getCurrentPlayer();
    for (int i = 0; i < NUM_PLAYER; i++) {

        // Enable the current human player's button
        if (model_->getPlayerByIndex(i)->isHuman()) {
            if (currentPlayer == i) {
                playerButton[i]->set_sensitive(true);
            }
            else {
                playerButton[i]->set_sensitive(false);
            }
        }   
    }
}

// Highlight the current player's frame
void View::highlightCurrentPlayer() {

    int currentPlayer = model_->getCurrentPlayer();
    for (int i = 0; i < NUM_PLAYER; i++) {

        if (currentPlayer == i) {
            playerFrame[i]->set_shadow_type(Gtk::SHADOW_IN);
        }
        else {
            playerFrame[i]->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        }
    }
}

// Update score label
void View::updateScoreLabel() {

    // Fetch each player's score and update the label
    for (int i = 0; i < NUM_PLAYER; i++) {

        Player* player = model_->getPlayerByIndex(i);
        Glib::ustring score = 
            Glib::ustring::compose("Score: %1", player->getOldScore());
        scoreLabel[i]->set_label(score);
    }
}

// Update discard label
void View::updateDiscardLabel() {

    // Fetch each player's discarded list and update the label
    for (int i = 0; i < NUM_PLAYER; i++) {

        Player* player = model_->getPlayerByIndex(i);
        Glib::ustring discard = 
            Glib::ustring::compose("Discarded cards: %1", player->getDiscarded().size());
        discardLabel[i]->set_label(discard);
    }
}

