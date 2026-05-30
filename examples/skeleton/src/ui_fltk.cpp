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

using namespace auction_viewer;

class FltkUI : public IUI {
public:
    FltkUI(Presenter& presenter): presenter_(presenter) {
        selected_ = presenter_.selected();
        // register callback to refresh view when presenter updates
        presenter_.set_on_change([this]() { this->refresh(); });
        build();
    }

    bool run(Presenter& /*presenter*/) override {
        Fl::run();
        return false;
    }

private:
    Presenter& presenter_;
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
        const auto& items = presenter_.items();
        for (size_t i = 0; i < items.size(); ++i) {
            browser->add(items[i].title.c_str());
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

    void refresh() {
        // repopulate browser and update presenter widgets
        browser->clear();
        const auto& items = presenter_.items();
        for (size_t i = 0; i < items.size(); ++i) browser->add(items[i].title.c_str());
        if (presenter_.selected() >= 0) browser->select(presenter_.selected() + 1);
        update_presenter();
    }

    void on_select() {
        int idx = browser->value() - 1;
        const auto& items = presenter_.items();
        if (idx >= 0 && idx < (int)items.size()) {
            selected_ = idx;
            presenter_.select(idx);
            const auto* it = presenter_.currentItem();
            char buf[64];
            snprintf(buf, sizeof(buf), "%.2f", it ? it->currentPrice : 0.0);
            priceInput->value(buf);
            update_presenter();
        }
    }

    void on_update() {
        if (selected_ < 0) return;
        const char* v = priceInput->value();
        if (!v) return;
        double p = atof(v);
        presenter_.updatePrice(p);
    }

    void update_presenter() {
        const auto* it = presenter_.currentItem();
        if (it) {
            std::string title = it->title + "\n";
            char pbuf[64];
            snprintf(pbuf, sizeof(pbuf), "Current: %.2f", it->currentPrice);
            presenterBox->label(title.c_str());
            presenterPrice->label(pbuf);
            // attempt to load image if path present
            if (!it->imagePath.empty()) {
                Fl_Image* img = nullptr;
                // try PNG then JPEG
                img = new Fl_PNG_Image(it->imagePath.c_str());
                if (!img || img->w() == 0) {
                    delete img;
                    img = new Fl_JPEG_Image(it->imagePath.c_str());
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
        bool run(Presenter& presenter) override {
            if (!impl_) impl_ = new FltkUI(presenter);
            // when impl_->run() returns, exit app
            return impl_->run(presenter);
        }
    private:
        FltkUI* impl_;
    };
    return std::make_unique<FactoryUI>();
}
