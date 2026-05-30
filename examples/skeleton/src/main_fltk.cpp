#include "auction_viewer/app.h"

// Forward-declare factory (defined in ui_fltk.cpp)
std::unique_ptr<auction_viewer::IUI> make_fltk_ui();

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    auto ui = make_fltk_ui();
    auction_viewer::App app(std::move(ui));
    return app.run();
}
