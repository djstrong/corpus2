/*
	Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski, Paweł Kędzia
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

#include <libcorpus2/relation.h>

namespace Corpus2 {

Relation::Relation(const std::string& name,
				   const boost::shared_ptr<const DirectionPoint> from,
				   const boost::shared_ptr<const DirectionPoint> to)
	: name_(name), from_(from), to_(to)
{
}

Relation::~Relation()
{
}

} /* end ns Corpus2 */
