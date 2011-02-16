#include <libcorpus2/ann/channel.h>
#include <algorithm>
#include <boost/bind.hpp>

namespace Corpus2 {

AnnotationChannel::AnnotationChannel()
	: segments_(), iobs_()
{
}

AnnotationChannel::AnnotationChannel(int size)
	: segments_(size), iobs_(size)
{
}

void AnnotationChannel::make_iob_from_segments()
{
	int prev_seg = 0;
	for (size_t i = 0; i < segments_.size(); ++i) {
		if (segments_[i] == 0) {
			iobs_[i] = IOB::O;
		} else if (segments_[i] != prev_seg) {
			iobs_[i] = IOB::I;
		} else {
			iobs_[i] = IOB::B;
			prev_seg = segments_[i];
		}
	}
}

void AnnotationChannel::make_segments_from_iob()
{
	int sid = 0;
	bool i_can_has = false;
	for (size_t i = 0; i < segments_.size(); ++i) {
		if (iobs_[i] == IOB::O) {
			segments_[i] = 0;
			i_can_has = false;
		} else {
			if (iobs_[i] == IOB::B || !i_can_has) {
				iobs_[i] = IOB::B;
				++sid;
			}
			segments_[i] = sid;
			i_can_has = true;
		}
	}
}

int AnnotationChannel::renumber_segments()
{
	std::map<int, int> re;
	int next = 0;
	for (size_t i = 0; i < segments_.size(); ++i) {
		if (segments_[i] > 0) {
			std::map<int, int>::const_iterator ci = re.find(segments_[i]);
			if (ci != re.end()) {
				segments_[i] = ci->second;
			} else {
				++next;
				re.insert(std::make_pair(segments_[i], next));
				segments_[i] = next;
			}
		}
	}
	return next;
}

IOB::Enum AnnotationChannel::get_iob_at(int idx)
{
	if (idx >= 0 && idx < static_cast<int>(iobs_.size())) {
		return iobs_[idx];
	} else {
		return IOB::O;
	}
}

void AnnotationChannel::set_iob_at(int idx, IOB::Enum iob)
{
	if (idx >= 0 && idx < static_cast<int>(iobs_.size())) {
		iobs_[idx] = iob;
	}
}

std::vector<Annotation> AnnotationChannel::make_annotation_vector() const
{
	std::vector<Annotation> rv;
	int smax = 0;
	for (size_t i = 0; i < segments_.size(); ++i) {
		int s = segments_[i];
		if (s > smax) {
			rv.resize(smax = s);
		}
		rv[s].indices.push_back(i);
		if (rv[s].head_index == -1) {
			rv[s].head_index = i;
		}
	}
	std::sort(rv.begin(), rv.end(), AnnotationHeadCompare());
	rv.erase(std::remove_if(rv.begin(), rv.end(),
		boost::bind(&Annotation::empty, _1)));
	return rv;
}

} /* end ns Corpus2 */
