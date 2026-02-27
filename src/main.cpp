#include "ui/app.hpp"

int main(int argc, char *argv[]) {
	UI::App app;
	app.setup();
	app.run();

	return 0;
}
