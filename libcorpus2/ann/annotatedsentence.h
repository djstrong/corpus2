#ifndef LIBCORPUS2_ANN_ANNOTATEDSENTENCE_H
#define LIBCORPUS2_ANN_ANNOTATEDSENTENCE_H

#include <libcorpus2/sentence.h>
#include <libcorpus2/exception.h>
#include <libcorpus2/ann/channel.h>

namespace Corpus2 {

class AnnotationView;

class MissingAnnotationChannel : public Corpus2Error
{
public:
	MissingAnnotationChannel(const std::string& name)
		: Corpus2Error("Annotation channel missing: " + name)
	{
	}

	~MissingAnnotationChannel() throw()
	{
	}
};


class AnnotatedSentence : public Corpus2::Sentence
{
public:
	AnnotatedSentence();

	~AnnotatedSentence();

	Ptr clone_shared() const;

	void create_channel(const std::string& name);

	bool has_channel(const std::string& name) const {
		return channels_.find(name) != channels_.end();
	}

	AnnotationChannel& get_channel(const std::string& name) {
		chan_map_t::iterator i = channels_.find(name);
		if (i == channels_.end()) {
			throw MissingAnnotationChannel(name);
		}
		return i->second;
	}

	const AnnotationChannel& get_channel(const std::string& name) const {
		chan_map_t::const_iterator i = channels_.find(name);
		if (i == channels_.end()) {
			throw MissingAnnotationChannel(name);
		}
		return i->second;
	}

private:
	typedef std::map<std::string, AnnotationChannel> chan_map_t;

	chan_map_t channels_;
};

boost::shared_ptr<AnnotationView> create_view(
	const boost::shared_ptr<AnnotatedSentence>& s,
	const std::string& ann_name);


} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_ANNOTATEDSENTENCE_H
