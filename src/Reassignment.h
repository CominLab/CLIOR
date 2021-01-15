/*
 * Reassignment2.h
 *
 *  Created on: 30/giu/2016
 *      Author: samuele
 */

#ifndef REASSIGNMENT_H_
#define REASSIGNMENT_H_

#include "Input/FilesHandler.h"
#include "Input/FileScan.h"
#include <unordered_set>

enum Mode_Weight{independent_weight=0, Last_Weight};
enum Mode_Win {Max=0, Last_Win};
enum Mode_Assign{RWT_All=0, RWT_NAssign, Last_Assign};
struct Mode_CLIOR {
	Mode_CLIOR(Mode_Weight m_weight, Mode_Win m_win, Mode_Assign m_assign) {
		this->m_weight = m_weight;
		this->m_win = m_win;
		this->m_assign = m_assign;
	};
	Mode_CLIOR() : Mode_CLIOR(independent_weight,Max,RWT_All) {};
	Mode_Weight m_weight;
	Mode_Win m_win;
	Mode_Assign m_assign;
};

class Reassignment {
public:
	typedef string clas;
	typedef uint64_t id_read;

	typedef unordered_map<id_read, clas> Read_Class_One_File;
	typedef pair<Read_Class_One_File,Read_Class_One_File> Pair_Read_Class_One_File;

	typedef pair<clas,clas> Pair_Clas;

	typedef pair<clas,double> Weight;
	typedef pair<Weight,Weight> Pair_Weight;

	typedef unordered_map<clas, double> Specie_Weight;
	typedef unordered_map<clas, Specie_Weight> Groups_Weight;//Divided from here

	typedef vector<Weight> Weight_Vector;
	struct DividedClass
	{
		bool rnd = false;
		size_t top_win = numeric_limits<size_t>::max();
		Weight_Vector winner;
		Weight_Vector loser;
		Weight NA;
	};
	typedef unordered_map<clas,DividedClass> Group_DividedClass;//Divided into winner, loser and NA

	Reassignment(PairedEnd_G_C& files, string& dir_output);
	virtual ~Reassignment();

	void compute_and_save_clior(Mode_CLIOR m_clior = Mode_CLIOR());
	void save_info_reassignment();

private:
	PairedEnd_G_C& files;
	string& dir_output;
	id_read last_index_read_in_grp;
	id_read last_index_read_in_cls;
	Pair_Read_Class_One_File pair_readsGroups;
	Pair_Read_Class_One_File pair_readsClass;

	Groups_Weight grp_weight;
	Group_DividedClass grp_dividedClass;

	//info
	id_read n_grp;
	id_read n_read;
	id_read n_grp_rnd;
	id_read n_read_rnd;
	id_read n_read_assign;
	id_read n_read_reassign;

	bool get_Groups();
	bool get_Class();
	bool get_single_file(SingleEnd_G_C& file, Read_Class_One_File& map_cl, File file_type);

	void get_pair(Pair_Read_Class_One_File& pair_map, id_read idx, Pair_Clas& pair);
	void get_weight(Pair_Clas& pair_cls, Mode_Weight mode_weight, Pair_Weight& pair_weight);
	void get_grp_weight(Mode_Weight mode_weight);

	void get_winner(Specie_Weight& specie_weight, Mode_Win mode_assign, DividedClass& divided_class);
	void get_grp_dividedClass(Mode_Win mode_win);

	void save(string& id_read, string& grp, string& prev_class, Mode_Assign mode_assign, ofstream& outFile);
	void save_class_clior_single(SingleEnd_G_C& file, Read_Class_One_File& map_grp, Mode_Assign mode_assign);
	void save_class_clior(Mode_Assign mode_assign);
};

#endif /* REASSIGNMENT_H_ */
