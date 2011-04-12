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

	int size() const {
		return segments_.size();
	}

	void resize(int size);

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
	 * Figure out an index for a new segment
	 */
	int get_new_segment_index() const;

	enum AnnotationVectorMode
	{
		O_DISJOINT_EXCLUSIVE = 0,
		O_CONTINUOUS = 1,
		O_INCLUSIVE = 2,
		O_CONTINUOUS_INCLUSIVE = 3,
	};

	/**
	 * Create a vector of AnnotationSegment objects, each corresponding to
	 * an annotation, with the annotations possibly being disjoint unless
	 * O_CONTINUOUS is specified in mode, and omiting unanottated tokens unless
	 * O_INCLUSIVE is specified.
	 */
	std::vector<Annotation> make_annotation_vector(
		AnnotationVectorMode mode = O_DISJOINT_EXCLUSIVE) const;

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
	 * Segment index setter, out of range indices are not processed.
	 */
	void set_segment_at(int token_idx, int segment_idx);

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

	/**
	 * Compose a string consisting of all segment indices in order, e.g. "01102"
	 */
	std::string dump_segments() const;

	/**
	 * Compose a string consisting of all head flags in order, e.g. " H  H"
	 */
	std::string dump_heads() const;

	/**
	 * Compose a string consisting of segment/head info in alphabetic format
	 * where capital letters denote the head flag, and underscore indicates
	 * no segment, so e.g. "_Aa_B"
	 */
	std::string dump_alpha() const;

	void do_counts(int& annotations, int& disjoint, int& unannotated) const;

private:
	/// segment indices
	std::vector<int> segments_;

	/// IOB data
	std::vector<IOB::Enum> iobs_;

	std::vector<bool> heads_;
};

} /* end ns Corpus2 */

#endif // LIBCORPUS2_ANN_CHANNEL_H
