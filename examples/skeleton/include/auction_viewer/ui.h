#pragma once

#include "auction_viewer/model.h"
#include "auction_viewer/presenter.h"
#include <vector>
#include <memory>

namespace auction_viewer {

class IUI {
public:
    virtual ~IUI() = default;
    // Run UI loop; return true to continue, false to exit.
    virtual bool run(Presenter& presenter) = 0;
};

} // namespace auction_viewer
