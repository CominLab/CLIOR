/*
 * FilesHandler.h
 *
 *  Created on: 28/giu/2016
 *      Author: samuele
 */

#ifndef FILESHANDLER_H_
#define FILESHANDLER_H_

#include "../Utilities.h"
#include "Input.h"

enum File {Groups, Classification};

class SingleEnd_G_C {
public:
	SingleEnd_G_C();
	bool init(string& path_groups, string& path_classification);

	const vector<string>& getParsePath(File type) const;
	const string& getPath(File type) const;
	const string getDirectory(File type) const;
	const string& getFilename(File type) const;
	const string getFilenameWithoutExt(File type) const;
	const string getExt(File type) const;
	bool isCorrect() const;
private:
	bool open_correct;
	string path_groups;
	string path_classification;
	vector<string> parse_path_groups;
	vector<string> parse_path_classification;
	void reset();
};

class PairedEnd_G_C : public pair<SingleEnd_G_C, SingleEnd_G_C>
{
public:
	PairedEnd_G_C();
	bool init(string& path_groups_1, string& path_classification_1, string& path_groups_2, string& path_classification_2);

	bool isCorrect(); //return if the file or files opened isCorrect
	PairType getType() const;
	const string& getIdentify() const;

private:
	PairType pair_type;
	void reset();
};
#endif /* FILESHANDLER_H_ */
