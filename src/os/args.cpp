#include "args.hpp"
#include <SDL2/SDL_assert.h>
#include <span>
#include <string>

extern std::string gAppName;

Arguments *gAllArgs;

Arguments::Arguments(int argc, const char **argv) {
	SDL_assert(argc >= 1 && argv != nullptr);
	gAppName = argv[0];
	argc--;
	argv++;
	std::span<char *> args(const_cast<char **>(argv), argc);
	for (int i = 0; i < argc; i++) {
		std::string arg = args[i];
		if (arg.starts_with("--")) {
			std::string argname = arg.substr(2);
			if (args[i + 1] == nullptr ||
				std::string(args[i + 1]).starts_with("--")) {
				mArguments.push_back(Argument(argname, true));
			} else {
				i++;
				std::string second_half = args[i];
				if (second_half.find_first_not_of("0123456789") ==
					second_half.size()) { // fully int
					mArguments.push_back(
						Argument(argname, (uint32_t)std::stoul(second_half)));
				} else {
					mArguments.push_back(Argument(argname, second_half));
				}
			}
		}
	}
}

Arguments::~Arguments() {}

void Arguments::Init(int argc, const char **argv) {
	gAllArgs = new Arguments(argc, argv);
}

Arguments::Argument Arguments::operator[](std::string rhs) {
	for (Argument &arg : mArguments) {
		if (arg.first == rhs)
			return arg;
	}
	Argument none;
	return none;
}

bool Arguments::ArgBool(std::string name) {
	Argument arg = (*this)[name];
	if (arg.first
			.empty()) // since a nonfound arg shouldn't be, it won't have a name
		return false;
	return std::get<bool>(arg.second);
}

uint32_t Arguments::ArgInt(std::string name) {
	Argument arg = (*this)[name];
	if (arg.first.empty())
		return -1;
	return std::get<uint32_t>(arg.second);
}

std::string Arguments::ArgStr(std::string name) {
	Argument arg = (*this)[name];
	if (arg.first.empty())
		return "";
	return std::get<std::string>(arg.second);
}
