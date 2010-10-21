#ifndef PWRNLP_BITSET_H
#define PWRNLP_BITSET_H

#include <libpwrutils/foreach.h>
#include <boost/range.hpp>
#include <bitset>
#include <boost/functional/hash.hpp>
#include <boost/pending/lowest_bit.hpp>


namespace PwrNlp {

using std::bitset;

static const size_t ulong_bits = sizeof(unsigned long) * CHAR_BIT;

typedef bitset<ulong_bits> word_bitset;


/**
 * Count set bits in a integral type.
 * http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
 */
template <typename T> inline
int count_bits_set(T v)
{
	v = v - ((v >> 1) & (T)~(T)0/3);                              // temp
	v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);         // temp
	v = (v + (v >> 4)) & (T)~(T)0/255*15;                         // temp
	return (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
}

template <size_t S> inline
size_t count_bits_set(const std::bitset<S>& b)
{
	return b.count();
}

template <size_t S> inline
size_t lowest_bit(const bitset<S>& b)
{
	// GCC specific
	return b._Find_first();
}

/**
 * Get index of lowest set bit in an integral type
 */
inline size_t lowest_bit(const unsigned long long& t)
{
	if (t <= 0) return static_cast<size_t>(-1);
	return boost::lowest_bit(t);
}

inline size_t lowest_bit(const unsigned long& t)
{
	if (t <= 0) return static_cast<size_t>(-1);
	return boost::lowest_bit(t);
}

} /* end ns PwrNlp */

namespace std {

template<size_t S> inline
size_t hash_value(bitset<S> b)
{
	size_t seed = 0;
	const bitset<S> mask(std::numeric_limits<unsigned long>::max());
	while (b.any()) {
		boost::hash_combine(seed, (b & mask).to_ulong());
		b >>= PwrNlp::ulong_bits;
	}
	return seed;
}

template<> inline
size_t hash_value(bitset<PwrNlp::ulong_bits> b)
{
	size_t seed = 0;
	boost::hash_combine(seed, b.to_ulong());
	return seed;
}

template<size_t S> inline
bool operator<(bitset<S> left, bitset<S> right)
{
	const bitset<S> mask(std::numeric_limits<unsigned long>::max());
	while (left.any()) {
		unsigned long l1 = (left & mask).to_ulong();
		unsigned long r1 = (right & mask).to_ulong();
		if (l1 < r1) {
			return true;
		} else if (l1 > r1) {
			return false;
		}
		left >>= PwrNlp::ulong_bits;
		right >>= PwrNlp::ulong_bits;
	}
	return right.any();
}

template<> inline
bool operator<(bitset<PwrNlp::ulong_bits> left, bitset<PwrNlp::ulong_bits> right)
{
	return left.to_ulong() < right.to_ulong();
}

}

#endif // PWRNLP_BITSET_H
