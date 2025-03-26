/**
 * @file msg.hpp
 * @author ieee 802.11ac
 * @brief Messaging struct using a simple packed-data format.
 * @version 0.1
 * @date 2025-03-13
 */

#pragma once

#include <algorithm>
#include <math.h>
#include <stdint.h>
#include <string>
#include <variant>
#include <vector>

struct Message {
	enum MsgType {
		kNoType,
		kDraw,		   // propagates draw calls; (void)
		kInputDigital, // input handling; (int inputId, int upOrDown)
		kInputAnalog,  // input handling; (int inputId, float newVal)
		kTranslate,	   // movement/rotation; (float pos[3], float rot[3], bool
					   // already_applied)
	};

	typedef std::variant<uint32_t, double_t, std::string> MsgNode;
	std::vector<MsgNode> mNodes;
	MsgType mType;

	Message(MsgType ty = kNoType) : mNodes(0), mType(ty) {}
	Message(const Message &msg) : mNodes(msg.mNodes.size()), mType(msg.mType) {
		std::copy(msg.mNodes.begin(), msg.mNodes.end(), mNodes.begin());
	}
	Message &operator=(const Message &msg) {
		mNodes.clear();
		mType = msg.mType;
		mNodes.reserve(msg.mNodes.size());
		std::copy(msg.mNodes.begin(), msg.mNodes.end(), mNodes.begin());
		return *this;
	}
	~Message() { mNodes.clear(); }
	bool operator==(MsgType other_type) const { return mType == other_type; }
	MsgNode &operator[](size_t i) { return mNodes.at(i); }
};
