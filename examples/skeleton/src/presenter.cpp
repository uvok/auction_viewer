#include "auction_viewer/presenter.h"
#include <sstream>
#include <iomanip>

namespace auction_viewer {

Presenter::Presenter() = default;

void Presenter::seedDummy() {
    items_.clear();
    items_.push_back({"Fancy Vase", 10.0, 10.0, "", ""});
    items_.push_back({"Antique Clock", 50.0, 50.0, "", ""});
    selected_ = 0;
    notifyItemsChanged();
}

const AuctionItem* Presenter::getCurrentItem() const {
    if (selected_ >= 0 && selected_ < (int)items_.size()) return &items_[selected_];
    return nullptr;
}

const AuctionItem* Presenter::getItem(int idx) const {
    if (idx >= 0 && idx < (int)items_.size()) return &items_[idx];
    return nullptr;
}

std::string Presenter::getItemDisplayName(int idx) const {
    const AuctionItem* item = getItem(idx);
    if (!item) return "";
    return item->title;
}

std::string Presenter::getItemCurrentPriceDisplay() const {
    const AuctionItem* item = getCurrentItem();
    if (!item) return "N/A";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << item->currentPrice;
    return oss.str();
}

void Presenter::selectItem(int idx) {
    if (idx >= 0 && idx < (int)items_.size()) {
        selected_ = idx;
        notifySelectionChanged();
    }
}

void Presenter::updateCurrentPrice(double price) {
    if (selected_ >= 0 && selected_ < (int)items_.size()) {
        items_[selected_].currentPrice = price;
        notifyPriceChanged();
    }
}

} // namespace auction_viewer
