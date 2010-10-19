#include <libcorpus2/token.h>
#include <sstream>
#include <libpwrutils/foreach.h>

namespace Corpus2 {

Token::Token()
	: orth_(), wa_(), lexemes_()
{
}

Token::Token(const UnicodeString &orth, PwrNlp::Whitespace::Enum wa)
	: orth_(orth), wa_(wa), lexemes_()
{
}

Token* Token::clone() const
{
	Token* t = new Token(*this);
	return t;
}

const Lexeme& Token::get_preferred_lexeme(const Tagset& tagset) const
{
	size_t idx = get_preferred_lexeme_index(tagset);
	if (idx < lexemes_.size()) {
		return lexemes_[idx];
	} else {
		throw Corpus2Error("No lexemes but best lexeme requested");
	}
}

struct preferred_lexeme_cmp
{
	const Tagset* tagset;

	bool operator()(const Lexeme& l1, const Lexeme& l2) const {
		return (!l1.is_disamb() && l2.is_disamb())
				|| (l1.is_disamb() == l2.is_disamb()
				&& (tagset->get_original_pos_index(l1.tag().get_pos_index()) >
					tagset->get_original_pos_index(l2.tag().get_pos_index())
				|| (l1.tag().get_pos() == l2.tag().get_pos()
				&& l1 < l2)));
	}
};

size_t Token::get_preferred_lexeme_index(const Tagset& tagset) const
{
	if (lexemes_.empty()) {
		return static_cast<size_t>(-1);
	}
	preferred_lexeme_cmp cmp;
	cmp.tagset = &tagset;
	std::vector<Lexeme>::const_iterator pref;
	pref = std::max_element(lexemes_.begin(), lexemes_.end(), cmp);
	return std::distance(lexemes_.begin(), pref);
}

void Token::make_ign(const Tagset& tagset)
{
	lexemes_.clear();
	add_ign(tagset);
}

void Token::add_ign(const Tagset &tagset)
{
	Lexeme lex(orth_, tagset.make_ign_tag());
	lexemes_.push_back(lex);
}

bool Token::operator ==(const Token& other) const
{
	return orth_ == other.orth_ && wa_ == other.wa_
			&& lexemes_ == other.lexemes_;
}

bool Token::check_duplicate_lexemes() const
{
	std::set<Lexeme> s(lexemes_.begin(), lexemes_.end());
	return s.size() != lexemes_.size();
}

bool Token::remove_duplicate_lexemes()
{
	size_t old_size = lexemes_.size();
	std::sort(lexemes_.begin(), lexemes_.end());
	lexemes_.erase(std::unique(lexemes_.begin(), lexemes_.end(), Lexeme::DisamblessComparator()),
			lexemes_.end());
	return old_size != lexemes_.size();
}

bool Token::orth_pos_match(mask_t pos, const UnicodeString &orth) const
{
	if (orth.length() > 0) {
		if (orth.caseCompare(orth_, 0) != 0) return false;
	}
	if (pos) {
		foreach (const Lexeme& lex, lexemes_) {
			if (lex.tag().get_pos() != pos) return false;
		}
	}
	return true;
}

} /* end ns Corpus2 */
