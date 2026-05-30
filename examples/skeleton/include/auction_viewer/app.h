#pragma once

#include "auction_viewer/model.h"
#include "auction_viewer/ui.h"
#include "auction_viewer/presenter.h"
#include <vector>
#include <memory>

namespace auction_viewer {

class App {
public:
    explicit App(std::unique_ptr<IUI> ui);
    int run();

private:
    Presenter presenter_;
    std::unique_ptr<IUI> ui_;
};

} // namespace auction_viewer
