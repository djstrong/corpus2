#include <libcorpus2/ann/view.h>
#include <libcorpus2/ann/annotatedsentence.h>
#include <boost/make_shared.hpp>

namespace Corpus2 {

AnnotationView::AnnotationView(
	const boost::shared_ptr<AnnotatedSentence> &original,
	const std::string &ann_name)
	: Sentence(), original_(original), ann_name_(ann_name)
{
}

AnnotationView::~AnnotationView()
{
}

Sentence::Ptr AnnotationView::clone_shared() const
{
	boost::shared_ptr<AnnotationView> copy;
	copy = boost::make_shared<AnnotationView>(original_, ann_name_);
	foreach (const Token* t, tokens_) {
		copy->append(t->clone());
	}
	return copy;
}

void AnnotationView::release_original()
{
	original_.reset();
}

void AnnotationView::commit()
{
	commit_to(original_, ann_name_);
}

void AnnotationView::commit_to(
	const boost::shared_ptr<AnnotatedSentence> &original,
	const std::string &ann_name)
{
	if (!original) {
		throw AnnotationViewOutOfSync("null-pointer-to-original");
	}
	if (!original->has_channel(ann_name)) {
		throw MissingAnnotationChannel(ann_name);
	}
	// ...
}

} /* end ns Corpus2 */
