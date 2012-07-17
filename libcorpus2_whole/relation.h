/*
	Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski, Paweł Kędzia
	Part of the libcorpus2 project

	This program is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

	This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.

	See the LICENSE.CORPUS2, LICENSE.POLIQARP, COPYING.LESSER and COPYING files for more details.
*/

#ifndef LIBCORPUS2_RELATIONT_H
#define LIBCORPUS2_RELATIONT_H

#include <string>
#include <boost/shared_ptr.hpp>

namespace Corpus2 {
namespace whole {

/**
 * Helper class to represent one of two point of direction in any relation.
 * Each relation has two points: from (source) and to (target)
 */
class DirectionPoint
{
public:
	/**
	 * @param sentence_id Sentence identifier
	 * @param channel_name Channel name
	 * @param annotation_number Annotation number
	 */
	DirectionPoint(const std::string sentence_id,
			  const std::string channel_name,
			  const int annotation_number)
		: sentence_id_(sentence_id), channel_name_(channel_name),
		annotation_number_(annotation_number)
	{
		//
	}

	/// Sentence identifier accessor
	const std::string sentence_id() const {
		return sentence_id_;
	}

	/// Channel name accessor
	const std::string channel_name() const {
		return channel_name_;
	}

	/// Annotatio number accessor
	int annotation_number() const {
		return annotation_number_;
	}

private:
	const std::string sentence_id_;
	const std::string channel_name_;
	const int annotation_number_;
};

/**
 * Class to represent directed relation. As points of direction uses
 * DirectionPoint class. Relation contains two points: from and to
 */
class Relation
{
public:
	/**
	 * Makes directed relation
	 * @param name Name of the relation
	 * @param from Source of relation direction
	 * @param to Target of relation direction
	 */
	Relation(const std::string& name,
			 const boost::shared_ptr<const DirectionPoint> from,
			 const boost::shared_ptr<const DirectionPoint> to);

	/**
	 * Makes directed relation
	 * @param name Name of the relation
	 * @param from Source of relation direction
	 * @param to Target of relation direction
	 */
	Relation(const std::string& name,
			 const DirectionPoint& from,
			 const DirectionPoint& to);

	~Relation();

	/// Accessor to "from" direction point
	const boost::shared_ptr<const DirectionPoint>& from() const {
		return from_;
	}

	/// Accessor to "to" direction point
	const boost::shared_ptr<const DirectionPoint>& to() const {
		return to_;
	}

	/// Accessor to name of the relation
	const std::string name() const {
		return name_;
	}

private:
	/// Direction name
	const std::string name_;

	/// Direction points: from and to
	const boost::shared_ptr<const DirectionPoint> from_;
	const boost::shared_ptr<const DirectionPoint> to_;
};

} // whole ns
} // Corpus2 ns

#endif // LIBCORPUS2_RELATIONT_H
