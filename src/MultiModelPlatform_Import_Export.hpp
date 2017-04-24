#ifndef DEF_MULTIMODELPLATFORM_IMPORT_EXPORT
#define DEF_MULTIMODELPLATFORM_IMPORT_EXPORT


#include <libpq-fe.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <utility>

bool connexionBase();
void fermeConnection();
std::string retourneEtatConnection();
bool retourneEtatRequete(std::string typeRequete);
void executeRequete(std::string laRequete);
void videCurseurRes();
std::vector<std::string> decoupeLigneLueEnVecteur(std::string ligneLue);
int getNbResult();
int getNbColonnes();
std::string getValeurChampResultat(int ligne,int colonne);
std::string getValeurChampResultatParNom(std::string nomChamp,int ligne);

bool exportTablDansFichier(std::string nomFichier,std::string nomTable,std::string nomChampClePrimaire,std::string requete="",int nombreClePrimaire = 1,std::string nomChampClePrimaireDeux ="",std::string nomChampClePrimaireTrois = "",std::string nomChampClePrimaireQuatre ="");
	
void onExportSim_clicked(std::string idSimuSelected);

void onExportResultats_clicked(std::string idSimSelected);

std::vector<std::string> importFichierDansTable(std::string nomFichier,std::string nomTable,std::string nomChampClePrimaire,int nombreClePrimaire=1,std::string nomChampClePrimaireDeux="",std::string nomChampClePrimaireTrois="",std::string nomChampClePrimaireQuatre="");

#endif