#include "auction_viewer/ui.h"
#include "auction_viewer/model.h"
#include "auction_viewer/presenter.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <memory>
#include <string>

using namespace auction_viewer;

class FltkUI : public IUI {
public:
    explicit FltkUI(Presenter& presenter) : presenter_(presenter) {
        // Register callbacks with presenter to receive updates
        presenter_.setOnItemsChanged([this]() { this->refreshItems(); });
        presenter_.setOnSelectionChanged([this](int idx) { this->refreshSelection(); });
        presenter_.setOnPriceChanged([this](int idx, double price) { this->refreshPresenter(); });
        build();
    }

    bool run(Presenter& /*presenter*/) override {
        Fl::run();
        return false;
    }

private:
    Presenter& presenter_;
    Fl_Window *mainWin = nullptr;
    Fl_Multi_Browser *browser = nullptr;
    Fl_Input *priceInput = nullptr;
    Fl_Button *updateBtn = nullptr;
    Fl_Window *presenterWin = nullptr;
    Fl_Box *presenterBox = nullptr;
    Fl_Box *presenterPrice = nullptr;
    Fl_Image *presenterImage = nullptr;

    // Storage for strings to ensure they persist for widget labels
    std::string title_buffer_;
    std::string price_buffer_;

    static void cb_select(Fl_Widget* /*w*/, void* data) {
        auto ui = static_cast<FltkUI*>(data);
        ui->onSelect();
    }

    static void cb_update(Fl_Widget* /*w*/, void* data) {
        auto ui = static_cast<FltkUI*>(data);
        ui->onUpdate();
    }

    void build() {
        mainWin = new Fl_Window(600, 400, "Auction Viewer");
        browser = new Fl_Multi_Browser(10, 10, 280, 380);
        browser->callback(cb_select, this);

        priceInput = new Fl_Input(300, 30, 280, 25, "Price:");
        updateBtn = new Fl_Button(300, 70, 100, 30, "Update");
        updateBtn->callback(cb_update, this);

        mainWin->end();
        mainWin->show();

        presenterWin = new Fl_Window(800, 600, "Presenter");
        presenterBox = new Fl_Box(10, 10, 780, 520, "");
        presenterBox->labelfont(FL_HELVETICA_BOLD);
        presenterBox->labelsize(36);
        presenterPrice = new Fl_Box(10, 540, 780, 50, "");
        presenterPrice->labelfont(FL_HELVETICA_BOLD);
        presenterPrice->labelsize(24);
        presenterWin->end();

        refreshItems();
    }

    void refreshItems() {
        // Repopulate browser with all items
        browser->clear();
        for (size_t i = 0; i < presenter_.getItemCount(); ++i) {
            const std::string& name = presenter_.getItemDisplayName(i);
            browser->add(name.c_str());
        }
        // Re-select current item
        if (presenter_.getSelectedIndex() >= 0) {
            browser->select(presenter_.getSelectedIndex() + 1);
        }
        refreshPresenter();
    }

    void refreshSelection() {
        int idx = presenter_.getSelectedIndex();
        if (idx >= 0) {
            browser->select(idx + 1);
        }
        // Update price input field
        const auto* item = presenter_.getCurrentItem();
        if (item) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%.2f", item->currentPrice);
            priceInput->value(buf);
        }
        refreshPresenter();
    }

    void refreshPresenter() {
        const auto* item = presenter_.getCurrentItem();
        if (item) {
            // Store title in buffer to ensure it persists
            title_buffer_ = item->title;
            presenterBox->label(title_buffer_.c_str());

            // Store price string in buffer
            char pbuf[64];
            snprintf(pbuf, sizeof(pbuf), "Current: %.2f", item->currentPrice);
            price_buffer_ = pbuf;
            presenterPrice->label(price_buffer_.c_str());

            // Load and display image if path is provided
            if (!item->imagePath.empty()) {
                Fl_Image* img = nullptr;
                // Try PNG first, then JPEG
                img = new Fl_PNG_Image(item->imagePath.c_str());
                if (!img || img->w() == 0) {
                    delete img;
                    img = new Fl_JPEG_Image(item->imagePath.c_str());
                }
                if (img && img->w() > 0) {
                    if (presenterImage) delete presenterImage;
                    presenterImage = img;
                    presenterBox->image(presenterImage);
                }
            }
        }
    }

    void onSelect() {
        int idx = browser->value() - 1;
        if (idx >= 0 && idx < (int)presenter_.getItemCount()) {
            presenter_.selectItem(idx);
        }
    }

    void onUpdate() {
        const char* v = priceInput->value();
        if (v) {
            double p = atof(v);
            presenter_.updateCurrentPrice(p);
        }
    }
};

std::unique_ptr<IUI> make_fltk_ui() {
    // Defer UI construction until run() is called, so presenter exists
    class FactoryUI : public IUI {
    public:
        FactoryUI() = default;
        bool run(Presenter& presenter) override {
            if (!impl_) {
                impl_ = std::make_unique<FltkUI>(presenter);
            }
            return impl_->run(presenter);
        }
    private:
        std::unique_ptr<FltkUI> impl_;
    };
    return std::make_unique<FactoryUI>();
}
