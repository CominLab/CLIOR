#include "Reassignment.h"
using namespace std;

int main(int argc, char* argv[]) {
	string empty = "";
	string dir_output = "output/";

	PairedEnd_G_C input;
	FilesScan scans;
	Mode_CLIOR m_clior;
	vector<string> grp_cls_files;
	vector<string> d_files;
	for(int i=1;i<argc;i++)
	{
		if(strcmp(argv[i], "-si") == 0)
		{
			grp_cls_files.push_back(argv[++i]);
			grp_cls_files.push_back(argv[++i]);
			input.init(grp_cls_files[0], grp_cls_files[1], empty, empty);
			if(!input.isCorrect())
			{
				cerr<<"Please enter input files for single-end dataset: -si <AbsPathGroupsFile> <AbsPathClassificationFile>"<<endl<<flush;
				return 0;
			}
		}
		else if(strcmp(argv[i], "-pi") == 0)
		{
			grp_cls_files.push_back(argv[++i]);
			grp_cls_files.push_back(argv[++i]);
			grp_cls_files.push_back(argv[++i]);
			grp_cls_files.push_back(argv[++i]);
			input.init(grp_cls_files[0], grp_cls_files[1], grp_cls_files[2], grp_cls_files[3]);
			if(!input.isCorrect())
			{
				cerr<<"Please enter input files for paired-end dataset: -pi <AbsPathGroupsFile_1> <AbsPathClassificationFile_1> <AbsPathGroupsFile_2> <AbsPathClassificationFile_2>"<<endl<<flush;
				return 0;
			}
		}
		else if(strcmp(argv[i], "-dirOutput") == 0)
		{
			dir_output.assign(argv[++i]);
			if(dir_output == "")
			{
				cerr<<"Please enter an output directory if you specify -dirOutput"<<endl<<flush;
				return 0;
			}
		}
		else if(strcmp(argv[i], "-mod_weight") == 0)
		{
			size_t val = atoi(argv[++i]);
			if(val >= Last_Weight)
			{
				cerr<<"Please enter a weight mode correct: from 0 to "<< Last_Weight-1 << "if you specify -mod_weight"<<endl<<flush;
				return 0;
			}
			m_clior.m_weight = Mode_Weight(val);
		}
		else if(strcmp(argv[i], "-mod_win") == 0)
		{
			size_t val = atoi(argv[++i]);
			if(val >= Last_Win)
			{
				cerr<<"Please enter a win mode correct: from 0 to "<< Last_Win-1 << "if you specify -mod_win"<<endl<<flush;
				return 0;
			}
			m_clior.m_win = Mode_Win(val);
		}
		else if(strcmp(argv[i], "-mod_assign") == 0)
		{
			size_t val = atoi(argv[++i]);
			if(val >= Last_Assign)
			{
				cerr<<"Please enter a assign mode correct: from 0 to "<< Last_Assign-1 << "if you specify -mod_assign"<<endl<<flush;
				return 0;
			}
			m_clior.m_assign = Mode_Assign(val);
		}
	}
	createDirAndSubDir(dir_output);
	if(input.isCorrect())
	{
		Reassignment reass(input, dir_output);
		reass.compute_and_save_clior(m_clior);
		reass.save_info_reassignment();
	}

	return 0;
}
