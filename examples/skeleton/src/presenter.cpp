#include "auction_viewer/presenter.h"

namespace auction_viewer {

Presenter::Presenter() = default;

void Presenter::seedDummy() {
    items_.clear();
    items_.push_back({"Fancy Vase", 10.0, 10.0, "", ""});
    items_.push_back({"Antique Clock", 50.0, 50.0, "", ""});
    notify();
}

const AuctionItem* Presenter::currentItem() const {
    if (selected_ >= 0 && selected_ < (int)items_.size()) return &items_[selected_];
    return nullptr;
}

void Presenter::select(int idx) {
    if (idx >= 0 && idx < (int)items_.size()) {
        selected_ = idx;
        notify();
    }
}

void Presenter::updatePrice(double price) {
    if (selected_ >= 0 && selected_ < (int)items_.size()) {
        items_[selected_].currentPrice = price;
        notify();
    }
}

} // namespace auction_viewer
