#ifndef LIBCORPUS2_ANN_CHANNEL_H
#define LIBCORPUS2_ANN_CHANNEL_H

#include <libcorpus2/ann/iob.h>
#include <vector>
#include <boost/utility.hpp>
#include <algorithm>
#include <map>

namespace Corpus2 {


/**
 * A general indexed annotation spanning a possibly disjoint group of
 * tokens, with a distinguished 'head' token.
 */
struct Annotation
{
	Annotation() : indices(), head_index(-1) {}
	std::vector<int> indices;
	int head_index;
	bool empty() const {
		return indices.empty();
	}
	bool sane() const {
		return empty() ||
			std::find(indices.begin(), indices.end(), head_index) != indices.end();
	}
};

struct AnnotationHeadCompare
{
	bool operator()(const Annotation& a1, const Annotation& a2) {
		return a1.head_index < a2.head_index;
	}
};

/**
 * An annotation channel in IOB format and segment index format, with manual
 * sync between the two.
 */
class AnnotationChannel
{
public:
	/**
	 * Empty ctor
	 */
	AnnotationChannel();

	/**
	 * Ctor for an annotation of a specific size
	 */
	explicit AnnotationChannel(int size);

	/**
	 * Discard IOB annotation information, regenerate it from the segment info.
	 */
	void make_iob_from_segments();

	/**
	 * Discard indexed annotation info, regenerate from IOB.
	 */
	void make_segments_from_iob();

	/**
	 * Number segments from 1 so there are no ommited indices, return the
	 * highest used index
	 */
	int renumber_segments();

	/**
	 * Create a vector of AnnotationSegment objects, each corresponding to
	 * an annotation, with the annotations possibly being disjoint.
	 */
	std::vector<Annotation> make_annotation_vector() const;

	/**
	 * Create a vector of AnnotationSegment objects, each corresponding to
	 * an annotation, forcing the annotations to be continous (disjoint
	 * annotations are split)
	 */
	std::vector<Annotation> make_continuous_annotation_vector() const;

	/**
	 * The segment-index array accesor
	 */
	std::vector<int>& segments() {
		return segments_;
	}

	/**
	 * The segment-index array accesor, const
	 */
	const std::vector<int>& segments() const {
		return segments_;
	}

	/**
	 * Segment index getter, 0 (no segment) if idx is out of range.
	 */
	int get_segment_at(int idx) const;

	/**
	 * The IOB data vector
	 */
	const std::vector<IOB::Enum>& iobs() const {
		return iobs_;
	}

	/**
	 * IOB getter, returns IOB::O if idx is out of range.
	 */
	IOB::Enum get_iob_at(int idx);

	/**
	 * IOB setter, out of range indices are not processed.
	 */
	void set_iob_at(int idx, IOB::Enum iob);

	/**
	 * Head flag getter, false if out of range.
	 */
	bool is_head_at(int idx) const;

	/**
	 * Head flag setter, out of range indices are not processed.
	 */
	void set_head_at(int idx, bool v);

	/**
	 * Compose a string consisting of all IOB markers in order.
	 */
	std::string dump_iob() const;

private:
	/// segment indices
	std::vector<int> segments_;

	/// IOB data
	std::vector<IOB::Enum> iobs_;

	std::vector<bool> heads_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_CHANNEL_H
