#ifndef EXO11_H
#define EXO11_H

WorkTree * mergeWorkTrees(WorkTree * wt1, WorkTree * wt2, List ** conflicts);
WorkTree * branchToWorkTree(char * branch_name);
List * merge(char * remote_branch, char * message);
void createDeletionCommit(char * branch, List * conflicts, char * message);

#endif