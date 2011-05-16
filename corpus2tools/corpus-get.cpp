#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/util/ioformat-options.h>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, char** argv)
{
	std::string tagset_name, filename;
	std::string input_format, output_format;
	int sentence, token = -1;
	size_t stats = 0;
	using boost::program_options::value;
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
			("filename,F", value(&filename),
			 "filename")
			("sentence,S", value(&sentence),
			 "Sentence idx")
			("stats,s", value(&stats),
			 "Stats")
			("token,T", value(&token),
			 "Token idx ")
			("tagset,t", value(&tagset_name)->default_value("kipi"),
			 "Tagset name")
			;
	Corpus2::add_input_options(desc);
	Corpus2::add_output_options(desc);
	boost::program_options::variables_map vm;
	boost::program_options::positional_options_description p;
	p.add("filename", 1);
	p.add("sentence", 1);
	p.add("token", 1);

	try {
		boost::program_options::store(
			boost::program_options::command_line_parser(argc, argv)
			.options(desc).positional(p).run(), vm);
	} catch (boost::program_options::error& e) {
		std::cerr << e.what() << "\n";
		return 2;
	}
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}
	const Corpus2::Tagset& tagset = Corpus2::get_named_tagset(tagset_name);
	boost::shared_ptr<Corpus2::TokenReader> reader;
	reader = Corpus2::create_reader(vm, tagset, filename);
	Corpus2::Sentence::Ptr s;
	boost::shared_ptr<Corpus2::TokenWriter> writer;
	writer = Corpus2::create_writer(vm, tagset);
	std::map<int,int> lens;
	for (int i = 0; i <= sentence; ++i) {
		s = reader->get_next_sentence();
		if (s) {
			lens[s->size()]++;
			if (s->size() > stats) {
				std::cerr << i << "\n";
				writer->write_sentence(*s);
			}
		}
	}
	if (s) {
		if (token == -1) {
			writer->write_sentence(*s);
		} else if (static_cast<size_t>(token) < s->size()) {
			writer->write_token(*(*s)[token]);
		}
	}
	if (stats) {
		typedef std::pair<int,int> pp;
		foreach (const pp& p, lens) {
			std::cerr << p.first << " " << p.second << "\n";
		}
	}
}
