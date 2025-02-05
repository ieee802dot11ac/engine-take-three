#include "filestream.hpp"
#include <cstdio>

FileStream::FileStream(std::string filename, bool read_only) : 
	mCFileObj(fopen(filename.c_str(), read_only ? "rb" : "r+b")),
	mFilename(filename), 
	mReadOnly(read_only) {}

FileStream::~FileStream() {
	fclose(mCFileObj);
	mCFileObj = nullptr;
}

void FileStream::ReadImpl(size_t size, void *buf) {
	fread(buf, size, 1, mCFileObj);
}

void FileStream::WriteImpl(size_t size, const void *buf) {
	if (mReadOnly) {
		fprintf(stderr, "Tried to write to read-only file %s!", mFilename.c_str());
	} else fwrite(buf, size, 1, mCFileObj);
}
