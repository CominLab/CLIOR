#ifndef SRC_INPUTCLASS_H_
#define SRC_INPUTCLASS_H_

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../Utilities.h"

enum FileType {Fasta, Fastq, UnknownFile};
enum PairType {SingleEnd, PairedEnd, UnknownPair};

typedef uint32_t id_seq_type;
typedef uint32_t id_cluster_type;
typedef uint16_t size_seq;
typedef uint16_t size_seq_tot;

using namespace std;

struct SingleEndFile
{
	typedef shared_ptr<SingleEndFile> Ptr;
	typedef unordered_map<id_seq_type, id_seq_type> Map_IdxFile_IdxApp;
	SingleEndFile();
	bool init(const string& path);
	bool isCorrect() const;
	void AddSequenceMap(id_seq_type idxFile, id_seq_type idxApp); //idFile is ordered read sequence number in file
	//Get
	const string& getPath() const;
	const vector<string>& getPathParse() const;
	const string getDirectory() const;
	const string& getFilename() const;
	const string getFilenameWithoutExt() const;
	const string getExt() const;
	FileType getFileType() const;
	const char& getSequenceDelimiter() const;
	id_seq_type getIdxApp(id_seq_type idxFile);
	Map_IdxFile_IdxApp::iterator beginMapID();
	Map_IdxFile_IdxApp::iterator endMapID();
	void reset();

private:
	string path;
	vector<string> path_parse;
	FileType file_type;
	char read_delimiter;
	bool open_correct;
	Map_IdxFile_IdxApp map_idxF_idxA;
};

struct PairFiles : public pair<SingleEndFile, SingleEndFile>
{
	PairFiles();
	void init(const string& path_end1, const string& path_end2);
	bool isCorrect(); //return if the file or files opened isCorrect
	PairType getType() const;
	const string& getIdentify() const;
	void reset();

private:
	PairType pair_type;
	string identify;
};

#endif /* SRC_INPUTCLASS_H_ */
