#ifndef PWRNLP_SENTENCE_H
#define PWRNLP_SENTENCE_H

#include <libpwrutils/foreach.h>
#include <boost/range.hpp>
#include <vector>

namespace PwrNlp {

	/**
	 * A simple sentence wrapper template, a template not a plain class since
	 * it is intended for use with various types of tokens.
	 *
	 * A sentence owns its Tokens.
	 */
	template<typename TT>
	class SentenceTemplate : private boost::noncopyable
	{
	public:
		/// Empty constructor
		SentenceTemplate()
			: tokens_()
		{
		}

		/// Range constructor
		template <typename T>
		explicit SentenceTemplate(const T& range)
			: tokens_()
		{
			std::copy(range.begin(), range.end(), std::back_inserter(tokens_));
		}

		SentenceTemplate* clone() const;

		/// Destructor
		~SentenceTemplate()
		{
			foreach (TT* t, tokens_) {
				delete t;
			}
		}

		bool empty() const {
			return tokens_.empty();
		}

		/// Size accesor
		size_t size() const {
			return tokens_.size();
		}

		/// Token accesor
		TT* operator[](size_t idx) {
			return tokens_[idx];
		}

		/// Token accesor, const
		const TT* operator[](size_t idx) const {
			return tokens_[idx];
		}

		/// Underlying vector accesor, const
		const std::vector<TT*>& tokens() const {
			return tokens_;
		}

		/// Underlying vector accesor
		std::vector<TT*>& tokens() {
			return tokens_;
		}

		/// Helper function for appending tokens
		void append(TT* t) {
			tokens_.push_back(t);
		}

		/// convenience first token accesor
		const TT* first_token() const {
			assert(!tokens_.empty());
			return tokens_[0];
		}

	private:
		/// The tokens this sentence contains and owns
		std::vector<TT*> tokens_;
	};

	template<class TT>
	SentenceTemplate<TT>* SentenceTemplate<TT>::clone() const
	{
		SentenceTemplate<TT>* s = new SentenceTemplate<TT>;
		foreach (const TT* t, tokens_) {
			s->append(t->clone());
		}
		return s;
	}

} /* end ns PwrNlp */

#endif // PWRNLP_SENTENCE_H
