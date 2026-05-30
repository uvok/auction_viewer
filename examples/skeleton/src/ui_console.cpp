#include "auction_viewer/ui.h"
#include <iostream>
#include <iomanip>

using namespace auction_viewer;

static void printItems(const Presenter& presenter) {
    const auto& items = presenter.items();
    int selected = presenter.selected();
    std::cout << "\nAuction Items:\n";
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& it = items[i];
        std::cout << (i == (size_t)selected ? "> " : "  ")
                  << i << ": " << it.title
                  << " (current: " << std::fixed << std::setprecision(2) << it.currentPrice << ")\n";
    }
}

class ConsoleUI : public IUI {
public:
    bool run(Presenter& presenter) override {
        printItems(presenter);
        std::cout << "Commands: [s]elect [u]pdate [q]uit > ";
        char cmd = 0;
        if (!(std::cin >> cmd)) return false;
        if (cmd == 'q') return false;
        if (cmd == 's') {
            int idx = 0;
            std::cout << "Index: ";
            if (std::cin >> idx) presenter.select(idx);
        } else if (cmd == 'u') {
            double p = 0.0;
            std::cout << "New price: ";
            if (std::cin >> p) {
                presenter.updatePrice(p);
                std::cout << "Updated.\n";
            }
        }
        return true;
    }
};

// Factory function used by main
std::unique_ptr<IUI> make_console_ui() {
    return std::make_unique<ConsoleUI>();
}
