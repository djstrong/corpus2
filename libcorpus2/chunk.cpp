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

#include <libcorpus2/chunk.h>
#include <libpwrutils/foreach.h>

namespace Corpus2 {

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
	foreach (Sentence* s, sentences_) {
		delete s;
	}
}

Chunk* Chunk::clone() const
{
	Chunk* copy = new Chunk;
	foreach (Sentence* s, sentences_) {
		copy->append(s->clone());
	}
	copy->attributes_ = attributes_;
	return copy;
}

bool Chunk::has_attribute(const std::string &name) const
{
	return attributes_.find(name) != attributes_.end();
}

std::string Chunk::get_attribute(const std::string &name) const
{
	std::map<std::string, std::string>::const_iterator i;
	i = attributes_.find(name);
	if (i != attributes_.end()) {
		return i->second;
	} else {
		return "";
	}
}

void Chunk::set_attribute(const std::string &name,
		const std::string &value)
{
	attributes_[name] = value;
}

} /* end ns Corpus2 */
