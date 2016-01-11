#ifndef CONTROLLER_H
#define CONTROLLER_H

class Model;

class Controller {
public:
    // Constructor
    Controller(Model*);
    // Destructor
    ~Controller();
    
    // Public Functions
    void aiLevelBtnClicked();
    void startBtnClicked(int);
    void playerBtnClicked(int);
    void rageQuitBtnClicked(int);
    void quitBtnClicked();
    void cardBtnClicked(int);
    
private:
    // Connected to Model
    Model* model_;
};

#endif

