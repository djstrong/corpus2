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

#ifndef LIBCORPUS2_SETTINGS_H
#define LIBCORPUS2_SETTINGS_H

#include <libcorpus2/exception.h>
#include <libpwrutils/pathsearch.h>
#include <loki/Singleton.h>

namespace Corpus2 {

const int version_major = (LIBCORPUS2_VERSION_MAJOR);
const int version_minor = (LIBCORPUS2_VERSION_MINOR);
const int version_patch = (LIBCORPUS2_VERSION_PATCH);

const std::string version_string();

class Corpus2PathSearcher : public PwrNlp::PathSearcher<FileNotFound>
{
public:
	Corpus2PathSearcher();
};

typedef Loki::SingletonHolder<Corpus2PathSearcher> Path;

} /* end ns Corpus2 */

#endif // LIBCORPUS2_SETTINGS_H
