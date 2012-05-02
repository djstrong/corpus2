/*
	Copyright (C) 2010 Tomasz ¦niatowski, Adam Radziszewski, Pawe³ Kêdzia
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

#include <libpwrutils/foreach.h>
#include <libcorpus2/exception.h>
#include <libcorpus2_whole/io/relwriter.h>

#include <fstream>
#include <sstream>
#include <boost/make_shared.hpp>

namespace Corpus2 {
namespace whole {

RelationWriter::RelationWriter(const std::string &rela_path)
{
	rela_path_=  rela_path;
}


void RelationWriter::write(const std::vector< boost::shared_ptr<Relation> >& relations)
{
    	file_.reset(new std::ofstream(rela_path_.c_str()));
	
	//header
	file_->write("<relations>\n",12);

	foreach (const boost::shared_ptr<Relation>& r, relations){
		file_->write("   <rel name=\"",14);
		std::streamsize size = r->name().size();
		file_->write(r->name().c_str(),size);
		//Maybe additional info about set is needed in Relation class
		//file_->write("\" set=\"Anaphora\">\n",18);
		file_->write("\">\n",3);
		file_->write("      <from sent=\"",18);
		size = r->from()->sentence_id().size();
		file_->write(r->from()->sentence_id().c_str(),size);
		file_->write("\" chan=\"",8);
		size = r->from()->channel_name().size();
		file_->write(r->from()->channel_name().c_str(),size);
		file_->write("\">",2);
		int an_num = r->from()->annotation_number();
		std::ostringstream ss;
		ss << an_num;
		std::string str = ss.str();
		size=str.size();
		file_->write(str.c_str(),size);
		file_->write("</from>\n",8);
		file_->write("      <to sent=\"",16);
                size = r->to()->sentence_id().size();
                file_->write(r->to()->sentence_id().c_str(),size);
                file_->write("\" chan=\"",8);
                size = r->to()->channel_name().size();
                file_->write(r->to()->channel_name().c_str(),size);
                file_->write("\">",2);
                an_num = r->to()->annotation_number();
                std::ostringstream ss1;
                ss1 << an_num;
                str = ss1.str();
		size=str.size();
                file_->write(str.c_str(),size);
                file_->write("</to>\n",6);
		//file_->write("\n",1);
		file_->write("   </rel>",9);
		file_->write("\n",1);
	}
	
	
	
        //footer
	file_->write("</relations>",12);
        file_->close();
}


void write_relation(const boost::shared_ptr<Relation>& r)
{
	//std::streamsize size = r->from()->sentence_id().size();
	//f->write(r->from()->sentence_id().c_str(),size);

}
  
void do_header(){
}

void do_footer(){
}


} // whole ns
} // Corpus2 ns

