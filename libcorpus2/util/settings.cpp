#include <libcorpus2/util/settings.h>

#ifdef HAVE_CONFIG_D_H
#include <libcorpus2/config_d.h>
#endif

#ifndef LIBCORPUS2_PATH_SEPARATOR
#define LIBCORPUS2_PATH_SEPARATOR ":"
#endif

namespace Corpus2 {

Corpus2PathSearcher::Corpus2PathSearcher()
	: PwrNlp::PathSearcher<FileNotFound>(LIBCORPUS2_PATH_SEPARATOR)
{
#ifdef LIBCORPUS2_DATA_DIR
	set_search_path(LIBCORPUS2_DATA_DIR);
#else
	set_search_path(".");
#endif
}

} /* end namespace Corpus2 */
