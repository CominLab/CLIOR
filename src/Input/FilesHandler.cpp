/*
 * FilesHandler.cpp
 *
 *  Created on: 28/giu/2016
 *      Author: samuele
 */

#include "FilesHandler.h"

SingleEnd_G_C::SingleEnd_G_C()
{
	this->reset();
}

bool SingleEnd_G_C::init(string& path_groups, string& path_classification) {
	this->open_correct = true;
	ifstream file_groups(path_groups);
	ifstream file_classification(path_classification);
	if(file_groups.is_open() && file_classification.is_open())
	{
		//Verifica se corretto formato input gruppi
		string line_groups_test;
		getline(file_groups, line_groups_test);
		vector<string> parse_groups;
		parseLine(line_groups_test, parse_groups, {","});
		if(parse_groups.size() == 2)
			this->open_correct &= true;

		//Verifica se corretto formato input classificazione
		string line_classification_test;
		getline(file_classification, line_classification_test);
		vector<string> parse_classification;
		parseLine(line_classification_test, parse_classification, {","});
		if(line_classification_test.size() == 3)
			this->open_correct &= true;

		if(this->open_correct)
		{
			this->path_groups = path_groups;
			this->path_classification = path_classification;
			parseLine(path_groups, this->parse_path_groups, {"/"});
			parseLine(path_classification, this->parse_path_classification, {"/"});
		}

		file_groups.close();
		file_classification.close();
	}
	else
	{
		this->open_correct = false;
		if(path_groups != "" && !file_groups.is_open())
			cerr << endl << "Fail to open: " << path_groups << flush;
		if(path_classification != "" && file_classification.is_open())
			cerr << endl << "Fail to open: " << path_classification << flush;
		if(file_groups.is_open())
			file_groups.close();
		if(file_classification.is_open())
			file_classification.close();
	}
	if(!this->open_correct)
		this->reset();

	return this->open_correct;
}

const vector<string>& SingleEnd_G_C::getParsePath(File type) const {
	switch (type)
	{
		case (Groups):
			return this->parse_path_groups;
		case (Classification):
			return this->parse_path_classification;
	}
}

const string& SingleEnd_G_C::getPath(File type) const {
	switch (type)
	{
		case (Groups):
			return this->path_groups;
		case (Classification):
			return this->path_classification;
	}
}

const string SingleEnd_G_C::getDirectory(File type) const {
	string dir = "";
	switch (type)
	{
		case (Groups):
			for(size_t i = 0; i < this->parse_path_groups.size() - 1; ++i)
				dir += this->parse_path_groups[i] + "/";
			return dir;
		case (Classification):
			for(size_t i = 0; i < this->parse_path_classification.size() - 1; ++i)
				dir += this->parse_path_classification[i] + "/";
			return dir;
	}
}

const string& SingleEnd_G_C::getFilename(File type) const {
	switch (type)
	{
		case (Groups):
			return this->parse_path_groups.back();
		case (Classification):
			return this->parse_path_classification.back();
	}
}

const string SingleEnd_G_C::getFilenameWithoutExt(File type) const {
	string filename_without_ext = "";
	switch (type)
	{
		case (Groups):
			filename_without_ext = this->parse_path_groups.back();
			break;

		case (Classification):
			filename_without_ext = this->parse_path_classification.back();
			break;
	}
	string delimiter = ".";
	size_t pos = filename_without_ext.rfind(delimiter);
	filename_without_ext.erase(pos, filename_without_ext.length());
	return filename_without_ext;
}

const string SingleEnd_G_C::getExt(File type) const {
	string filename_without_ext = "";
	switch (type)
	{
		case (Groups):
			filename_without_ext = this->parse_path_groups.back();
			break;

		case (Classification):
			filename_without_ext = this->parse_path_classification.back();
			break;
	}
	string delimiter = ".";
	size_t pos = filename_without_ext.rfind(delimiter);
	return filename_without_ext.substr(pos+1, filename_without_ext.length());
}

bool SingleEnd_G_C::isCorrect() const {
	return this->open_correct;
}

void SingleEnd_G_C::reset() {
	this->open_correct = false;
	this->path_groups = "";
	this->path_classification = "";
	this->parse_path_groups.clear();this->parse_path_groups.shrink_to_fit();
	this->parse_path_classification.clear();this->parse_path_classification.shrink_to_fit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

PairedEnd_G_C::PairedEnd_G_C()
{
	this->reset();
}

bool PairedEnd_G_C::init(string& path_groups_1, string& path_classification_1,
		string& path_groups_2, string& path_classification_2) {
	bool init1 = this->first.init(path_groups_1, path_classification_1);
	bool init2 = this->second.init(path_groups_2, path_classification_2);
	if(init1 != init2)
	{
		if(init1 == true)
			this->pair_type = SingleEnd;
		else //init1 != init2 -> init1==false e init2== true, then change
		{
			SingleEnd_G_C& tmp = this->first;
			this->first = this->second;
			this->second = tmp;
		}
	}
	else
		if(init1 == true) //init1 == init2 -> init2 == true
			this->pair_type = PairedEnd;
		else
			this->pair_type = UnknownPair;
}

bool PairedEnd_G_C::isCorrect() {
	if(this->pair_type == SingleEnd)
		return get<0>(*this).isCorrect();
	else if(this->pair_type == PairedEnd)
		return get<0>(*this).isCorrect() && get<1>(*this).isCorrect();
	else
		return false;
}

PairType PairedEnd_G_C::getType() const {
	return pair_type;
}

void PairedEnd_G_C::reset() {
	this->pair_type = UnknownPair;
}
