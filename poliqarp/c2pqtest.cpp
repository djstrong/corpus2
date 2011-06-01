#include "pqclient.h"
#include <libcorpus2/tagsetmanager.h>
#include <iostream>
#include <libcorpus2/io/writer.h>

int main(int argc, char** argv)
{
	std::cerr << "C2PQTEST\n";
	if (argc < 3) return 2;
	const Corpus2::Tagset& tagset = Corpus2::get_named_tagset("kipi");
	std::cerr << argv[1] << "\n";
	std::cerr << argv[2] << "\n";
	Corpus2::PoliqarpClient pqc(tagset, argv[1]);
	pqc.compile_query(argv[2]);
	pqc.execute_query();
	boost::shared_ptr<Corpus2::TokenWriter> writer;
	writer = Corpus2::TokenWriter::create_stream_writer("plain", std::cout, tagset);
	while (Corpus2::Token* t = pqc.get_next_focus_token()) {
		writer->write_token_dispose(t);
	}
}
