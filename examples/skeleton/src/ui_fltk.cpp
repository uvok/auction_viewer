#include "auction_viewer/ui.h"
#include "auction_viewer/model.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>

using namespace auction_viewer;

class FltkUI : public IUI {
public:
    FltkUI(std::vector<AuctionItem>& items): items_(items) {
        selected_ = 0;
        build();
    }

    bool run(std::vector<AuctionItem>& /*items*/, int& /*selectedIndex*/) override {
        Fl::run();
        return false;
    }

private:
    std::vector<AuctionItem>& items_;
    int selected_;
    Fl_Window *mainWin{nullptr};
    Fl_Multi_Browser *browser{nullptr};
    Fl_Input *priceInput{nullptr};
    Fl_Button *updateBtn{nullptr};
    Fl_Window *presenterWin{nullptr};
    Fl_Box *presenterBox{nullptr};
    Fl_Box *presenterPrice{nullptr};
    Fl_Image *presenterImage{nullptr};

    static void cb_select(Fl_Widget* w, void* data) {
        auto ui = (FltkUI*)data;
        ui->on_select();
    }

    static void cb_update(Fl_Widget* w, void* data) {
        auto ui = (FltkUI*)data;
        ui->on_update();
    }

    void build() {
        mainWin = new Fl_Window(600, 400, "Auction Viewer - FLTK");
        browser = new Fl_Multi_Browser(10, 10, 280, 380);
        for (size_t i = 0; i < items_.size(); ++i) {
            browser->add(items_[i].title.c_str());
        }
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

        update_presenter();
    }

    void on_select() {
        int idx = browser->value() - 1;
        if (idx >= 0 && idx < (int)items_.size()) {
            selected_ = idx;
            char buf[64];
            snprintf(buf, sizeof(buf), "%.2f", items_[selected_].currentPrice);
            priceInput->value(buf);
            update_presenter();
        }
    }

    void on_update() {
        if (selected_ < 0 || selected_ >= (int)items_.size()) return;
        const char* v = priceInput->value();
        if (!v) return;
        double p = atof(v);
        items_[selected_].currentPrice = p;
        // refresh browser text
        char buf[256];
        snprintf(buf, sizeof(buf), "%s (current: %.2f)", items_[selected_].title.c_str(), items_[selected_].currentPrice);
        browser->replace(selected_ + 1, buf);
        update_presenter();
    }

    void update_presenter() {
        if (selected_ >= 0 && selected_ < (int)items_.size()) {
            std::string title = items_[selected_].title + "\n";
            char pbuf[64];
            snprintf(pbuf, sizeof(pbuf), "Current: %.2f", items_[selected_].currentPrice);
            presenterBox->label(title.c_str());
            presenterPrice->label(pbuf);
            // attempt to load image if path present
            if (!items_[selected_].imagePath.empty()) {
                Fl_Image* img = nullptr;
                // try PNG then JPEG
                img = new Fl_PNG_Image(items_[selected_].imagePath.c_str());
                if (!img || img->w() == 0) {
                    delete img;
                    img = new Fl_JPEG_Image(items_[selected_].imagePath.c_str());
                }
                if (img && img->w() > 0) {
                    if (presenterImage) delete presenterImage;
                    presenterImage = img;
                    presenterBox->image(presenterImage);
                }
            }
        }
    }
};

std::unique_ptr<IUI> make_fltk_ui() {
    // NOTE: FltkUI expects a reference to the items vector at construction time.
    // We'll create a wrapper that constructs the UI when App calls run.
    class FactoryUI : public IUI {
    public:
        FactoryUI() : impl_(nullptr) {}
        bool run(std::vector<AuctionItem>& items, int& selectedIndex) override {
            if (!impl_) impl_ = new FltkUI(items);
            // when impl_->run() returns, exit app
            return impl_->run(items, selectedIndex);
        }
    private:
        FltkUI* impl_;
    };
    return std::make_unique<FactoryUI>();
}
