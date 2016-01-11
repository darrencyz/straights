#include "Subject.h"
#include "Observer.h"

void Subject::subscribe(Observer* obs) {
    observers_.insert(obs);
}

void Subject::unsubscribe(Observer* obs) {
    observers_.erase(obs);
}

void Subject::notify() {
    Observers::iterator it;
    for (it = observers_.begin(); it != observers_.end(); it++) {
        (*it)->update();
    }
}

