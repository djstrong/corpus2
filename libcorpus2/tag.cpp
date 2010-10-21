#include <libcorpus2/tag.h>
#include <libcorpus2/tagsetmanager.h>

#include <libpwrutils/foreach.h>
#include <libpwrutils/util.h>

#include <cstring>
#include <sstream>

#include <boost/functional/hash.hpp>

#include <bitset>

namespace Corpus2 {

int Tag::pos_count() const
{
	return PwrNlp::count_bits_set(pos_);
}

int Tag::get_pos_index() const
{
	if (pos_ == 0) return -1;
	return PwrNlp::lowest_bit(pos_);
}

std::string Tag::raw_dump() const
{
	std::ostringstream ss;
	ss << "[";
	std::bitset<sizeof(mask_t) * CHAR_BIT> binaryp(pos_);
	std::bitset<sizeof(mask_t) * CHAR_BIT> binaryv(values_);
	//ss << static_cast<int>(tagset_id_);
	ss << "#" << binaryp;
	ss << ":" << binaryv;
	ss << "]";
	return ss.str();
}

bool Tag::operator<(const Tag& other) const
{
	return pos_ < other.pos_ ||
		(pos_ == other.pos_ &&
		 values_ < other.values_);
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
