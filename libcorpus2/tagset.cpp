#include <libcorpus2/exception.h>
#include <libcorpus2/tagset.h>
#include <libcorpus2/lexeme.h>
#include <libcorpus2/token.h>
#include <libcorpus2/util/settings.h>
#include <libcorpus2/tagsetparser.h>

#include <libpwrutils/foreach.h>

#include <boost/algorithm/string.hpp>
#include <boost/strong_typedef.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/pending/lowest_bit.hpp>

#include <sstream>
#include <iostream>
#include <fstream>

namespace Corpus2 {

TagParseError::TagParseError(const std::string &what,
		const std::string& val, const std::string& tag,
		const std::string& tagset)
	: Corpus2Error(what), val(val), tag(tag), tagset(tagset)
{
}

std::string TagParseError::info() const
{
	std::ostringstream ss;
	ss << what();
	if (!val.empty()) {
		ss << ": " << val;
	}
	if (!tag.empty()) {
		ss << " [" << tag << "]";
	}
	if (!tagset.empty()) {
		ss << " (" << tagset << ")";
	}
	return ss.str();
}

TagsetMismatch::TagsetMismatch(const std::string& where,
		const Tagset& expected, const Tagset& actual)
	: Corpus2Error("Tagset mismatch in " + where), expected_id(expected.id())
	, actual_id(actual.id())
{
}

TagsetMismatch::TagsetMismatch(const std::string& where,
		tagset_idx_t expected, tagset_idx_t actual)
	: Corpus2Error("Tagset mismatch in " + where), expected_id(expected)
	, actual_id(actual)
{
}

std::string TagsetMismatch::info() const
{
	std::ostringstream ss;
	ss << what() << ". Expected tagset id " << (int)expected_id
		<< ", got id " << actual_id;
	return ss.str();
}


tagset_idx_t Tagset::next_id_ = static_cast<tagset_idx_t>(0);

Tagset::Tagset()
	: id_(++next_id_)
{
}

Tagset::Tagset(const char *s)
	: id_(++next_id_)
{
	std::stringstream ss;
	ss << s;
	*this = TagsetParser::load_ini(ss);
}

std::string Tagset::id_string() const
{
	std::stringstream ss;
	ss << "tagset " << name_ << " (" << (int)id_ << ")";
	return ss.str();
}

std::string Tagset::id_string(const Tag& tag) const
{
	std::stringstream ss;
	ss << "tagset " << name_ << " (" << (int)id_ << "), ";
	ss << "tag " << tag_to_string(tag);
	return ss.str();
}

void Tagset::parse_tag(const string_range &s, bool allow_extra,
		boost::function<void(const Tag &)> sink) const
{
	string_range_vector fields;
	boost::algorithm::split(fields, s, boost::is_any_of(":"));
	parse_tag(fields, allow_extra, sink);
}

namespace {
	void append_to_multi_tag(
			std::vector< mask_t > & current,
			const std::vector<mask_t> & to_add)
	{
		size_t current_size = current.size();
		for (size_t ai = 1; ai < to_add.size(); ++ai) {
			for (size_t oi = 0; oi < current_size; ++oi) {
				current.push_back(current[oi]);
				current.back() |= to_add[ai];
			}
		}
		for (size_t i = 0; i < current_size; ++i) {
			current[i] |= to_add[0];
		}
	}
}

void Tagset::parse_tag(const string_range_vector &fields, bool allow_extra,
		boost::function<void(const Tag &)>sink) const
{
	if (fields.empty()) {
		throw TagParseError("No POS", "", "", id_string());
	}
	idx_t pos_idx = get_pos_index(fields[0]);
	if (pos_idx < 0) {
		throw TagParseError("Invalid POS",
				boost::copy_range<std::string>(fields[0]), "",
				id_string());
	}
	std::vector< mask_t > all_variants;
	all_variants.push_back(0);
	for (size_t fi = 1; fi < fields.size(); ++fi) {
		const string_range& r = fields[fi];
		if (r.size() != 1 || *r.begin() != '_') {
			string_range_vector dots;
			boost::algorithm::split(dots, r, boost::is_any_of("."));
			std::vector<mask_t> values;
			foreach (string_range& dot, dots) {
				mask_t v = get_value_mask(boost::copy_range<std::string>(dot));
				if (!v) {
					throw TagParseError("Unknown attribute value",
							boost::copy_range<std::string>(r), "",
							id_string());
				}
				values.push_back(v);
			}
			append_to_multi_tag(all_variants, values);
		} else if (!r.empty()) { // underscore handling
			if (fi - 1 >= pos_attributes_[pos_idx].size()) {
				throw TagParseError(
						"Underscore beyond last attribute for this POS",
						"", "", id_string());
			}
			idx_t attr = pos_attributes_[pos_idx][fi - 1];
			append_to_multi_tag(all_variants, attribute_values_[attr]);
		} // else empty, do nothing
	}
	foreach (mask_t variant, all_variants) {
		sink(make_tag(pos_idx, variant, allow_extra));
	}
}

std::vector<Tag> Tagset::parse_tag(const string_range& sr,
		bool allow_extra) const
{
	string_range_vector fields;
	boost::algorithm::split(fields, sr, boost::is_any_of(":"));
	return parse_tag(fields, allow_extra);
}

std::vector<Tag> Tagset::parse_tag(const string_range_vector &fields,
		bool allow_extra) const
{
	std::vector<Tag> tags;
	parse_tag(fields, allow_extra,
			  boost::bind(&std::vector<Tag>::push_back, boost::ref(tags),
				_1));
	return tags;
}

Tag Tagset::parse_simple_tag(const string_range &s, bool allow_extra) const
{
	string_range_vector fields;
	boost::algorithm::split(fields, s, boost::is_any_of(std::string(":")));
	return parse_simple_tag(fields, allow_extra);
}

Tag Tagset::parse_simple_tag(const string_range_vector &ts,
		bool allow_extra) const
{
	if (ts.empty()) {
		throw TagParseError("Empty POS+attribute list", "", "",
				id_string());
	}
	idx_t pos_idx = get_pos_index(ts[0]);
	if (pos_idx < 0) {
		throw TagParseError("Invalid POS",
				boost::copy_range<std::string>(ts[0]), "", id_string());
	}
	mask_t values = 0;
	for (size_t i = 1; i < ts.size(); ++i) {
		if (!ts[i].empty()) {
			mask_t val = get_value_mask(boost::copy_range<std::string>(ts[i]));
			if (val == 0) {
				mask_t a = get_attribute_mask(ts[i]);
				if (a != 0) {
					values &= (~a);
				} else {
					throw TagParseError("Unknown attribute value",
							boost::copy_range<std::string>(ts[i]), "",
							id_string());
				}
			} else {
				values |= val;
			}
		}
	}
	return Tag(get_pos_mask(pos_idx), values);
}

Tag Tagset::make_tag(idx_t pos_idx, mask_t values, bool allow_extra) const
{
	mask_t valid_values = get_pos_value_mask(pos_idx);
	mask_t invalid = values & ~valid_values;
	if (invalid && !allow_extra) {
		mask_t first_invalid = boost::lowest_bit(invalid);
		throw TagParseError("Attribute not valid for this POS",
				get_value_name(first_invalid),
				get_pos_name(pos_idx), id_string());
	}
	// check singularity
	return Tag(get_pos_mask(pos_idx), values);
}

Tag Tagset::make_ign_tag() const
{
	mask_t ign_pos_mask = get_pos_mask("ign");
	assert(ign_pos_mask);
	return Tag(ign_pos_mask);
}

bool Tagset::validate_tag(const Tag &t, bool allow_extra,
		std::ostream* os) const
{
	// check singularity
	if (t.pos_count() != 1) {
		if (os) {
			(*os) << " POS not singular : " << t.pos_count();
		}
		return false;
	}

	idx_t pos_idx = t.get_pos_index();
	if (!pos_dict_.is_id_valid(pos_idx)) {
		if (os) {
			(*os) << " POS not valid : " << (int)pos_idx;
		}
		return false;
	}
	std::vector<bool> valid = get_pos_attributes_flag(pos_idx);
	std::vector<bool> required = get_pos_required_attributes(pos_idx);

	for (idx_t i = 0; i < attribute_dict_.size(); ++i) {
		mask_t value = t.get_values_for(get_attribute_mask(i));
		if (value == 0) {
			if (required[i]) {
				if (os) {
					(*os)  << " Required attribuite "
						<< get_attribute_name(i)
						<< " missing";
				}
				return false;
			}
		} else {
			if (!valid[i] && !allow_extra) {
				if (os) {
					(*os) << " Extra attribute value: "
						<< get_value_name(value)
						<< " (attribute "
						<< get_attribute_name(i) << ")";
				}
				return false;
			}
		}
	}
	return true;
}

std::string Tagset::tag_to_string(const Tag &tag) const
{
	std::ostringstream ss;
	idx_t pos_idx = tag.get_pos_index();
	ss << get_pos_name(pos_idx);
	const std::vector<idx_t>& attrs = get_pos_attributes(pos_idx);
	foreach (const idx_t& a, attrs) {
		mask_t value = tag.get_values_for(get_attribute_mask(a));
		if (pos_requires_attribute(pos_idx, a) || value) {
			ss << ":";
			if (value) {
				ss << get_value_name(value);
			}
		}
	}
	// print extra attributes
	for (idx_t a = 0; a < attribute_dict_.size(); ++a) {
		if (!pos_has_attribute(pos_idx, a)) {
			mask_t value = tag.get_values_for(get_attribute_mask(a));
			if (value) {
				ss << ":" << get_value_name(value);
			}
		}
	}
	return ss.str();
}

std::string Tagset::tag_to_no_opt_string(const Tag &tag) const
{
	std::ostringstream ss;
	idx_t pos_idx = tag.get_pos_index();
	ss << get_pos_name(pos_idx);
	const std::vector<idx_t>& attrs = get_pos_attributes(pos_idx);
	foreach (const idx_t& a, attrs) {
		mask_t value = tag.get_values_for(get_attribute_mask(a));
		ss << ":";
		if (value) {
			ss << get_value_name(value);
		} else {
			ss << get_attribute_name(a);
		}
	}
	return ss.str();
}

idx_t Tagset::get_pos_index(const string_range& pos) const
{
	return pos_dict_.get_id(pos);
}

const std::string& Tagset::get_pos_name(idx_t pos) const
{
	return pos_dict_.get_string(pos);
}

mask_t Tagset::get_pos_mask(const string_range& pos) const
{
	return get_pos_mask(get_pos_index(pos));
}

mask_t Tagset::get_pos_mask(idx_t pos) const
{
	if (pos >= 0) {
		return 1 << pos;
	} else {
		return 0;
	}
}

idx_t Tagset::get_attribute_index(const string_range& a) const
{
	return attribute_dict_.get_id(a);
}

const std::string& Tagset::get_attribute_name(idx_t a) const
{
	return attribute_dict_.get_string(a);
}

const std::vector<mask_t>& Tagset::get_attribute_values(idx_t a) const
{
	static std::vector<mask_t> null_vec;
	if (a < 0 || a >= attribute_dict_.size()) {
		return null_vec;
	} else {
		return attribute_values_[a];
	}
}

mask_t Tagset::get_attribute_mask(idx_t a) const
{
	if (a < 0 || a >= attribute_dict_.size()) {
		return 0;
	} else {
		return attribute_masks_[a];
	}
}

mask_t Tagset::get_attribute_mask(const string_range& a) const
{
	return get_attribute_mask(get_attribute_index(a));
}

mask_t Tagset::get_value_mask(const std::string& v) const
{
	std::map<std::string, mask_t>::const_iterator ci;
	ci = string_to_value_mask_.find(v);
	if (ci == string_to_value_mask_.end()) {
		return 0;
	} else {
		return ci->second;
	}
}

const std::string& Tagset::get_value_name(mask_t v) const
{
	static std::string nullstr;
	std::map<mask_t, std::string>::const_iterator ci;
	ci = value_mask_to_string_.find(v);
	if (ci == value_mask_to_string_.end()) {
		return nullstr;
	} else {
		return ci->second;
	}
}

idx_t Tagset::get_value_attribute_index(mask_t v) const
{
	std::map<mask_t, idx_t>::const_iterator ci;
	ci = value_mask_to_attribute_index_.find(v);
	if (ci == value_mask_to_attribute_index_.end()) {
		return -1;
	} else {
		return ci->second;
	}
}



idx_t Tagset::get_value_attribute(mask_t v) const
{
	std::map<mask_t, idx_t>::const_iterator ci;
	ci = value_mask_to_attribute_index_.find(v);
	if (ci == value_mask_to_attribute_index_.end()) {
		return -1;
	} else {
		return ci->second;
	}
}

const std::vector<idx_t>& Tagset::get_pos_attributes(idx_t pos) const
{
	assert(pos_dict_.is_id_valid(pos));
	return pos_attributes_[pos];
}

const std::vector<bool>& Tagset::get_pos_attributes_flag(
		idx_t pos) const
{
	assert(pos_dict_.is_id_valid(pos));
	return pos_valid_attributes_[pos];
}

const std::vector<bool>& Tagset::get_pos_required_attributes(
		idx_t pos) const
{
	assert(pos_dict_.is_id_valid(pos));
	return pos_required_attributes_[pos];
}

bool Tagset::pos_requires_attribute(idx_t pos, idx_t attribute) const
{
	return pos_required_attributes_[pos][attribute];
}

bool Tagset::pos_has_attribute(idx_t pos, idx_t attribute) const
{
	return pos_valid_attributes_[pos][attribute];
}

mask_t Tagset::get_pos_value_mask(idx_t pos) const
{
	return 0; //TODO
}

mask_t Tagset::get_pos_required_mask(idx_t pos) const
{
	return 0; //TODO
}

size_t Tagset::pos_count() const
{
	return pos_dict_.size();
}

size_t Tagset::attribute_count() const
{
	return attribute_dict_.size();
}

size_t Tagset::value_count() const
{
	return value_mask_to_string_.size();
}

size_t Tagset::size() const
{
	size_t sum = 0;
	for (size_t p = 0; p < pos_dict_.size(); ++p) {
		size_t pos_size = 1;
		for (size_t i = 0; i < pos_attributes_[p].size(); ++i) {
			idx_t a = pos_attributes_[p][i];
			if (pos_required_attributes_[p][a]) {
				pos_size *= attribute_values_[a].size();
			} else {
				pos_size *= (attribute_values_[a].size() + 1);
			}
		}
		sum += pos_size;
	}
	return sum;
}

double Tagset::size_extra() const
{
	double vs = 1;
	for (size_t a = 0; a < attribute_dict_.size(); ++a) {
		vs *= (attribute_values_[a].size() + 1);
	}
	vs *= pos_dict_.size();
	return vs;
}

void Tagset::lexemes_into_token(Token& tok, const UnicodeString& lemma,
		const string_range& tags) const
{
	string_range_vector options;
	boost::algorithm::split(options, tags, boost::is_any_of("+|"));

	boost::function<Lexeme (const Tag&)> lex;
	lex = boost::bind(&Lexeme::create, boost::cref(lemma), _1);

	boost::function<void (const Tag&)> func;
	func = boost::bind(&Token::add_lexeme, boost::ref(tok),
			boost::bind(lex, _1));

	foreach (const string_range& o, options) {
		parse_tag(o, true, func);
	}
}

int Tagset::get_original_pos_index(idx_t pos) const
{
	std::map<idx_t, int>::const_iterator i =
			original_pos_indices_.find(pos);
	if (i != original_pos_indices_.end()) {
		return i->second;
	} else {
		return -1;
	}
}

} /* end ns Corpus2 */
