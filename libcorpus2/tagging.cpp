/*
	Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski
	Part of the libcorpus2 project

	This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

	This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.

	See the LICENSE and COPYING files for more details.
*/

#include <libcorpus2/tagging.h>
#include <libcorpus2/lexeme.h>


#include <libpwrutils/foreach.h>
#include <libpwrutils/bitset.h>

namespace Corpus2 {

Tag get_attribute_mask(const Tagset& tagset, std::string attr_name)
{
	if(attr_name.empty())
	{
		// return all-POS mask
		Tag t;

		for (idx_t pos_idx = 0; pos_idx < tagset.pos_count(); ++pos_idx) {
			t.add_pos(tagset.get_pos_mask(pos_idx));
		}

		return t;
	}
	else
	{
		return Tag(0, tagset.get_attribute_mask(attr_name));
	}
}

Tag mask_token(const Token& token, const Tag& mask, bool disamb_only)
{
	Tag t;
	foreach (const Corpus2::Lexeme& lexeme, token.lexemes()) {
		if(lexeme.is_disamb() || !disamb_only) {
			t.combine_with(lexeme.tag().get_masked(mask));
		}
	}
	return t;
}

int mask_card(const Tag& mask)
{
	return PwrNlp::count_bits_set(mask.get_pos())
			+ PwrNlp::count_bits_set(mask.get_values());
}

bool select_preferred_disamb(const Tagset& tagset, Token* token)
{
	size_t lex_idx = token->get_preferred_lexeme_index(tagset);
	if(!token->lexemes()[lex_idx].is_disamb()) {
		return false;
	}

	for (size_t other_idx = 0;
		 other_idx < token->lexemes().size();
		 ++other_idx) {
		if (other_idx != lex_idx) {
			token->lexemes()[other_idx].set_disamb(false);
		}
	}
	return true;
}

void select_preferred_lexeme(const Tagset& tagset, Token* token)
{
	foreach (Lexeme& lex, token->lexemes()) {
		lex.set_disamb(true);
	}
	if (token->lexemes().size() > 1) {
		std::vector<Lexeme> one;
		one.push_back(token->get_preferred_lexeme(tagset));
		token->replace_lexemes(one);
	}
}

void expand_optional_attrs(const Tagset& tagset, Token* token)
{
	foreach (Lexeme& lex, token->lexemes()) {
		lex.set_tag(tagset.expand_optional_attrs(lex.tag()));
	}
}

void select_singular_tags(const Tagset& tagset, Token* token)
{
	foreach (Lexeme& lex, token->lexemes()) {
		lex.set_tag(tagset.select_singular(lex.tag()));
	}
}

bool disambiguate_equal(Token* token, const Tag& mask_where,
						const Tag& mask_wanted)
{
	std::vector<Lexeme> wanted;
	foreach (const Lexeme& lex, token->lexemes()) {
		Tag mask_theirs = lex.tag().get_masked(mask_where);
		if (mask_theirs == mask_wanted) {
			wanted.push_back(lex);
		}
	}
	if (wanted.empty()) {
		return false;
	}
	token->replace_lexemes(wanted);
	return true;
}

bool disambiguate_subset(Token* token, const Tag& mask_where,
						const Tag& mask_wanted)
{
	std::vector<Lexeme> wanted;
	foreach (const Lexeme& lex, token->lexemes()) {
		Tag mask_theirs = lex.tag().get_masked(mask_where);
		if (mask_theirs.get_masked(mask_wanted) == mask_theirs) {
			wanted.push_back(lex);
		}
	}
	if (wanted.empty()) {
		return false;
	}
	token->replace_lexemes(wanted);
	return true;
}

} /* end ns Corpus2 */
