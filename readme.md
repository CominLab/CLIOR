#CLIOR

#Installation
##Download CLIOR
[CLIOR](https://bitbucket.org/samu661/clior/downloads/CLIOR_v1.tar.gz)  

##Compilation:
Open terminal and go to CLIOR/Release/ and then:  
make all  

#Algorithm Option
In the following paragraph is described the input file's structure and the parameters available in the CLIOR algorithm.  

##File accepted and structure
In the following paragraph is described the input file's structure.  
CLIOR to execute correctly needs two kind of file, one in which reads are divided into groups and another in which reads are classified.  
  
For groups file the structure is the following:  
> READ_IDENTIFICATION,GROUP  
  
To obtain this file call MetaProb with -mg option, and if possible use paired-end files.
  
For classification file the structure is the following:  
> READ_IDENTIFICATION,Length,Class <first line is a colums description line>  
> READ_IDENTIFICATION,Length,Class (first read)  
  
To obtain this file use Clark-l algorithm, or other software and convert the output in this format.  
ATTENTION: First line is a identification line with 3 colums in that order  
  
##Parameter
**-si** File paths of groups and classification for single-end dataset. Ex. -si pathgroups pathclass  
**-pi** File paths of groups and classification for paired-end dataset. Ex. -pi pathgroups1 pathclass1 pathgroups2 pathclass2  
**-dirOutput** Path directory output files. Default: output/  

#Run
CLIOR needs to compute groups and classification of read datasets in a certain structure of file (**See** file accepted and structure). You need to download two software which can do this without translate outputs and these are MetaProb and Clark.  
  
Calls algorithm where is compiled:  
./CLIOR -si pathfilegroups/example.groups.csv pathfileclass/example.csv  
./CLIOR -pi pathfilegroups/example1.groups.csv pathfileclass/example1.csv pathfilegroups/example2.groups.csv pathfileclass/example2.csv  
./CLIOR -pi pathfilegroups/example1.groups.csv pathfileclass/example1.csv pathfilegroups/example2.groups.csv pathfileclass/example2.csv  

##Download MetaProb
[MetaProb](https://bitbucket.org/samu661/metaprob/overview)  

##Download Clark
[Clark](http://clark.cs.ucr.edu/Tool/) 

#Test File

[Paired-End_split_dataset_part01](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part01.rar)  
[Paired-End_split_dataset_part02](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part02.rar)  
[Paired-End_split_dataset_part03](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part03.rar)  
[Paired-End_split_dataset_part04](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part04.rar)  
[Paired-End_split_dataset_part05](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part05.rar)  
[Paired-End_split_dataset_part06](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part06.rar)  
[Paired-End_split_dataset_part07](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part07.rar)  
[Paired-End_split_dataset_part08](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part08.rar)  
[Paired-End_split_dataset_part09](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part09.rar)  
[Paired-End_split_dataset_part10](https://bitbucket.org/samu661/metaprob/downloads/paired_end_dataset_splitted.part10.rar)  

[HiSeq_MiSeq_Simba5_part01](https://bitbucket.org/samu661/clior/downloads/HiSeq_MiSeq_Simba5.part01.rar)  
[HiSeq_MiSeq_Simba5_part02](https://bitbucket.org/samu661/clior/downloads/HiSeq_MiSeq_Simba5.part02.rar)  
[HiSeq_MiSeq_Simba5_part03](https://bitbucket.org/samu661/clior/downloads/HiSeq_MiSeq_Simba5.part03.rar)  
[HiSeq_MiSeq_Simba5_part04](https://bitbucket.org/samu661/clior/downloads/HiSeq_MiSeq_Simba5.part04.rar)  

#Publication
S.Girotto, M.Comin, C.Pizzi
*Higher recall in metagenomic sequence classification exploiting overlapping reads*  
BMC Genomics 2017, 18 (Suppl 10) :917  
DOI: https://doi.org/10.1186/s12864-017-4273-6  

#License
[MIT](https://bitbucket.org/samu661/clior/src/master/license.md)