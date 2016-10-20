#include "zlccompressor.h"
#include "libcorpus2/exception.h"

ZlcCompressor::ZlcCompressor()
{
    pc_compressor = new zlibcomplete::GZipCompressor(9,zlibcomplete::auto_flush);
}

void ZlcCompressor::compress(std::stringstream &ssStream, std::ostream& os){
    if(pc_compressor!=NULL)
    {
        std::string input(ssStream.str());
        std::string output = pc_compressor->compress(input);
        os << output;
    } else {
        throw Corpus2::Corpus2Error("Compressor not found!");
    }
}

void ZlcCompressor::decompress(std::string &s, std::istream*& is)
{
    zlibcomplete::GZipDecompressor c_g_zip_decompressor;
    std::string decompressed =  c_g_zip_decompressor.decompress(s);
    is = new std::istringstream(decompressed);

}

void ZlcCompressor::finish_compression(std::ostream &os){
    os << pc_compressor->finish();
}

ZlcCompressor::~ZlcCompressor(){
    if(pc_compressor!=NULL){
        delete pc_compressor;
        pc_compressor = NULL;
    }
}
