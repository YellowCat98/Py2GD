#include <Geode/Geode.hpp>
#include "Py2GD.hpp"

$execute {
	if (!Py2GD::initInterpreterAndBindings()) {
		geode::log::warn("Python is already initialized");
	} else {
		geode::log::info("Python initialized.");
	}
}