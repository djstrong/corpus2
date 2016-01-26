#include "tree.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <unicode/ustream.h>

#include <libcorpus2/tagsetmanager.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH



Properties::Properties()
{
	counter = 0;
	depth = 0;
}

void Properties::feedOn(Properties &another)
{
	foreach (tag_map::value_type kv, another.tags)
	{
		typedef std::map<recipe, int> rec_info;
		rec_info & my_recipes = tags[kv.first].recipes;
		rec_info & its_recipes = kv.second.recipes;
		
		foreach (rec_info::value_type it, its_recipes)
			my_recipes[it.first] += it.second;
		
		tags[kv.first].strength += kv.second.strength;
	}
	
	another.tags.clear();
}



void Properties::dump(std::ostream &stream) const
{
	stream << tags.size();
	foreach (Properties::tag_map::value_type kv, tags)
	{
		typedef std::map<Properties::recipe, int> rec_info;
		rec_info::iterator best = kv.second.recipes.begin();
		for (rec_info::iterator it = best; it != kv.second.recipes.end(); ++it)
			if (it->second > best->second)
				best = it;
		
		std::string str;
		best->first.first.toUTF8String(str);
		stream << '\t' << kv.first.get_pos().to_ulong() << '\t' << kv.first.get_values().to_ulong() << '\t' << boost::io::quoted(str) << '\t' << best->first.second;
	}
}

void Properties::print(std::ostream &stream) const
{
	foreach (Properties::tag_map::value_type kv, tags)
	{
		typedef std::map<Properties::recipe, int> rec_info;
		rec_info::iterator best = kv.second.recipes.begin();
		for (rec_info::iterator it = best; it != kv.second.recipes.end(); ++it)
			if (it->second > best->second)
				best = it;
		
		std::string str;
		best->first.first.toUTF8String(str);
		stream << '[' << Cfg.getTagset().tag_to_string(kv.first) << ", " << boost::io::quoted(str) << ", " << best->first.second << ']';
	}
}




void increase_count(Properties & prop)
{
	prop.counter++;
}

void Tree::insert(const Corpus2::Token &token)
{
	// insert
	
	UnicodeString orth = token.orth();
	orth.toLower();
	
	if (isNeg(token) && orth.startsWith("nie"))
		orth.remove(0, 3);
	if (isSup(token) && orth.startsWith("naj"))
		orth.remove(0, 3);
	
	Node & n = find(orth, &increase_count);
	
	Node * last_node = &n;
	Node ** it = !n.first_child ? &n.first_child : &n.lastChild()->next_sibling;
	for (int i = n.properties.depth; i < orth.length(); i++)
	{
		last_node = new Node(orth[orth.length()-1-i]);
		last_node->properties.depth = i+1;
		last_node->properties.counter = 1;
		*it = last_node;
		it = &last_node->first_child;
	}
	
	
	// set tags and recipes 
	
	foreach (const Corpus2::Lexeme & lx, token.lexemes())
	{
		if (Cfg.isForbidden(lx))
			continue;
		
		Corpus2::Lexeme lexeme = normalize(lx);
		Properties::tag_info & ti = last_node->properties.tags[lexeme.tag()];
		UnicodeString basel = lexeme.lemma();
		basel.toLower();
		
		int i = 0;
		for (; i < std::min(orth.length(), basel.length()); i++)
			if (orth[i] != basel[i])
				break;
		
		UnicodeString suffix = basel.tempSubStringBetween(i, basel.length());
		int delendum = orth.length() - i;
		
		ti.recipes[std::make_pair(suffix, delendum)] += 1;
		ti.strength += 1;
	}
}

bool Tree::isSup(const Corpus2::Token &token)
{
	const static Corpus2::Tagset & ts = Cfg.getTagset();
	const static Corpus2::mask_t sup_mask = ts.get_value_mask(std::string("sup"));
	
	foreach (const Corpus2::Lexeme & lx, token.lexemes())
		if (lx.tag().get_values_for(sup_mask).any())
			return true;
	return false;
}

bool Tree::isNeg(const Corpus2::Token &token)
{
	const static Corpus2::Tagset & ts = Cfg.getTagset();
	const static Corpus2::mask_t neg_mask = ts.get_value_mask(std::string("neg"));
	
	foreach (const Corpus2::Lexeme & lx, token.lexemes())
		if (lx.tag().get_values_for(neg_mask).any())
			return true;
	return false;
}

Corpus2::Lexeme Tree::normalize(const Corpus2::Lexeme &lexeme)
{
	Corpus2::Tag tag = lexeme.tag();
	UnicodeString lemma = lexeme.lemma();
	
	const static Corpus2::Tagset & ts = Cfg.getTagset();
	const static Corpus2::mask_t sup_mask = ts.get_value_mask(std::string("sup"));
	const static Corpus2::mask_t com_mask = ts.get_value_mask(std::string("com"));
	const static Corpus2::mask_t aff_mask = ts.get_value_mask(std::string("aff"));
	const static Corpus2::mask_t neg_mask = ts.get_value_mask(std::string("neg"));
	
	if (tag.get_values_for(neg_mask).any())
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~neg_mask) | aff_mask;
		tag.set_values(v);
		
		if (lemma.startsWith("nie"))
			lemma.remove(0, 3);
	}
	
	if (tag.get_values_for(sup_mask).any())
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~sup_mask) | com_mask;
		tag.set_values(v);
		
		if (lemma.startsWith("naj"))
			lemma.remove(0, 3);
	}
	
	int pos = lemma.indexOf(':');
	if (pos > 0)
		lemma.remove(pos);
	
	return Corpus2::Lexeme(lemma, tag);
}







void Tree::compressLemmawise()
{
	compressLemmawise(root);
}

bool Tree::compressLemmawise(Node &node)
{
	if (!node.first_child)
	{
		return true;
	}
	else if (!node.first_child->next_sibling)
	{
		if (compressLemmawise(*node.first_child) && node.first_child->properties.counter == node.properties.counter)
		{
			node.properties.feedOn(node.first_child->properties);
			delete node.first_child;
			node.first_child = NULL;
			return true;
		}
	}
	else for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
	{
		compressLemmawise(*it);
	}
	
	return false;
}





bool Tree::compressTagwise(Node &node)
{
	Node * new_list = NULL;
	
	Node * it = node.first_child;
	while (it != NULL)
	{
		Node * tmp = it->next_sibling;
		
		if (!compressTagwise(*it))
		{
			it->next_sibling = new_list;
			new_list = it;
		}
		else
		{
			it->next_sibling = NULL;
			delete it;
		}
		
		it = tmp;
	}
	
	node.first_child = new_list;
	
	if (!node.first_child && node.properties.tags.empty())
		return true;
	else
		return false;
}





void Tree::gatherTags()
{
	gatherTags(root);
	compressTagwise(root);
}

void Tree::gatherTags(Node &node)
{
	if (!node.first_child)
		return;
	
	for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
		gatherTags(*it);
	
	
	
	typedef std::map <Corpus2::Tag, int> tag_strengths;
	
	tag_strengths intersection;
	int whole_strength = 0;
	
	foreach (Properties::tag_map::value_type kv, node.first_child->properties.tags)
		intersection[kv.first] = 0;
	
	for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
	{
		foreach (Properties::tag_map::value_type kv, it->properties.tags)
		{
			if (intersection.find(kv.first) != intersection.end())
				intersection[kv.first] += kv.second.strength;
			else
				intersection.erase(kv.first);
			
			whole_strength += kv.second.strength;
		}
	}
	
	int common_sum = 0;
	foreach (tag_strengths::value_type kv, intersection)
		common_sum += kv.second;
	
	if (common_sum >= 1 * whole_strength)
		for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
			node.properties.feedOn(it->properties);
}





void Tree::guessEmpty()
{
	guessEmpty(root);
}

void Tree::guessEmpty(Node &node)
{
	for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
		guessEmpty(*it);
	
	
}




void Tree::prune()
{
	prune(root);
}

void Tree::prune(Node &node) const
{
	while (node.first_child && isBad(*node.first_child))
	{
		Node * tmp = node.first_child;
		node.first_child = node.first_child->next_sibling;
		tmp->next_sibling = NULL;
		
		collect(*tmp);
		node.properties.feedOn(tmp->properties);
		delete tmp;
	}
	
	if (! node.first_child)
		return;
	
	prune(*node.first_child);
	
	Node * prev = node.first_child;
	for (Node * it = node.first_child->next_sibling; it != NULL; it = it->next_sibling)
		if (isBad(*it))
		{
			prev->next_sibling = it->next_sibling;
			it->next_sibling = NULL;
			
			collect(*it);
			node.properties.feedOn(it->properties);
			delete it;
			it = prev;
		}
		else
		{
			prune(*it);
			prev = it;
		}
}

bool Tree::isBad(const Node &node) const
{
	//return node.properties.counter < root.properties.counter / 3000.0;
	return false;
}

void Tree::collect(Node &node)
{
	for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
	{
		collect(*it);
		node.properties.feedOn(it->properties);
	}
	delete node.first_child;
	node.first_child = NULL;
}










