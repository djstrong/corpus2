#include <libcorpus2/io/xces.h>
#include <libcorpus2/io/reader.h>
#include <libpwrutils/foreach.h>
#include <libxml++/libxml++.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/element.h>
#include <libxml++/parsers/saxparser.h>

#include <iostream>

namespace Corpus2 {

XcesError::XcesError(const std::string &what)
	: Error(what)
{
}

XcesError::~XcesError() throw()
{
}

} /* end ns Corpus2 */
