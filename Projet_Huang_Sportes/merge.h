#ifndef MERGE_H
#define MERGE_H

void affiche_commande();
WorkTree * mergeWorkTrees(WorkTree * wt1, WorkTree * wt2, List ** conflicts);
List * merge(char * remote_branch, char * message);
void createDeletionCommit(char * branch, List * conflicts, char * message);

#endif
