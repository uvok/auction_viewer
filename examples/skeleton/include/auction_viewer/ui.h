#pragma once

#include "auction_viewer/model.h"
#include <vector>

namespace auction_viewer {

class IUI {
public:
    virtual ~IUI() = default;
    // Run one iteration; return true to continue, false to exit.
    virtual bool run(std::vector<AuctionItem>& items, int& selectedIndex) = 0;
};

} // namespace auction_viewer
