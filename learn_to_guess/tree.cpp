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

void Properties::feedOn(const Properties &another)
{
	foreach (tag_info::value_type kv, another.tags)
	{
		typedef std::map<recipe, int> rec_info;
		rec_info & my_recipes = tags[kv.first];
		rec_info & its_recipes = kv.second;
		
		foreach (rec_info::value_type it, its_recipes)
			my_recipes[it.first] += it.second;
	}
}

std::ostream & operator<< (std::ostream& stream, const Properties & props)
{
	stream << props.tags.size();
	foreach (Properties::tag_info::value_type kv, props.tags)
	{
		typedef std::map<Properties::recipe, int> rec_info;
		rec_info::iterator best = kv.second.begin();
		for (rec_info::iterator it = best; it != kv.second.end(); ++it)
			if (it->second > best->second)
				best = it;
		
		std::string str;
		best->first.first.toUTF8String(str);
		stream << '\t' << kv.first.get_pos().to_ulong() << '\t' << kv.first.get_values().to_ulong() << '\t' << boost::io::quoted(str) << '\t' << best->first.second;
	}

	return stream;
}




void increase_count(Properties & prop)
{
	prop.counter++;
}

void Tree::insert(const UnicodeString &orth, const UnicodeString &base, Corpus2::Tag tag)
{
	// insert
	
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
	
	std::map<Properties::recipe, int> & map = last_node->properties.tags[unmark(tag)];
	UnicodeString a = orth, b = base;
	a.toLower(), b.toLower();
	int i = 0;
	for (; i < std::min(orth.length(), base.length()); i++)
		if (a[i] != b[i])
			break;
	
	UnicodeString suffix = base.tempSubStringBetween(i, base.length());
	int delendum = orth.length() - i;
	
	map[std::make_pair(suffix, delendum)] += 1;
}



void Tree::compress()
{
	compress(root);
}

bool Tree::compress(Node &node)
{
	if (!node.first_child)
	{
		return true;
	}
	else if (!node.first_child->next_sibling)
	{
		if (compress(*node.first_child) && node.first_child->properties.counter == node.properties.counter)
		{
			node.properties.feedOn(node.first_child->properties);
			delete node.first_child;
			node.first_child = NULL;
			return true;
		}
	}
	else for (Node * it = node.first_child; it != NULL; it = it->next_sibling)
	{
		compress(*it);
	}
	
	return false;
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
	return node.properties.counter < root.properties.counter / 3000.0;
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

Corpus2::Tag Tree::unmark(Corpus2::Tag tag)
{
	const static Corpus2::Tagset & ts = Cfg.getTagset();
	const static Corpus2::mask_t sup_mask = ts.get_value_mask(std::string("sup"));
	const static Corpus2::mask_t com_mask = ts.get_value_mask(std::string("com"));
	const static Corpus2::mask_t aff_mask = ts.get_value_mask(std::string("aff"));
	const static Corpus2::mask_t neg_mask = ts.get_value_mask(std::string("neg"));
	
	if (tag.get_values_for(sup_mask).any())
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~sup_mask) | com_mask;
		tag.set_values(v);
	}
	
	if (tag.get_values_for(neg_mask).any())
	{
		Corpus2::mask_t v = ((tag.get_values()) & ~neg_mask) | aff_mask;
		tag.set_values(v);
	}
	
	return tag;
}
















