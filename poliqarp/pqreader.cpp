#include "pqreader.h"
#include "pqclient.h"

namespace Corpus2 {

bool PoliqarpReader::registered = TokenReader::register_path_reader<PoliqarpReader>(
	"poliqarp","token,chunk,sentence");


PoliqarpReader::PoliqarpReader(const Tagset &tagset, const std::string &filename)
	: TokenReader(tagset), pq_(new PoliqarpClient(tagset, filename)),
	executed_(false), mode_(PQ_SENTENCES)
{
	pq_->compile_query("[]+ within s");
}

PoliqarpReader::~PoliqarpReader()
{
}

void PoliqarpReader::set_query(const std::string &query)
{
	pq_->compile_query(query);
	mode_ = PQ_MANUAL;
	executed_ = false;
}

void PoliqarpReader::execute()
{
	pq_->execute_query();
	executed_ = true;
}

Token* PoliqarpReader::get_next_token()
{
	if (!executed_) execute();
	return pq_->get_next_focus_token();
}

Sentence::Ptr PoliqarpReader::get_next_sentence()
{
	if (!executed_) execute();
	return pq_->get_next_match_sequence();
}

boost::shared_ptr<Chunk> PoliqarpReader::get_next_chunk()
{
	if (!executed_) execute();
	return pq_->get_next_document();
}

void PoliqarpReader::set_option(const std::string &option)
{
	if (option == "chunk") {
		pq_->compile_query("[]+ within p");
		mode_ = PQ_CHUNKS;
		executed_ = false;
	} else if (option == "token") {
		pq_->compile_query("[]+ within s");
		mode_ = PQ_SENTENCES;
		executed_ = false;
	} else if (option == "token") {
		pq_->compile_query("[]");
		mode_ = PQ_TOKENS;
		executed_ = false;
	} else {
		TokenReader::set_option(option);
	}
}

std::string PoliqarpReader::get_option(const std::string& option) const
{
	return TokenReader::get_option(option);
}

} /* end ns Corpus2 */
