#ifndef LIBCORPUS2_SETTINGS_H
#define LIBCORPUS2_SETTINGS_H

#include <libcorpus2/exception.h>
#include <libpwrutils/pathsearch.h>
#include <loki/Singleton.h>

namespace Corpus2 {

class Corpus2PathSearcher : public PwrNlp::PathSearcher<FileNotFound>
{
public:
	Corpus2PathSearcher();
};

typedef Loki::SingletonHolder<Corpus2PathSearcher> Path;

} /* end ns Corpus2 */

#endif // LIBCORPUS2_SETTINGS_H
