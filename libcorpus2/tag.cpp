#include <libcorpus2/tag.h>
#include <libcorpus2/tagsetmanager.h>

#include <libpwrutils/foreach.h>

#include <cstring>
#include <sstream>

#include <boost/functional/hash.hpp>
#include <boost/pending/lowest_bit.hpp>

namespace Corpus2 {


template <typename T>
int count_bits_set(T v)
{
	// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
	v = v - ((v >> 1) & (T)~(T)0/3);                           // temp
	v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);      // temp
	v = (v + (v >> 4)) & (T)~(T)0/255*15;                      // temp
	return (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
}

int Tag::pos_count() const
{
	return count_bits_set(pos_);
}

int Tag::get_pos_index() const
{
	if (pos_ == 0) return -1;
	return boost::lowest_bit(pos_);
}

std::string Tag::raw_dump() const
{
	std::ostringstream ss;
	ss << "[";
	//ss << static_cast<int>(tagset_id_);
	ss << "#" << (void*)(pos_);
	ss << ":" << (void*)(values_) ;
	ss << "]";
	return ss.str();
}

bool Tag::operator<(const Tag& other) const
{
	return pos_ < other.pos_ || (pos_ == other.pos_ && values_ < other.values_);
}

bool Tag::operator ==(const Tag& other) const
{
	return pos_ == other.pos_ && values_ == other.values_;
}

size_t hash_value(const Tag& tag)
{
	std::size_t seed = 0;
	boost::hash_combine(seed, tag.get_pos());
	boost::hash_combine(seed, tag.get_values());
	return seed;
}

} /* end ns Corpus2 */
