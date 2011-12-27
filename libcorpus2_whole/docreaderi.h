#ifndef READERI_H
#define READERI_H

#include <string>

#include <libcorpus2_whole/document.h>
#include <boost/shared_ptr.hpp>

namespace Corpus2 {

/**
 * Reader interface,
 * Contains method to get reader type (such as document, poliqarp).
 * Method for reading read must by implemented!
 */
class DocumentReaderI
{
public:
	DocumentReaderI(const std::string type) : type_(type) {}

	const std::string& type() const { return type_; }

	virtual boost::shared_ptr<Document> read() = 0;

protected:
	const std::string type_;
};

}

#endif // READERI_H
