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

#ifndef LIBCORPUS2_TAGGING_H
#define LIBCORPUS2_TAGGING_H

#include <libcorpus2/tag.h>
#include <libcorpus2/token.h>
#include <libcorpus2/tagset.h>

namespace Corpus2 {

/**
  * Creates a mask having all values of the given attribute set.
  * Pass an empty string to get a mask for all POS values.
  * The resulting object should only be used for masking as it won't be
  * a valid tag.
  *
  * @arg tagset_name tagset to use
  * @arg attr_name attrinbute as defined in tagset or empty string for POS
  * @return mask for given attribute
  */
Tag get_attribute_mask(const Tagset& tagset,
					   const std::string attr_name);

/**
  * Projects the token onto the mask. Depending on disamb_only, will consider
  * only disamb or all lexemes.
  */
Tag mask_token(const Token& token, const Tag& mask, bool disamb_only);

/** Returns the number of set elements belonging to the mask given. */
int mask_card(const Tag& mask);

/** Forces one disamb lexeme per token. The selection is based on tagset
  * definition order. Returns if any disamb found.
  */
bool select_preferred_disamb(const Tagset& tagset, Token* token);

/** Encodes attributes with unspecified values as each value set.
  * This is to facilitate safe masking when the value in question is not to be
  * skipped.
  */
void expand_unspec_attrs(const Tagset& tagset, Token* token);

/** Repairs multivalue tags. Optional attributes will be cleared if
  * multi-value. Regular attributes will be set to lowest value given.
  */
void select_singular_tags(const Tagset& tagset, Token* token);


} /* end ns Corpus2 */

#endif // LIBCORPUS2_TAGGING_H
