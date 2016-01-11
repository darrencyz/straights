#include <gtkmm/main.h>
#include "Model.h"
#include "Controller.h"
#include "View.h"

int main(int argc, char* argv[]) {

    // Initialize gtkmm
    Gtk::Main kit(argc, argv);

    // Create model, controller and view
    Model model;
    Controller controller(&model);
    View view(&controller, &model);

    // Run view
    Gtk::Main::run(view);

    return 0;
}

