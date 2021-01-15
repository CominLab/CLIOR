/*
 * FileScan.h
 *
 *  Created on: 09/feb/2016
 *      Author: samuele
 */

#ifndef UTILS_FILESCAN_H_
#define UTILS_FILESCAN_H_

#include "Input.h"
#include <map>
#include "Sequence.h"

struct PositionRead
{
	streampos start;
	streampos end;
};

class FileScan {
public:
	typedef Sequence::Parser Parser;

	FileScan();
	virtual ~FileScan();

	bool init(const string& path);

	size_t getSequenceNumber() const;
	bool getSequenceWithIndex(size_t index, Sequence& read, Parser parserHeader = NULL);
	const SingleEndFile& getFile() const;
	bool isCorrect() const; //return if the file or files opened isCorrect
	void reset();


private:
	SingleEndFile file;
	ifstream stream;
	vector<PositionRead> pos_read;

	bool getStreamPosRead(const string& path);
};

class FilesScan : public pair<FileScan, FileScan>
{
public:
	FilesScan();
	bool init(const string& path_end1, const string& path_end2);
	virtual ~FilesScan();
	size_t getPairedReadsNumber() const;
	size_t getSequencesNumber() const;
	const string& getIdentify() const;
	PairType getType() const;
	bool isCorrect() const; //return if the file or files opened isCorrect
	void reset();

private:
	PairType pair_type;
	string identify;
	};

#endif /* UTILS_FILESCAN_H_ */
