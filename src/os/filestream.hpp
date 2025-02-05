#pragma once

#include "iface/istream.hpp"

class FileStream : public IStream {
    FileStream(std::string fname, bool read_only);
    virtual ~FileStream();
    virtual void ReadImpl(size_t size, void *buf);
    virtual void WriteImpl(size_t size, const void *buf);

    FILE* mCFileObj;
    std::string mFilename;
    bool mReadOnly;
};
