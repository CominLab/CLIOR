/*
 * InputClass.cpp
 *
 *  Created on: 10/apr/2015
 *      Author: Samuele Girotto
 */

#include "Input.h"

void PairFiles::reset() {
	this->pair_type = UnknownPair;
	this->identify = "";
}

PairFiles::PairFiles() {
	this->reset();
}

void PairFiles::init(const string& path_end1, const string& path_end2) {
	this->reset();

	this->first.init(path_end1);
	this->second.init(path_end2);

	bool init1 = this->first.isCorrect();
	bool init2 = this->second.isCorrect();
	if(init1 != init2)
	{
		if(init1 == true)
			this->pair_type = SingleEnd;
		else //init1 != init2 -> init1==false e init2== true, then change
		{
			SingleEndFile& tmp = this->first;
			this->first = this->second;
			this->second = tmp;
		}
	}
	else
		if(init1 == true) //init1 == init2 -> init2 == true
			this->pair_type = PairedEnd;
		else
			this->pair_type = UnknownPair;

	if(init1 || init2)
	{
		if(this->pair_type == SingleEnd)
			this->identify = this->first.getFilename();
		else if (this->pair_type == PairedEnd)
			this->identify = this->first.getFilenameWithoutExt();
	}
}

bool PairFiles::isCorrect() {
	if(this->pair_type == SingleEnd)
		return get<0>(*this).isCorrect();
	else if(this->pair_type == PairedEnd)
		return get<0>(*this).isCorrect() && get<1>(*this).isCorrect();
	else
		return false;
}

PairType PairFiles::getType() const {
	return pair_type;
}

void SingleEndFile::reset() {
	this->path = "";
	this->path_parse.clear(); this->path_parse.shrink_to_fit();
	this->file_type = UnknownFile;
	this->read_delimiter = '>';
	this->open_correct = false;
	this->map_idxF_idxA.clear();
}

SingleEndFile::SingleEndFile() {
	this->reset();
}

bool SingleEndFile::init(const string& path)
{
	this->reset();

	string line; //store the read data
	ifstream file(path);
	if(file.is_open())
	{
		//Prendi prima riga e guarda primo elemento per capire la tipologia di file
		getline(file,line);
		if(!line.empty())
		{
			this->path = path;
			parseLine(this->path, this->path_parse, {"/"});
			this->open_correct = true;

			if(line[0] == '>')
			{
				this->file_type = FileType::Fasta;
				this->read_delimiter = '>';
			}
			if(line[0] == '@')
			{
				this->file_type = FileType::Fastq;
				this->read_delimiter = '@';
			}
			file.close();
			return true;
		}
		else
		{
			file.close();
			return false;
		}
	}
	else
	{
		if(path != "")
			cerr << endl << "Fail to open: " << path << flush;
		return false;
	}
}

bool SingleEndFile::isCorrect() const {
	return this->open_correct && this->file_type != UnknownFile;
}

void SingleEndFile::AddSequenceMap(id_seq_type idxFile, id_seq_type idxApp) {
	this->map_idxF_idxA[idxFile] = idxApp; //Accedo direttamente perché devo solo inserire
}

const string& SingleEndFile::getPath() const {
	return path;
}

const vector<string>& SingleEndFile::getPathParse() const {
	return path_parse;
}

const string SingleEndFile::getDirectory() const
{
	string dir = "";
	for(size_t i = 0; i < this->path_parse.size() - 1; ++i)
		dir += this->path_parse[i] + "/";
	return dir;
}

const string& SingleEndFile::getFilename() const {
	return this->path_parse.back();
}

const string SingleEndFile::getFilenameWithoutExt() const {
	string filename_without_ext = this->path_parse.back();
	string delimiter = ".";
	size_t pos = filename_without_ext.rfind(delimiter);
	filename_without_ext.erase(pos, filename_without_ext.length());
	return filename_without_ext;
}

const string SingleEndFile::getExt() const {
	string filename_without_ext = this->path_parse.back();
	string delimiter = ".";
	size_t pos = filename_without_ext.rfind(delimiter);
	return filename_without_ext.substr(pos+1, filename_without_ext.length());
}

FileType SingleEndFile::getFileType() const {
	return file_type;
}

const string& PairFiles::getIdentify() const {
	return identify;
}

const char& SingleEndFile::getSequenceDelimiter() const {
	return read_delimiter;
}

id_seq_type SingleEndFile::getIdxApp(id_seq_type idxFile) {
	//Uso find perchè se non inserito non è presente e non voglio occupar memoria per niente
	auto it = this->map_idxF_idxA.find(idxFile);
	if(it != this->map_idxF_idxA.end()) //trovato
		return it->second;
	else
		return 0;
}

SingleEndFile::Map_IdxFile_IdxApp::iterator SingleEndFile::beginMapID() {
	return this->map_idxF_idxA.begin();
}

SingleEndFile::Map_IdxFile_IdxApp::iterator SingleEndFile::endMapID() {
	return this->map_idxF_idxA.end();
}
