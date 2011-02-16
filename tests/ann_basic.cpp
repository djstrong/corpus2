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

#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <libcorpus2/ann/annotatedsentence.h>

BOOST_AUTO_TEST_SUITE(ann)

BOOST_AUTO_TEST_CASE( wrap )
{
	Corpus2::Sentence::Ptr s = boost::make_shared<Corpus2::Sentence>();
	s->append(new Corpus2::Token(UnicodeString::fromUTF8("orth"), PwrNlp::Whitespace::None));
	Corpus2::Sentence::Ptr a = Corpus2::AnnotatedSentence::wrap_sentence(s);
	BOOST_CHECK(s->empty());
	BOOST_REQUIRE(!a->empty());
	BOOST_CHECK_EQUAL(a->tokens()[0]->orth_utf8(), "orth");
}

BOOST_AUTO_TEST_CASE( wrap_clone )
{
	Corpus2::Sentence::Ptr s = boost::make_shared<Corpus2::Sentence>();
	s->append(new Corpus2::Token(UnicodeString::fromUTF8("orth"), PwrNlp::Whitespace::None));
	Corpus2::Sentence::Ptr a = Corpus2::AnnotatedSentence::wrap_sentence_clone(s);
	BOOST_REQUIRE(!s->empty());
	s->tokens()[0]->set_orth(UnicodeString::fromUTF8("aaa"));
	BOOST_REQUIRE(!a->empty());
	BOOST_CHECK_EQUAL(a->tokens()[0]->orth_utf8(), "orth");
}


BOOST_AUTO_TEST_SUITE_END()
