
#include "cpp-utilities/logger.h"

int main() {
	logger::emergency("This is a test");
	logger::alert("This is a test");
	logger::critical("This is a test");
	logger::error("This is a test");
	logger::warning("This is a test");
	logger::notice("This is a test");
	logger::info("This is a test");
	logger::debug("This is a test");
}
