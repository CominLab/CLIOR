/*
 * Reassignment.h
 *
 *  Created on: 16/feb/2016
 *      Author: samuele
 */

#ifndef REASSIGNMENT_H_
#define REASSIGNMENT_H_

#include "../Input/FilesHandler.h"

class Reassignment_Deprecated {
public:
	typedef string specie;
	typedef string id_grp;
	typedef unordered_map<specie, uint64_t> ClassCount;
	typedef unordered_map<id_grp, ClassCount> GroupClassCount;
	typedef unordered_map<id_grp, specie> GroupClassMax;
	Reassignment_Deprecated(PairedEnd_G_C& files, string& dir_output);
	virtual ~Reassignment_Deprecated();

private:
	PairedEnd_G_C& files;
	string& dir_output;
	GroupClassCount grp_class_count; //Un solo contatore perché i gruppi sono creati contando tutti e due i file
	GroupClassMax grp_class_max; //Per ogni gruppo memorizza la specie massima
	ClassCount clark_class_count; //Un solo contatore perché conto il dataset totale (se paired-end contando i due file)
	ClassCount final_class_count; //Un solo contatore perché conto il dataset totale (se paired-end contando i due file)

	bool GetGroupsClassCount();
	bool GetGroupsClassCount_SingleEndFile(SingleEnd_G_C& file);
	bool GetGroupsClassCount_PairedEndFile(PairedEnd_G_C& files);

	void SaveGroupsClassMax();

	bool SaveGroupsClass();
	bool SaveGroupsClass_SingleEndFile(SingleEnd_G_C& file);
	void SaveClassCount();
};

#endif /* REASSIGNMENT_H_ */
