#ifndef LIBCORPUS2_TAGSET_H
#define LIBCORPUS2_TAGSET_H

#include <libcorpus2/tag.h>

#include <set>
#include <map>
#include <vector>

#include <boost/range.hpp>
#include <boost/function.hpp>

#include <libcorpus2/exception.h>
#include <libcorpus2/util/symboldictionary.h>
#include <libcorpus2/typedefs.h>

#include <unicode/unistr.h>

namespace Corpus2 {

/**
 * Exception class for signalling tag parse errors
 */
class TagParseError : public Corpus2Error
{
public:
	/// Constructor
	TagParseError(const std::string &what, const std::string& val,
			const std::string& tag, const std::string& tagset);

	/// Destructor
	~TagParseError() throw() {}

	/// Error override
	std::string info() const;

	std::string val, tag, tagset;
};

/// Exception class for signalling tagset mismatches in various situations
class TagsetMismatch : public Corpus2Error
{
public:
	/// Constructor from Tagset objects
	TagsetMismatch(const std::string& where, const Tagset& expected,
			const Tagset& actual);

	/// Constructor from tagset ids
	TagsetMismatch(const std::string& where, tagset_idx_t expected,
			tagset_idx_t actual);

	/// Destructor
	~TagsetMismatch() throw() {}

	/// Error override
	std::string info() const;

	/// ids of the mismatching tagsets
	tagset_idx_t expected_id, actual_id;
};

/**
 * Convenience function to check if tagsets match and throw if not, with the
 * given "where" circumstance string
 */
void require_matching_tagsets(const Tagset& expected, const Tagset& actual,
		const std::string& where);

/**
 * Convenience function that calls the non-template require_matching_tagsets
 * but can be called with something that has a tagset() method or a tagset
 * object.
 */
template<class T>
void require_matching_tagsets(const T& expected, const T& actual,
		const std::string& where);

class TagsetParser;

class Token;

/**
 * A tagset is the set of possible tags, the Tagset class also contains the
 * necessary string--number mappings for POS, attribute and value names.
 *
 * Tagset objects are used for tag string parsing and to create tag strings
 * from tags. Created tags must be used with the same tagset object.
 *
 * Tagsets are usually loaded from ini-like files, but note that tagset
 * file loading is delegated to a separate class, TagsetParser.
 *
 * Tagsets are referred to by name and managed centrally by TagsetManager,
 * though it is possible to have tagset objects not using the manager.
 */
class Tagset
{
public:
	/// Empty tagset constructor
	Tagset();

	/**
	 * Tagset convenience constructor, parse a string as if it were the
	 * contents of a tagset ini file
	 */
	explicit Tagset(const char*);

	/**
	 * Tag parsing -- functional version, whole tag string.
	 *
	 * A simple wrapper for string split and a call to the split string
	 * version.
	 */
	void parse_tag(const string_range& s, bool allow_extra,
			boost::function<void (const Tag&)> sink) const;

	/**
	 * Tag parsing -- functional version, whole tag string, char* overload.
	 * TODO explain why needed
	 *
	 * A simple wrapper for string split and a call to the split string
	 * version.
	 */
	void parse_tag(const char* c, bool allow_extra,
			boost::function<void (const Tag&)> sink) const {
		parse_tag(std::make_pair(c, c + strlen(c)), allow_extra, sink);
	}

	/**
	 * Tag parsing -- functional version, split tag string.
	 *
	 * This is the main tag parsing function, other variants end up calling
	 * this with some preprocessing and tweaks. Parses the tag string and
	 * calls the sink function with each of the resulting tag objects.
	 *
	 * May throw various tag parse errors.
	 *
	 * The tag string is assumed to be in colon-saprated format, with the
	 * POS name first, followed by attribute values.
	 * The tag string may contain special characters that make the parsing
	 * end up with more than one tag:
	 * - a dot (.) separates several possible values for one field
	 * - an underscore (_) indicates that all values for the attribute at
	 *   the underscore's position should be taken.
	 */
	void parse_tag(const string_range_vector& ts, bool allow_extra,
			boost::function<void (const Tag&)> sink) const;

	/**
	 * Tag parsing -- plain version, whole string.
	 *
	 * A simple wrapper for string split and a call to the split string
	 * version.
	 */
	std::vector<Tag> parse_tag(const string_range& s,
			bool allow_extra) const;

	/**
	 * Tag parsing -- plain version, whole string, char* overload.
	 *
	 * A simple wrapper for string split and a call to the split string
	 * version.
	 */
	std::vector<Tag> parse_tag(const char* c, bool allow_extra) const {
		return parse_tag(std::make_pair(c, c + strlen(c)), allow_extra);
	}

	/**
	 * Tag parsing -- plain version, split string.
	 *
	 * A wrapper to the functional version that sets up parameters so that
	 * the tags end up in a vector, which is then returned.
	 */
	std::vector<Tag> parse_tag(const string_range_vector& ts,
			bool allow_extra) const;

	/**
	 * Simple tag parsing -- whole string version.
	 *
	 * A simple wrapper for string split and a call to the split string
	 * version.
	 */
	Tag parse_simple_tag(const string_range& s, bool allow_extra) const;

	/**
	 * Simple tag parsing -- whole string version, char* overload.
	 *
	 * A simple wrapper for string split and a call to the split string
	 * version.
	 */
	Tag parse_simple_tag(const char* c, bool allow_extra) const {
		return parse_simple_tag(std::make_pair(c, c + strlen(c)),
				allow_extra);
	}

	/**
	 * Simple tag parsing -- split string version.
	 *
	 * The tag string must not contain any special characters that make the
	 * string result on more than one actual tag object (i.e. no dots,
	 * underscores or plus / pipe characters).
	 */
	Tag parse_simple_tag(const string_range_vector& ts,
			bool allow_extra) const;


	/**
	 * Create a tag from the given POS and a (unsorted) vector of values.
	 *
	 * The POS is assumed to be valid in this tagset.
	 * The values are assumed to be valid in this tagset, but are checked
	 * for correctness with regards to the POS.
	 */
	Tag make_tag(idx_t pos, mask_t values, bool allow_extra) const;

	/**
	 * Convenience function for creating a 'ign' (ignored) tag within this
	 * tagset.
	 */
	Tag make_ign_tag() const;

	/**
	 * Check if the tag is valid, that is:
	 * * has a valid POS
	 * * all atributes are valid, with either no value or a valid value for
	 *   that attribute
	 * * all required attributes are set
	 * * no extra attrbutes are set, unless allow_extra is true
	 * @return true if the tag is valid, false otherwise
	 */
	bool validate_tag(const Tag& t, bool allow_extra,
			std::ostream* os = NULL) const;

	/**
	 * Create the string representation of a tag.
	 *
	 * The output format is the same as the input format of the parsing
	 * functions. If the tag is valid, parsing the resulting string again
	 * should result in the exact same tag object (invalid tags will be
	 * output as strings anyway, but the string may trigger a parse error).
	 */
	std::string tag_to_string(const Tag& tag) const;

	/**
	 * Create the string representation of a tag with no extra attributes
	 * and optional and missing attributes with 0 values represented as
	 * attribute names.
	 *
	 * The output format is not currently valid for parsing if there are
	 * missing attributes.
	 */
	std::string tag_to_no_opt_string(const Tag &tag) const;

	size_t tag_size(const Tag& tag) const;

	bool tag_is_singular(const Tag& tag) const;

	std::vector<Tag> split_tag(const Tag& tag) const;

	/// POS name <-> index dictionary getter
	const SymbolDictionary<idx_t>& pos_dictionary() const {
		return pos_dict_;
	}

	/// attribute name <-> index dictionary getter
	const SymbolDictionary<idx_t>& attribute_dictionary() const {
		return attribute_dict_;
	}


	/// POS name -> index mapping
	/// @returns -1 on invalid name
	idx_t get_pos_index(const string_range& pos) const;

	/// POS index -> name
	/// @returns empty string on invalid index
	const std::string& get_pos_name(idx_t pos) const;

	/// POS name -> mask mapping
	/// @return null mask on invalid name
	mask_t get_pos_mask(const string_range& pos) const;

	/// POS index -> mask mapping
	/// @return null mask on invalid index
	mask_t get_pos_mask(idx_t pos) const;


	/// Attribute name -> index mapping
	/// @returns -1 on invalid name
	idx_t get_attribute_index(const string_range& a) const;

	/// Attribute index -> name
	/// @returns empty string on invalid index
	const std::string& get_attribute_name(idx_t pos) const;

	/// Value mask -> attribute index mapping.
	/// if the value mask contains values from more than one attribute,
	/// behavior is not well defined
	/// @return -1 on invalid mask
	idx_t get_value_attribute(mask_t v) const;

	/// Attribute index -> vector of valid value masks mapping
	/// @return empty vector on invalid index
	const std::vector<mask_t>& get_attribute_values(idx_t a) const;

	/// Attribute index -> combined value mask
	/// @return null mask on invalid index
	mask_t get_attribute_mask(idx_t a) const;

	/// Attribute name -> combined value mask
	/// @return null mask on invalid name
	mask_t get_attribute_mask(const string_range& a) const;


	/// Value name -> mask
	/// @returns null mask on invalid name
	mask_t get_value_mask(const std::string& v) const;

	/// Value mask -> name
	/// @returns empty string on invalid mask
	const std::string& get_value_name(mask_t v) const;


	/// Getter for the pos -> valid attributes (in order) mapping
	/// Valid attributes are both the required and optional attributes.
	/// Generally the optonal ones should be after the required ones.
	const std::vector<idx_t>& get_pos_attributes(idx_t pos) const;

	/// Getter for the pos -> valid attributes flag vector
	const std::vector<bool>& get_pos_attributes_flag(idx_t pos) const;

	/// Getter for the pos -> required attributes flag vector
	const std::vector<bool>& get_pos_required_attributes(idx_t pos) const;

	bool pos_requires_attribute(idx_t pos, idx_t attribute) const;

	bool pos_has_attribute(idx_t pos, idx_t attribute) const;

	mask_t get_pos_value_mask(idx_t pos) const;

	mask_t get_pos_required_mask(idx_t pos) const;

	int pos_count() const;

	int attribute_count() const;

	int value_count() const;

	/**
	 * Tagset cardinality counter -- the number of different valid tags
	 * this tagset represents.
	 */
	size_t size() const;

	/**
	 * Tagset cardinality counter -- the number of all the different tags
	 * this tagset represents, both valid and invalid.
	 *
	 * Return type is double since this can easily become huge.
	 */
	double size_extra() const;

	/// Tagset ID accesor
	tagset_idx_t id() const {
		return id_;
	}

	/// Tagset name accesor
	std::string name() const {
		return name_;
	}

	/// Tagset name setter
	void set_name(const std::string& name) {
		name_ = name;
	}

	/// Return a pretty name/id string for the tagset
	std::string id_string() const;

	/// Return a pretty name/id string for the tagset and a tag
	std::string id_string(const Tag& tag) const;

	/**
	 * Convenience function for the case when there is a lemma and a
	 * possibly complex tag string and we want to add the resulting lexemes
	 * into a token by parsing the tag string and creating one lexeme for
	 * every separate tag, all with the same lemma.
	 *
	 * The new lexemes are appended, duplicates are not checked.
	 */
	void lexemes_into_token(Token& tok, const UnicodeString& lemma,
			const string_range& tags) const;

	/**
	 * Convenience overload
	 */
	void lexemes_into_token(Token& tok, const std::string& lemma,
			const string_range& tags) const {
		UnicodeString u = UnicodeString::fromUTF8(lemma);
		lexemes_into_token(tok, u, tags);
	}

	/// get the original index of the POS in the tagset definition
	int get_original_pos_index(idx_t pos) const;


	struct mask_iterator
	{
		typedef mask_t value_type;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;
		typedef const mask_t *pointer;
		typedef const mask_t &reference;
		mask_iterator(const mask_iterator &i): i_(i.i_) {}
		mask_iterator(const mask_t& i) : i_(i) {}

		mask_iterator &operator++() { i_ <<= 1; return *this; }
		mask_iterator operator++(int) { return mask_iterator(i_ << 1); }
		mask_iterator &operator--() { i_ >>= 1; return *this; }
		mask_iterator operator--(int) { return mask_iterator(i_ >> 1); }

		const mask_t &operator*() const { return i_; }

		bool operator==(const mask_iterator &i) const { return i_ == i.i_; }
		bool operator!=(const mask_iterator &i) const { return i_ != i.i_; }

	private:
		mask_t i_;
	};

	boost::iterator_range<mask_iterator> all_pos_masks() const {
		return boost::iterator_range<mask_iterator>(static_cast<mask_t>(1),
				static_cast<mask_t>(1) << pos_count());
	}

	boost::iterator_range<mask_iterator> all_value_masks() const {
		return boost::iterator_range<mask_iterator>(static_cast<mask_t>(1),
				static_cast<mask_t>(1) << value_count());
	}

	const std::vector<mask_t>& all_attribute_masks() const {
		return attribute_masks_;
	}


private:
	/// Temporary solution to allow splitting the parser into a separate
	/// class
	friend class TagsetParser;

	/// Tagset name
	std::string name_;

	/// Tagset ID
	tagset_idx_t id_;

	/// Next ID
	static tagset_idx_t next_id_;

	/// String - number dictionary for the POS names
	SymbolDictionary<idx_t> pos_dict_;

	/// String - number dictionary for the attribute names
	SymbolDictionary<idx_t> attribute_dict_;

	std::map<std::string, mask_t> string_to_value_mask_;

	std::map<mask_t, std::string> value_mask_to_string_;

	/// The original indices of the POSes in the tagset definition
	std::map<idx_t, int> original_pos_indices_;

	/// mapping from attribute indices to valid value masks
	std::vector< std::vector<mask_t> > attribute_values_;

	std::vector<mask_t> attribute_masks_;

	/// reverse mapping, from a value mask to the respective attribute
	/// index (values are assumed to be unique and not shared between
	/// attributes)
	std::map<mask_t, idx_t> value_mask_to_attribute_index_;

	/// POS to valid attribute indices mapping
	/// The order of the attributes is important, as it affects string
	/// output and the behavior of the _ special character in parsing
	std::vector< std::vector<idx_t> > pos_attributes_;

	std::vector< std::vector<idx_t> > pos_required_attributes_idx_;

	std::vector<mask_t> pos_valid_value_masks_;

	std::vector<mask_t> pos_required_value_masks_;

	/// Flags for attributes which are valid for a given POS
	std::vector< std::vector<bool> > pos_valid_attributes_;

	/// Flags for attributes which are required for a given POS
	std::vector< std::vector<bool> > pos_required_attributes_;
};

/* implementation */

template<class T, class U> inline
void require_matching_tagsets(const T& expected, const U& actual,
		const std::string& where)
{
	require_matching_tagsets(expected.tagset(), actual.tagset(), where);
}

template<class T> inline
void require_matching_tagsets(const T& expected, const Tagset& actual,
		const std::string& where)
{
	require_matching_tagsets(expected.tagset(), actual, where);
}

template<class T> inline
void require_matching_tagsets(const Tagset& expected, const T& actual,
		const std::string& where)
{
	require_matching_tagsets(expected, actual.tagset(), where);
}

inline
void require_matching_tagsets(const Tagset& expected, const Tagset& actual,
		const std::string& where)
{
	if (actual.id() != expected.id()) {
		throw TagsetMismatch(where, expected, actual);
	}
}

} /* end ns Corpus2 */

#endif // LIBCORPUS2_TAGSET_H
