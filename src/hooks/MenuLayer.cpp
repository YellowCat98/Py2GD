#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../Py2GD.hpp"

namespace py = pybind11;


class $modify(MenuLayerHook, MenuLayer) {
    bool init() {
        if (py::globals().contains("MenuLayer_init")) {
            py::object MenuLayer_init = py::globals()["MenuLayer_init"];
            return MenuLayer_init(this).cast<bool>();
        } else {
            return MenuLayer::init();
        }
    }
    void onMoreGames(CCObject* sender) {
        if (py::globals().contains("MenuLayer_onMoreGames")) {
            py::object MenuLayer_onMoreGames = py::globals()["MenuLayer_onMoreGames"];
            return MenuLayer_onMoreGames(this, sender).cast<void>();
        } else {
            return MenuLayer::onMoreGames(sender);
        }
    }
};