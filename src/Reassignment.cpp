/*
 * Reassignment2.cpp
 *
 *  Created on: 30/giu/2016
 *      Author: samuele
 */

#include "Reassignment.h"

Reassignment::Reassignment(PairedEnd_G_C& files, string& dir_output) :files(files), dir_output(dir_output) {
	this->last_index_read_in_grp = 0;
	this->last_index_read_in_cls = 0;
	bool state = this->get_Groups() && this->get_Class();
	state = this->last_index_read_in_cls == this->last_index_read_in_grp; //input non dello stesso numero
	if(!state)
	{
		cout<<endl<<"Input not contains the same number of reads"<<flush;
		abort();
	}
	//for info
	this->n_grp = 0;
	this->n_read = 0;
	this->n_grp_rnd = 0;
	this->n_read_rnd = 0;
	this->n_read_assign = 0;
	this->n_read_reassign = 0;
}

Reassignment::~Reassignment() {
}

void Reassignment::compute_and_save_clior(Mode_CLIOR m_clior) {
	this->get_grp_weight(m_clior.m_weight);
	this->get_grp_dividedClass(m_clior.m_win);
	this->save_class_clior(m_clior.m_assign);
}

bool Reassignment::get_Groups() {
	bool ret = this->get_single_file(this->files.first, this->pair_readsGroups.first, Groups);
	if(this->files.getType() == PairedEnd)
		ret = this->get_single_file(this->files.second, this->pair_readsGroups.second, Groups);
	return ret;
}

bool Reassignment::get_Class() {
	bool ret = this->get_single_file(this->files.first, this->pair_readsClass.first, Classification);
	if(this->files.getType() == PairedEnd)
		ret = this->get_single_file(this->files.second, this->pair_readsClass.second, Classification);
	return ret;
}

bool Reassignment::get_single_file(SingleEnd_G_C& file, Read_Class_One_File& map_cl, File file_type) {
	id_read idx = 0;
	string line;
	ifstream fileClass(file.getPath(file_type));
	if(fileClass.is_open())
	{
		if(file_type == Classification)
			getline(fileClass,line);//Scarta prima di intestazione
		while(getline(fileClass,line))
		{
			if(!line.empty())
			{
				vector<string> parse;
				parseLine(line, parse, {","});
				string value = parse.back();
				map_cl[idx] = value;
				if(file_type == Groups)
					this->last_index_read_in_grp = idx;
				if(file_type == Classification)
					this->last_index_read_in_cls = idx;
				++idx; //Primo indice è 0
			}
		}
		fileClass.close();
		return true;
	}
	else
	{
		if(!fileClass.is_open())
			cerr << endl << "Fail to open: " << file.getPath(file_type) << flush;
		return false;
	}
}

//posso ricavarmi sia le coppie di gruppi sia le coppie di classi basta cambiare pair_map
void Reassignment::get_pair(Pair_Read_Class_One_File& pair_map, id_read idx, Pair_Clas& pair) {
	auto it_class_first = pair_map.first.find(idx);
	auto it_class_second = pair_map.second.find(idx);
	if(it_class_first != pair_map.first.end())
		pair.first = it_class_first->second;
	if(it_class_second != pair_map.second.end())
		pair.second = it_class_second->second;
}

//Data una coppia di assegnazioni, le do un peso
void Reassignment::get_weight(Pair_Clas& pair_cls, Mode_Weight mode_weight, Pair_Weight& pair_weight) {
	switch (mode_weight)
	{
	case independent_weight:
		{
			if(pair_cls.first != "")
			{
				pair_weight.first.first = pair_cls.first;
				pair_weight.first.second = 1;
			}
			if(pair_cls.second != "")
			{
				pair_weight.second.first = pair_cls.second;
				pair_weight.second.second = 1;
			}
			break;
		}
	}
}

//Assegno peso alle specie nei gruppi
void Reassignment::get_grp_weight(Mode_Weight mode_weight) {
	cout << endl << "Weighs groups... " << flush;
	for(id_read idx = 0; idx <= this->last_index_read_in_cls; ++idx)
	{
		Pair_Clas pair_cls;
		this->get_pair(this->pair_readsClass, idx, pair_cls); //Prendo coppia di assegnazioni di specie

		Pair_Weight pair_weight;
		this->get_weight(pair_cls, mode_weight, pair_weight); //Assegno peso alle assegnazioni

		Pair_Clas pair_grp;
		this->get_pair(this->pair_readsGroups, idx, pair_grp); //Prendo coppia di assegnazioni di gruppi. I gruppi potrebbero esser diversi

		//TODO: Si può far qualcos'altro in caso di gruppi diversi
		//Incremento peso della specie nel gruppo
		if(pair_weight.first.first != "")
			this->grp_weight[pair_grp.first][pair_weight.first.first] += pair_weight.first.second;
		if(pair_weight.second.first != "")
			this->grp_weight[pair_grp.second][pair_weight.second.first] += pair_weight.second.second;
	}
	this->n_grp = this->grp_weight.size();
	cout << "Complete" << flush;
}

//Restituisce una lista di vincitori in base al metodo scelto, in più torna anche i perdenti e i non assegnati
void Reassignment::get_winner(Specie_Weight& specie_weight, Mode_Win mode_win, DividedClass& divided_class) {
	//Trova NA e loro peso
	auto na_it = specie_weight.find("NA");
	if(na_it != specie_weight.end())
		divided_class.NA = *na_it;

	//Crea vettore
	vector<Weight> all_elem(specie_weight.begin(), specie_weight.end());
	auto comparator = [](Weight& first, Weight& second) {
		return first.second > second.second; //Descending order
	};
	sort(all_elem.begin(), all_elem.end(), comparator); //Ordina

	//Elimina NA
	auto na = find_if(all_elem.begin(), all_elem.end(), [](Weight& w){return w.first == "NA";});
	if(na != all_elem.end())
		all_elem.erase(na);

	switch(mode_win)
	{
	case Max:
		{
			double max = all_elem[0].second;
			for(size_t i = 0; i < all_elem.size(); ++i)
			{
				//NA rimosso, non serve controllo if(all_elem[i].first != "NA")
				if(all_elem[i].second >= max)
					divided_class.winner.push_back(move(all_elem[i]));
				else
					divided_class.loser.push_back(move(all_elem[i]));
			}
			//imposta massimo random se più di uno
			divided_class.rnd = divided_class.winner.size() > 1;
			if(divided_class.rnd)
			{
				++this->n_grp_rnd;//for info
				/* initialize random seed: */
				srand (time(NULL));
				/* generate secret number between 0 and winner.size()-1: */
				divided_class.top_win = rand() % divided_class.winner.size();
			}
			else
				if(divided_class.winner.size() > 0)
					divided_class.top_win = 0;
			break;
		}
	}
}

void Reassignment::get_grp_dividedClass(Mode_Win mode_win) {
	cout << endl << "Get winners... " << flush;
	vector<Groups_Weight::key_type> vector_key(this->grp_weight.size());
	auto key_selector = [](Groups_Weight::value_type pair){return pair.first;};
	transform(this->grp_weight.begin(), this->grp_weight.end(), vector_key.begin(), key_selector);

	for(size_t grp = 0; grp < vector_key.size(); ++grp)
	{
		Specie_Weight& specie_weight = this->grp_weight[vector_key[grp]];
		DividedClass& divided_class = this->grp_dividedClass[vector_key[grp]];
		this->get_winner(specie_weight, mode_win, divided_class);
	}
	cout << "Complete" << flush;
}

void Reassignment::save(string& id_read, string& grp, string& prev_class,
		Mode_Assign mode_assign, ofstream& outFile) {
	DividedClass& div_class = this->grp_dividedClass[grp];
	string cls = "NA";
	switch(mode_assign)
	{
	case RWT_All:
		{
			cls = (div_class.winner.size() > 0) ? div_class.winner[div_class.top_win].first : "NA"; //Metti massimo
			break;
		}
	case RWT_NAssign:
		{
			cls = (div_class.winner.size() > 0) ? div_class.winner[div_class.top_win].first : "NA"; //Metti massimo
			cls = prev_class == "NA" ?  cls : prev_class; //controlla se precedente era NA, se si imposta nuovo valore max, altrimenti lascia precedente
			break;
		}
	}
	//for info
	if(div_class.rnd)
		++this->n_read_rnd;
	if(cls != "NA")
	{
		if(prev_class == "NA")
			++this->n_read_assign;
		else if (cls != prev_class)
			++this->n_read_reassign;
	}
	++this->n_read;
	//write output
	outFile << id_read << "," << cls << endl << flush;
}

void Reassignment::save_class_clior_single(SingleEnd_G_C& file, Read_Class_One_File& map_grp, Mode_Assign mode_assign) {
	string output_filename = file.getFilenameWithoutExt(Classification) + ".CLIOR." + file.getExt(Classification);
	ofstream outFile(this->dir_output + output_filename);
	id_read idx = 0;
	string line;
	ifstream fileCls(file.getPath(Classification));
	if(fileCls.is_open())
	{
		getline(fileCls,line); //Elimina intestazione
		while(getline(fileCls,line))
		{
			if(!line.empty())
			{
				vector<string> parse;
				parseLine(line, parse, {","});
				string& id_read = parse.front();
				string& grp = map_grp[idx];
				string& prev_class = parse.back();
				this->save(id_read, grp, prev_class, mode_assign, outFile);
				++idx; //Primo indice è 0
			}
		}
		fileCls.close();
	}
	else
	{
		if(!fileCls.is_open())
			cerr << endl << "Fail to open: " << file.getPath(Classification) << flush;
	}
}

void Reassignment::save_class_clior(Mode_Assign mode_assign) {
	cout << endl << "Save CLIOR... " << flush;
	this->save_class_clior_single(this->files.first, this->pair_readsGroups.first, mode_assign);
	if(this->files.getType() == PairedEnd)
		this->save_class_clior_single(this->files.second, this->pair_readsGroups.second, mode_assign);
	cout << "Complete" << flush;
}

void Reassignment::save_info_reassignment(){
	string output_filename = "CLIOR.info";
	ofstream outFile(this->dir_output + output_filename);
	outFile << "n_grp," << to_string(this->n_grp) << endl << flush;
	outFile << "n_read," << to_string(this->n_read) << endl << flush;
	outFile << "n_grp_rnd," << to_string(this->n_grp_rnd) << endl << flush;
	outFile << "n_read_rnd," << to_string(this->n_read_rnd) << endl << flush;
	outFile << "n_read_assign," << to_string(this->n_read_assign) << endl << flush;
	outFile << "n_read_reassign," << to_string(this->n_read_reassign) << endl << flush;
}
