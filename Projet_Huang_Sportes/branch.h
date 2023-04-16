#ifndef BRANCH_H
#define BRANCH_H

void initRefs();
void createUpdateRef(char* ref_name, char* hash);
void deleteRef(char* ref_name);
char * getRef(char* ref_name);
void myGitAdd(char* file_or_folder);
void myGitCommit(char* branch_name, char* message);
void initBranch();
int branchExists(char* branch);
void createBranch(char* branch);
char * getCurrentBranch();
void printBranch(char * branch);
List * branchList(char * branch);
List * getAllCommits();
void restoreCommit(char * hash_commit);
void myGitCheckoutBranch(char * branch);
void myGitCheckoutCommit(char* pattern);

#endif
