#pragma once

#include "iface/istream.hpp"

class FileStream : public IStream {
    template <bool IsRO> FileStream(std::string fname);
};
