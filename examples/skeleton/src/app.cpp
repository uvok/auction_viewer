#include "auction_viewer/app.h"
#include <iostream>

using namespace auction_viewer;

App::App(std::unique_ptr<IUI> ui) : ui_(std::move(ui)) {
}

void App::seedDummy() {
    items_.push_back({"Fancy Vase", 10.0, 10.0, "", ""});
    items_.push_back({"Antique Clock", 50.0, 50.0, "", ""});
}

int App::run() {
    seedDummy();
    int selected = 0;
    bool cont = true;
    while (cont) {
        cont = ui_->run(items_, selected);
    }
    std::cout << "Exiting application\n";
    return 0;
}
