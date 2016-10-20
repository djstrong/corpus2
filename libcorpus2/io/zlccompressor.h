#ifndef ZLCCOMPRESSOR_H
#define ZLCCOMPRESSOR_H
#include "compressor.h"
#include <zlc/zlibcomplete.hpp>
class ZlcCompressor : public Compressor
{
public:
    ZlcCompressor();
    void compress(std::stringstream &ssStream, std::ostream& os);
    void decompress(std::string &s, std::istream*& is);
    void finish_compression(std::ostream &os);
    ~ZlcCompressor();
private:
    zlibcomplete::GZipCompressor * pc_compressor;
};

#endif // ZLCCOMPRESSOR_H
