#pragma once

#include "auction_viewer/model.h"
#include "auction_viewer/ui.h"
#include <vector>
#include <memory>

namespace auction_viewer {

class App {
public:
    explicit App(std::unique_ptr<IUI> ui);
    int run();

private:
    std::vector<AuctionItem> items_;
    std::unique_ptr<IUI> ui_;
    void seedDummy();
};

} // namespace auction_viewer
