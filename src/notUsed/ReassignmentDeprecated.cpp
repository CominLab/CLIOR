/*
 * Reassignment.cpp
 *
 *  Created on: 16/feb/2016
 *      Author: samuele
 */

#include "ReassignmentDeprecated.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Reassignment_Deprecated::Reassignment_Deprecated(PairedEnd_G_C& files, string& dir_output):files(files),
													dir_output(dir_output) {
	this->GetGroupsClassCount();
	this->SaveGroupsClassMax();
	this->SaveGroupsClass();
	this->SaveClassCount();
}

Reassignment_Deprecated::~Reassignment_Deprecated() {
}

bool Reassignment_Deprecated::GetGroupsClassCount() {
	//Initialize Map
	GroupClassCount().swap(this->grp_class_count);

	bool ret = false;
	if(this->files.isCorrect())
	{
		if(this->files.getType() == SingleEnd)
			ret = this->GetGroupsClassCount_SingleEndFile(this->files.first);
		else if(this->files.getType() == PairedEnd)
			ret = this->GetGroupsClassCount_PairedEndFile(this->files);
	}
	return ret;
}

bool Reassignment_Deprecated::GetGroupsClassCount_SingleEndFile(SingleEnd_G_C& file) {
	string lineGroups;
	string lineClass;
	ifstream fileGroups(file.getPath(Groups));
	ifstream fileClass(file.getPath(Classification));
	if(fileGroups.is_open() && fileClass.is_open())
	{
		getline(fileClass,lineClass); //Leggi ed elimina prima riga
		//Prendi prima riga e guarda primo elemento per capire la tipologia di file
		while(getline(fileGroups,lineGroups) && getline(fileClass,lineClass))
		{
			if(!lineGroups.empty() && !lineClass.empty())
			{
				vector<string> parse_groups;
				parseLine(lineGroups, parse_groups, {","});
				string idGrp = parse_groups.back();

				vector<string> parse_class;
				parseLine(lineClass, parse_class, {","});
				string clas = parse_class.back();

				if(clas != "NA")
					++this->grp_class_count[idGrp][clas];
				++this->clark_class_count[clas]; //un elemento in più per questa specie nel dataset totale (unione dei due dataset se Paired-End)
			}
		}
		fileGroups.close();
		fileClass.close();
		return true;
	}
	else
	{
		if(!fileGroups.is_open())
			cerr << endl << "Fail to open: " << file.getPath(Groups) << flush;
		if(!fileClass.is_open())
			cerr << endl << "Fail to open: " << file.getPath(Classification) << flush;
		return false;
	}
}

bool Reassignment_Deprecated::GetGroupsClassCount_PairedEndFile(PairedEnd_G_C& files) {
	bool ret = true;
	ret &= this->GetGroupsClassCount_SingleEndFile(this->files.first);
	ret &= this->GetGroupsClassCount_SingleEndFile(this->files.second);
	return ret;
}

void Reassignment_Deprecated::SaveGroupsClassMax() {
	auto it_grp = this->grp_class_count.begin();
	while(it_grp != this->grp_class_count.end())
	{
		auto it_cls = it_grp->second.begin();
		auto it_cls_max = it_cls;
		while(it_cls != it_grp->second.end())
		{
			if(it_cls_max->second < it_cls->second)
				it_cls_max = it_cls;
			++it_cls;
		}
		this->grp_class_max[it_grp->first] = it_cls_max->first;
		++it_grp;
	}
}

bool Reassignment_Deprecated::SaveGroupsClass() {
	bool ret = false;
	if(this->files.isCorrect())
	{
		if(this->files.getType() == SingleEnd || this->files.getType() == PairedEnd)
			ret = this->SaveGroupsClass_SingleEndFile(this->files.first);
		if(this->files.getType() == PairedEnd)
			ret = this->SaveGroupsClass_SingleEndFile(this->files.second);
	}
	return ret;
}

bool Reassignment_Deprecated::SaveGroupsClass_SingleEndFile(SingleEnd_G_C& file) {
	string output_filename = file.getFilenameWithoutExt(Classification) + ".CLIOR." + file.getExt(Classification);
	string lineGroups;
	ifstream fileGroups(file.getPath(Groups));
	ofstream outFile(this->dir_output + output_filename);
	if(fileGroups.is_open())
	{
		//Prendi prima riga e guarda primo elemento per capire la tipologia di file
		while(getline(fileGroups,lineGroups))
		{
			if(!lineGroups.empty())
			{
				vector<string> parse_groups;
				parseLine(lineGroups, parse_groups, {","});

				string id_read = parse_groups.front().c_str();
				string id_grp = parse_groups.back().c_str();

				auto it_grp = this->grp_class_max.find(id_grp);
				if(it_grp != this->grp_class_max.end()) //Trovato
				{
					outFile << id_read << "," << it_grp->second << endl << flush;
					++this->final_class_count[it_grp->second];//un elemento in più per questa specie nel dataset totale (unione dei due dataset se Paired-End)
				}
				else //Non Trovato
				{
					outFile << id_read << "," << "NA" << endl << flush;
					++this->final_class_count["NA"];//un elemento in più per questa specie nel dataset totale (unione dei due dataset se Paired-End)
				}
			}
		}
		fileGroups.close();
		return true;
	}
	else
	{
		if(!fileGroups.is_open())
			cerr << endl << "Fail to open: " << file.getPath(Groups) << flush;
		return false;
	}
}

void Reassignment_Deprecated::SaveClassCount()
{
	if(this->files.isCorrect())
	{
		//Save clark abundance
		string filePath = this->dir_output + "stimate_abund_clark.csv";
		ofstream outFile_clark(filePath);
		outFile_clark << "id_specie,num_read" << endl << flush;
		auto it_specie = this->clark_class_count.begin();
		while(it_specie != this->clark_class_count.end())
		{
			outFile_clark << it_specie->first << "," << it_specie->second << endl << flush;
			++it_specie;
		}

		//Save CLIOR abundance
		filePath = this->dir_output + "stimate_abund_CLIOR.csv";
		ofstream outFile_CLIOR(filePath);
		outFile_CLIOR << "id_specie,num_read" << endl << flush;
		it_specie = this->final_class_count.begin();
		while(it_specie != this->final_class_count.end())
		{
			outFile_CLIOR << it_specie->first << "," << it_specie->second << endl << flush;
			++it_specie;
		}
	}
}
