/**
 * @file args.hpp
 * @author ieee 802.11ac
 * @brief Abstracts argument flags & options to a std::variant.
 * @version 0.1
 * @date 2025-03-26
 *
 */
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

class Arguments {
  public:
	typedef std::pair<std::string, std::variant<bool, uint32_t, std::string>>
		Argument;

  private:
	Arguments(int argc, const char **argv);
	Arguments(const Arguments &&) = delete;
	~Arguments();

	std::vector<Argument> mArguments;

  public:
	void ArgsInit(int argc, const char **argv);
	Argument &operator[](std::string);

	static Arguments &gAllArgs;
};
