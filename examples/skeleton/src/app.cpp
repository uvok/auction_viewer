#include "auction_viewer/app.h"
#include <iostream>

using namespace auction_viewer;

App::App(std::unique_ptr<IUI> ui) : ui_(std::move(ui)) {
}

int App::run() {
    presenter_.seedDummy();
    bool cont = true;
    while (cont) {
        cont = ui_->run(presenter_);
    }
    std::cout << "Exiting application\n";
    return 0;
}
