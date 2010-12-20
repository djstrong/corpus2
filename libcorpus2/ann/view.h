#ifndef LIBCORPUS2_ANN_VIEW_H
#define LIBCORPUS2_ANN_VIEW_H

#include <libcorpus2/sentence.h>

namespace Corpus2 {

class AnnotatedSentence;

class AnnotationView : public Sentence
{
public:
	AnnotationView(const boost::shared_ptr<AnnotatedSentence>& original,
		const std::string& ann_name_);

	~AnnotationView();

	Ptr clone_shared() const;

	void commit();

	void commit_to(const boost::shared_ptr<AnnotatedSentence>& original,
		const std::string& ann_name);
private:
	boost::shared_ptr<AnnotatedSentence> original_;
	const std::string ann_name_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_VIEW_H
