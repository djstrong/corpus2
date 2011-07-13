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


} /* end ns Corpus2 */
