#ifndef LIBSORPUS2_UTIL_IOFORMAT_OPTIONS_H
#define LIBCORPUS2_UTIL_IOFORMAT_OPTIONS_H

#include <boost/program_options.hpp>
#include <libcorpus2/io/reader.h>
#include <libcorpus2/io/writer.h>

namespace Corpus2 {

void add_input_options(
	boost::program_options::options_description& desc,
	const std::string& default_format = "xces"
	);

void add_output_options(
	boost::program_options::options_description& desc,
	const std::string& default_format = "xces"
	);

boost::shared_ptr<Corpus2::TokenReader> create_reader(
	boost::program_options::variables_map& vm,
	const Corpus2::Tagset& tagset,
	const std::string& filename = "-"
	);

boost::shared_ptr<Corpus2::TokenWriter> create_writer(
	boost::program_options::variables_map& vm,
	const Corpus2::Tagset& tagset,
	const std::string& filename = "-"
	);

} /* end ns Corpus2 */

#endif // LIBCORPUS2_UTIL_IOFORMAT_OPTIONS_H
