#include <libcorpus2/ann/channel.h>
#include <libpwrutils/foreach.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <sstream>

namespace Corpus2 {

AnnotationChannel::AnnotationChannel()
	: segments_(), iobs_(), heads_()
{
}

AnnotationChannel::AnnotationChannel(int size)
	: segments_(size), iobs_(size), heads_(size)
{
}

void AnnotationChannel::make_iob_from_segments()
{
	int prev_seg = 0;
	for (size_t i = 0; i < segments_.size(); ++i) {
		if (segments_[i] == 0) {
			iobs_[i] = IOB::O;
		} else if (segments_[i] == prev_seg) {
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

int AnnotationChannel::get_segment_at(int idx) const
{
	if (idx >= 0 && idx < static_cast<int>(segments_.size())) {
		return segments_[idx];
	} else {
		return 0;
	}
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

bool AnnotationChannel::is_head_at(int idx) const
{
	if (idx >= 0 && idx < static_cast<int>(heads_.size())) {
		return heads_[idx];
	} else {
		return false;
	}
}

void  AnnotationChannel::set_head_at(int idx, bool v)
{
	if (idx >= 0 && idx < static_cast<int>(heads_.size())) {
		heads_[idx] = v;
	}
}

std::vector<Annotation> AnnotationChannel::make_annotation_vector() const
{
	std::vector<Annotation> rv;
	int smax = 0;
	for (size_t i = 0; i < segments_.size(); ++i) {
		int s = segments_[i];
		if (s > 0) {
			if (s > smax) {
				rv.resize(smax = s);
			}
			rv[s - 1].indices.push_back(i);
			if (heads_[i]) {
				rv[s - 1].head_index = i;
			}
		}
	}
	rv.erase(std::remove_if(rv.begin(), rv.end(),
		boost::bind(&Annotation::empty, _1)), rv.end());
	foreach (Annotation& a, rv) {
		if (a.head_index == -1) {
			a.head_index = a.indices[0];
		}
	}
	std::sort(rv.begin(), rv.end(), AnnotationHeadCompare());
	return rv;
}

std::string AnnotationChannel::dump_iob() const
{
	std::stringstream ss;
	foreach (Corpus2::IOB::Enum e, iobs()) {
		ss << Corpus2::IOB::to_string(e);
	}
	return ss.str();
}

} /* end ns Corpus2 */
