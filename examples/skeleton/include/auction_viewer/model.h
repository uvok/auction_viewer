#pragma once

#include <string>

namespace auction_viewer {

struct AuctionItem {
    std::string title;
    double startPrice = 0.0;
    double currentPrice = 0.0;
    std::string winner;
    std::string imagePath;
};

} // namespace auction_viewer
