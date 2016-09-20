#define BOOST_TEST_MODULE main
#define BOOST_TEST_MAIN

#include "cclmwe/cclmwereader.h"
#include <libcorpus2/io/cclreader.h>

#include <libcorpus2/tagsetmanager.h>
#include <libcorpus2/io/writer.h>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

const std::string tagset_name = "nkjp";

using namespace Corpus2;
using namespace Corpus2MWE;

typedef boost::shared_ptr<TokenReader> TokenReaderPtr;

struct io_fixture {
    io_fixture() {
        // expected MWE - mwe_tok : mwe_base
        mwes["czerwoną kartkę"] = "czerwona kartka";
        mwes["Ubezpieczeń Społecznych"] = "ubezpieczenie społeczne";
    }
    // MWEs extracted from input CCL
    std::map<std::string, std::string> mwes;

    // dictionary with MWEs
    std::string mwe_file = "testdata/mwedict-headed.xml";

    // input CCL - s1: "Zaklad Ubezpieczen Spolecznych dal mi wczoraj czerwona kartke"
    std::string infile = "testdata/ccl.xml";

    // output CCL
    std::string outfile = "out.xml";
};

void write_ccl(std::string, TokenReaderPtr, const Tagset &);
bool mwe_exists(Sentence::Ptr, std::string, std::string, const Tagset &);


BOOST_AUTO_TEST_SUITE( io_test )

    BOOST_FIXTURE_TEST_CASE( mwes, io_fixture ) {
        /* 1) create a CclReader and read first sentence of input file
         * 2) create reader with MWE support and get also the first sentence
         * 3) compare sizes of sentences (MWE sentence should be shorter
         *      because of MWE tokens)
         */
        const Tagset & tagset = get_named_tagset(tagset_name);

        boost::shared_ptr<CclReader> ccl_reader(new CclReader(tagset, infile));
        Sentence::Ptr s1 = ccl_reader->get_next_sentence();

        boost::shared_ptr<CclMWEReader> cclmwer(
                new CclMWEReader(tagset, infile)
        );
        Sentence::Ptr s2 = cclmwer->reader()->get_next_sentence();
        BOOST_REQUIRE(s1->size() > s2->size());
    }

    BOOST_FIXTURE_TEST_CASE( without_mwes, io_fixture ) {
        /* 1) create a CclReader and read first sentence of input file
         * 2) create reader without MWE support and get the first sentence
         * 3) compare sizes of sentences (sentences should have same
	 * 	length because of the same reader type)
         */
        const Tagset & tagset = get_named_tagset(tagset_name);

        boost::shared_ptr<CclReader> ccl_reader(new CclReader(tagset, infile));
        Sentence::Ptr s1 = ccl_reader->get_next_sentence();

        boost::shared_ptr<CclMWEReader> cclmwer(
                new CclMWEReader(tagset, infile, false)
        );
        Sentence::Ptr s2 = cclmwer->reader()->get_next_sentence();
        BOOST_REQUIRE(s1->size() == s2->size());
    }

    BOOST_FIXTURE_TEST_CASE( ccl_mwe, io_fixture ) {
        /* 1) create a reader with MWE support
         * 2) reader loads MWEs dictionary and reads CCL document with MWEs
         * 3) write CCL with extracted MWEs
         * 4) read output CCL and check if MWEs exist
         */
        const Tagset & tagset = get_named_tagset(tagset_name);
        boost::shared_ptr<CclReader> ccl_reader(new CclReader(tagset, infile));

        boost::shared_ptr<CclMWEReader> cclmwer(
                new CclMWEReader(tagset, infile)
        );
        write_ccl(outfile, cclmwer->reader(), tagset);
        ccl_reader.reset(new CclReader(tagset, outfile));
        Sentence::Ptr sent = ccl_reader->get_next_sentence();
        BOOST_FOREACH(auto& mwe, mwes) {
        	BOOST_REQUIRE(mwe_exists(sent, mwe.first, mwe.second, tagset));
        }
    }

    BOOST_FIXTURE_TEST_CASE( mwedict_not_exists, io_fixture ) {
        const Tagset & tagset = get_named_tagset(tagset_name);
	std::string mwe_dict = "./some_not_existing_mwe_dict";
        BOOST_CHECK_THROW(
                new CclMWEReader(tagset, infile, mwe_dict),
                Corpus2::Corpus2Error
        );
    }

BOOST_AUTO_TEST_SUITE_END();


void write_ccl(std::string outfile, TokenReaderPtr reader, const Tagset & tagset) {
    TokenWriter::TokenWriterPtr writer = TokenWriter::create_path_writer(
            "ccl", outfile, tagset
    );
    boost::shared_ptr<Chunk> chunk;
    while (chunk = reader->get_next_chunk()) {
        writer->write_chunk(*chunk);
    }
}

bool mwe_exists(Sentence::Ptr s, std::string mwe_tok, std::string mwe_base,
                const Tagset & tagset)
{
    std::string t_tok;
    std::string t_base;
    BOOST_FOREACH(Token *t, s->tokens()) {
        t_tok = t->orth_utf8();
        t_base = t->get_preferred_lexeme(tagset).lemma_utf8();
        if (t_tok == mwe_tok && t_base == mwe_base) {
            return true;
        }
    }
    return false;
}
