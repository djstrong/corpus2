/*
    Copyright (C) 2010 Tomasz Śniatowski, Adam Radziszewski

    This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.

    This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.

    See the LICENSE and COPYING files for more details.
*/

#include <libpwrutils/exception.h>
#include <sstream>

namespace PwrNlp {

Error::Error(const std::string &what)
	: std::runtime_error(what)
{
}

Error::~Error() throw()
{
}

std::string Error::info() const
{
	return what();
}

} /* end ns PwrNlp */
