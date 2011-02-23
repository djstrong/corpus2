#include <libcorpus2/ann/annotatedsentence.h>
#include <libcorpus2/ann/view.h>
#include <boost/make_shared.hpp>
#include <sstream>
#include <libpwrutils/plural.h>

namespace Corpus2 {

AnnotatedSentence::AnnotatedSentence()
	: Sentence(), channels_()
{
}

AnnotatedSentence::~AnnotatedSentence()
{
}

Sentence::Ptr AnnotatedSentence::clone_shared() const
{
	boost::shared_ptr<AnnotatedSentence> copy;
	copy = boost::make_shared<AnnotatedSentence>();
	foreach (const Token* t, tokens_) {
		copy->append(t->clone());
	}
	copy->channels_ = channels_;
	return copy;
}

boost::shared_ptr<AnnotatedSentence> AnnotatedSentence::wrap_sentence(
	const boost::shared_ptr<Sentence>& s)
{
	boost::shared_ptr<AnnotatedSentence> a = boost::make_shared<AnnotatedSentence>();
	foreach (Token* t, s->tokens()) {
		a->append(t);
	}
	s->release_tokens();
	return a;
}

boost::shared_ptr<AnnotatedSentence> AnnotatedSentence::wrap_sentence_clone(
	const boost::shared_ptr<Sentence>& s)
{
	boost::shared_ptr<AnnotatedSentence> a = boost::make_shared<AnnotatedSentence>();
	foreach (Token* t, s->tokens()) {
		a->append(t->clone());
	}
	return a;
}

void AnnotatedSentence::create_channel(const std::string& name)
{
	channels_.insert(std::make_pair(name, AnnotationChannel(tokens_.size())));
}


boost::shared_ptr<AnnotationView> create_view(
	const boost::shared_ptr<AnnotatedSentence>& s,
	const std::string& ann_name)
{
	const AnnotationChannel& chan = s->get_channel(ann_name);
	std::vector<Annotation> ann = chan.make_annotation_vector(AnnotationChannel::O_INCLUSIVE);
	boost::shared_ptr<AnnotationView> view;
	view = boost::make_shared<AnnotationView>(s, ann_name);
	foreach (const Annotation& a, ann) {
		UnicodeString orth;
		orth = s->tokens()[a.indices[0]]->orth();
		for (size_t idxi = 1; idxi < a.indices.size(); ++idxi) {
			int idx = a.indices[idxi];
			orth += PwrNlp::Whitespace::to_whitespace(s->tokens()[idx]->wa());
			orth += s->tokens()[idx]->orth();
		}
		Token* t = new Token(orth, s->tokens()[a.indices[0]]->wa());
		Token* head_token = s->tokens()[a.head_index];
		std::copy(head_token->lexemes().begin(), head_token->lexemes().end(),
			std::back_inserter(t->lexemes()));
		view->append(t);
	}
	return view;
}

void AnnotatedSentence::append(Token *t)
{
	Sentence::append(t);
	foreach (chan_map_t::value_type& v, channels_) {
		v.second.resize(size());
	}
}

std::string AnnotatedSentence::annotation_info() const
{
	std::stringstream ss;
	foreach (const chan_map_t::value_type& v, channels_) {
		ss << "Channel " << v.first << ": \t";
		int ann, disj, un;
		v.second.do_counts(ann, disj, un);
		ss << PwrNlp::enpln(ann, "annotation") << ", ";
		ss << disj << " disjoint, ";
		int a = size() - un;
		double r = (double)a / size();
		ss << "annotations span: " << a << "/" << size() << " tokens (" << r*100 << "%)";
		ss << "\n";
	}
	return ss.str();
}

} /* end ns Corpus2 */
