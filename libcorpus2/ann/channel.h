#ifndef LIBCORPUS2_ANN_CHANNEL_H
#define LIBCORPUS2_ANN_CHANNEL_H

#include <libcorpus2/ann/iob.h>
#include <vector>
#include <boost/utility.hpp>

namespace Corpus2 {

struct Annotation
{
	Annotation() : indices(), head_index(-1) {}
	std::vector<int> indices;
	int head_index;
	bool empty() const { return indices.empty(); }
};

class AnnotationChannel
{
public:
	AnnotationChannel();

	explicit AnnotationChannel(int size);

	void make_iob_from_segments();

	void make_segments_from_iob();

	std::vector<Annotation> make_annotation_vector() const;

	std::vector<Annotation> make_continuous_annotation_vector() const;

	const std::vector<int>& segments() const {
		return segments_;
	}

	const std::vector<IOB::Enum>& iobs() const {
		return iobs_;
	}

	IOB::Enum get_iob_at(int idx);

	void set_iob_at(int idx, IOB::Enum iob);

private:
	std::vector<int> segments_;

	std::vector<IOB::Enum> iobs_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_CHANNEL_H
