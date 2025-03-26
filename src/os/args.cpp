#include "args.hpp"
#include <SDL2/SDL_assert.h>
#include <span>

extern std::string gAppName;

Arguments::Arguments(int argc, const char **argv) {
	SDL_assert(argc >= 1 && argv != nullptr);
	gAppName = argv[0];
	argv++;
	std::span<char *> args(const_cast<char **>(argv), argc - 1);
	for (std::string arg : args) {
		if (!arg.starts_with("--") &&
			arg.starts_with("-")) { // at least one bool toggle
		}
	}
}
