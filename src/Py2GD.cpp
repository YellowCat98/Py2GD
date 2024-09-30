#include <Geode/Geode.hpp>
#include "Py2GD.hpp"

using namespace geode::prelude;
namespace py = pybind11;

pybind11::scoped_interpreter* Py2GD::interpreter = nullptr;
pybind11::module Py2GD::m;
std::string Py2GD::pyScript;

bool Py2GD::initInterpreterAndBindings() {
	if (!interpreter) {
		interpreter = new py::scoped_interpreter();
		m = py::module::import("__main__");

        bindEnums();
        bindCocos();
        bindGeode();
        bindRobTop();

        pyScript = R"(
def MenuLayer_init(this):
    if not this.init():
        return False
    
    sprite = CCSprite.createWithSpriteFrameName("GJ_arrow_01_001.png")
    this.addChild(sprite)

    return True

def MenuLayer_onMoreGames(this, sender):
    info("Hook!")
    this.onMoreGames(sender) # call original, most likely causes recursion.
    info("After original!")
)";

        execPyWithTryCatch(pyScript.c_str());
		return true;
	}
	return false;
}

// terminate the interpreter
void Py2GD::terminateInterpreter() {
	delete interpreter;
	interpreter = nullptr;
}

// bind robtop stuff
void Py2GD::bindRobTop() {
	py::class_<FLAlertLayer>(m, "FLAlertLayer")
		.def_static("create",
			static_cast<FLAlertLayer* (*)(char const*, const gd::string&, char const*)>(&FLAlertLayer::create), "creates it")
		.def("show", &FLAlertLayer::show);
    
    py::class_<MenuLayer, CCLayer>(m, "MenuLayer")
        .def("init", &MenuLayer::init)
        .def("onMoreGames", &MenuLayer::onMoreGames);

    py::class_<GameManager>(m, "GameManager")
        .def_static("get", &GameManager::get, py::return_value_policy::reference)
        .def_readwrite("m_inMenuLayer", &GameManager::m_inMenuLayer)
        .def_readwrite("m_menuLayer", &GameManager::m_menuLayer);
}

// bind enums, includes all enums in geode, gd, cocos and tulip
void Py2GD::bindEnums() {
	py::enum_<NotificationIcon>(m, "NotificationIcon")
        .defineEnumVal(None, NotificationIcon)
        .defineEnumVal(Loading, NotificationIcon)
        .defineEnumVal(Success, NotificationIcon)
        .defineEnumVal(Warning, NotificationIcon)
        .defineEnumVal(Error, NotificationIcon)
        .defineEnumVal(Info, NotificationIcon);
    
    py::enum_<tulip::hook::TulipConvention>(m, "TulipConvention")
        .defineEnumVal(Default, tulip::hook::TulipConvention)
        .defineEnumVal(Cdecl, tulip::hook::TulipConvention)
        .defineEnumVal(Thiscall, tulip::hook::TulipConvention)
        .defineEnumVal(Fastcall, tulip::hook::TulipConvention)
        .defineEnumVal(Optcall, tulip::hook::TulipConvention)
        .defineEnumVal(Membercall, tulip::hook::TulipConvention)
        .defineEnumVal(Stdcall, tulip::hook::TulipConvention);
}

// bind cocos functions, includes robtop's additions
void Py2GD::bindCocos() {
    py::class_<CCObject>(m, "CCObject")
        .def("release", &CCObject::release)
        .def("retain", &CCObject::retain)
        .def("autorelease", &CCObject::autorelease)
        .def("copy", &CCObject::copy);

	py::class_<CCNode>(m, "CCNode")
		.def_static("create", &CCNode::create, py::return_value_policy::reference)
		.def("addChild", (void (CCNode::*)(CCNode*)) &CCNode::addChild, py::arg("child"))
        .def("addChild", (void (CCNode::*)(CCNode*, int)) &CCNode::addChild, py::arg("child"), py::arg("zOrder"))
        .def("addChild", (void (CCNode::*)(CCNode*, int, int)) &CCNode::addChild, py::arg("child"), py::arg("zOrder"), py::arg("tag"))
        .def("getChildByTag", &CCNode::getChildByTag, py::arg("tag"), py::return_value_policy::reference)
        .def("getID", &CCNode::getID)
        .def("setID", &CCNode::setID, py::arg("id"))
        .def("getChildByID", &CCNode::getChildByID, py::arg("id"), py::return_value_policy::reference)
        .def("setPosition", py::overload_cast<CCPoint const&>(&CCNode::setPosition), py::arg("point"))
        .def("getPosition", py::overload_cast<>(&CCNode::getPosition));
    
    py::class_<ccColor3B>(m, "ccColor3B")
        .def_readwrite("r", &ccColor3B::r)
        .def_readwrite("g", &ccColor3B::g)
        .def_readwrite("b", &ccColor3B::b);
    
    py::class_<ccColor4B>(m, "ccColor4B")
        .def_readwrite("r", &ccColor4B::r)
        .def_readwrite("g", &ccColor4B::g)
        .def_readwrite("b", &ccColor4B::b)
        .def_readwrite("a", &ccColor4B::a);


    py::class_<CCNodeRGBA, CCNode>(m, "CCNodeRGBA")
            .def_static("create", &CCNodeRGBA::create, py::return_value_policy::reference)
            .def("getColor", &CCNodeRGBA::getColor)
            .def("setColor", &CCNodeRGBA::setColor, py::arg("color"))
            .def("setOpacity", &CCNodeRGBA::setOpacity, py::arg("opacity"))
            .def("getOpacity", &CCNodeRGBA::getOpacity);
    
    py::class_<CCRect>(m, "CCRect");

    py::class_<CCPoint>(m, "CCPoint")
        .def(py::init<>())
        .def_readwrite("x", &CCPoint::x)
        .def_readwrite("y", &CCPoint::y);

    py::class_<CCSprite, CCNodeRGBA>(m, "CCSprite")
        .def_static("create", py::overload_cast<const char*>(&CCSprite::create), py::arg("pszFileName"), py::return_value_policy::reference) // ok...
        .def_static("create", py::overload_cast<>(&CCSprite::create), py::return_value_policy::reference)
        .def_static("create", py::overload_cast<const char*, CCRect const&>(&CCSprite::create), py::arg("pszFileName"), py::arg("rect"), py::return_value_policy::reference)
        .def_static("createWithSpriteFrameName", py::overload_cast<const char*>(&CCSprite::createWithSpriteFrameName), py::arg("pszSpriteFrameName"), py::return_value_policy::reference);

    py::class_<CCLayer, CCNode>(m, "CCLayer")
            .def_static("create", &CCLayer::create, py::return_value_policy::reference);
        
}

void Py2GD::bindGeode() {
	py::class_<Notification>(m, "Notification")
		.def_static("create", static_cast<Notification* (*)(std::string const&, NotificationIcon, float)>(&Notification::create), "Creates a notification", py::return_value_policy::reference)
		.def("show", &Notification::show);
	
		// bind the log functions so people dont get confused on why its showing `stdout` on the console
		m.def("info", [=](const std::string& str) {
			log::info("[Python]: {}", str);
		});

		m.def("err", [=](const std::string& str) {
			log::error("[Python]: {}", str);
		});

		m.def("warn", [=](const std::string& str) {
			log::warn("[Python]: {}", str);
		});

		m.def("debug", [=](const std::string& str) {
			log::debug("[Python]: {}", str);
		});
}