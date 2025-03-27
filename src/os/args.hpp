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
	static void Init(int argc, const char **argv);
	Argument operator[](std::string);
	bool ArgBool(std::string name);
	uint32_t ArgInt(std::string name);
	std::string ArgStr(std::string name);
};

inline bool operator==(const Arguments::Argument &lhs,
					   const Arguments::Argument &rhs) {
	return lhs.first == rhs.first;
}

extern Arguments *gAllArgs;
