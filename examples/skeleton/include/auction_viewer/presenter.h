#pragma once

#include "auction_viewer/model.h"
#include <vector>
#include <functional>

namespace auction_viewer {

class Presenter {
public:
    using ChangeCallback = std::function<void()>;

    Presenter();
    void seedDummy();

    const std::vector<AuctionItem>& items() const { return items_; }
    int selected() const { return selected_; }
    const AuctionItem* currentItem() const;

    void select(int idx);
    void updatePrice(double price);

    void set_on_change(ChangeCallback cb) { on_change_ = std::move(cb); }

private:
    std::vector<AuctionItem> items_;
    int selected_ = 0;
    ChangeCallback on_change_;
    void notify() { if (on_change_) on_change_(); }
};

} // namespace auction_viewer
