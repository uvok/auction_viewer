#pragma once

#include "auction_viewer/model.h"
#include <vector>
#include <functional>
#include <string>

namespace auction_viewer {

class Presenter {
public:
    using ItemsChangedCallback = std::function<void()>;
    using SelectionChangedCallback = std::function<void(int)>;
    using PriceChangedCallback = std::function<void(int, double)>;

    Presenter();
    void seedDummy();

    // Read-only access to model data
    size_t getItemCount() const { return items_.size(); }
    int getSelectedIndex() const { return selected_; }
    const AuctionItem* getCurrentItem() const;
    const AuctionItem* getItem(int idx) const;

    // Display-formatted data
    std::string getItemDisplayName(int idx) const;
    std::string getItemCurrentPriceDisplay() const;

    // User actions (from View)
    void selectItem(int idx);
    void updateCurrentPrice(double price);

    // Observer callbacks (View registers these)
    void setOnItemsChanged(ItemsChangedCallback cb) { on_items_changed_ = std::move(cb); }
    void setOnSelectionChanged(SelectionChangedCallback cb) { on_selection_changed_ = std::move(cb); }
    void setOnPriceChanged(PriceChangedCallback cb) { on_price_changed_ = std::move(cb); }

private:
    std::vector<AuctionItem> items_;
    int selected_ = 0;
    ItemsChangedCallback on_items_changed_;
    SelectionChangedCallback on_selection_changed_;
    PriceChangedCallback on_price_changed_;

    void notifyItemsChanged() { if (on_items_changed_) on_items_changed_(); }
    void notifySelectionChanged() { if (on_selection_changed_) on_selection_changed_(selected_); }
    void notifyPriceChanged() { if (on_price_changed_) on_price_changed_(selected_, getCurrentItem() ? getCurrentItem()->currentPrice : 0.0); }
};

} // namespace auction_viewer
