#include "MultiModelPlatform_Import_Export.hpp"

const std::string nomHost="host=localhost",numPort=" port=5432",nomDB=" dbname=samara",nomUser=" user=user_samara",valeurPassword=" password=samarapassword";
const std::string chaineConnexion=nomHost+numPort+nomDB+nomUser+valeurPassword;

std::string nomIdParc="idparcelle",nomIdVar="idvariete",nomIdItinTech="iditinerairetechnique",nomIdTypET="idtypeet",
            nomCodeStation="codestation",nomCodeStatMetSim="codestationmeteo",nomCodeStatPrecSim="codestationpluie",nomTypStatMet="Meteorologique",
            nomTypStatPluv="Pluviometrique",nomTypStatSyno="Synoptique",nomIdSite="idsite",nomCodeTypSol="codetypesol",nomCodeConti="codecontinent",
            nomCodePays="codepays",nomIdDoss="iddossier",nomIdSim="idsimulation",nomCodeTypStat="codetypestation",
            nomMotCleId="id",nomMotCleCode="code",nomMotCleDate="date",nomChampNom="nom",
            nomAliasParc="par",nomAliasPays="pay",nomAliasSite="sit",nomAliasSim="sim",
            nomAliasStat="sta",nomAliasTypStat="tst",nomJour="jour",nomET="et",nomTMax="tmax",nomTMin="tmin",nomTMoy="tmoy",nomHMax="hmax",
            nomHMin="hmin",nomHMoy="hmoy",nomVt="vt",nomIns="ins",nomRg="rg",nomDateFnSim="datefin",nomDescrDossier="description",nomDateDebSim="datedebut",
            nomAnneeDebSim="anneedebut",nomAnneeFnSim="anneefin",nomNbAnneeSim="simulationyearnumber",nomTabParc="parcelle",
            nomTabVar="variete",nomTabItinTech="itinerairetechnique",nomTabTypET="typeet",nomTabStation="station",
            nomTabMet="meteo",nomTabPrecip="precipitation",nomTabSite="site",nomTabTypSol="typesol",nomTabConti="continent",nomTabPays="pays",
            nomTabObs="observer",nomTabIrrig="irriguer",nomTabDoss="dossier",nomTabSim="simulation",nomTabTypStat="typestation",nomTabBilETO="bilaneto",
            nomTabResultat="resultat";

PGconn *laConnex;//connexion a la bdd postgres
PGresult *leResult;//objet resultat renvoyé apres execution d'une commande


bool connexionBase(){
    bool retourConnex;
    laConnex = PQconnectdb(chaineConnexion.c_str());
    if (PQstatus(laConnex) != CONNECTION_OK) { //test si la connection a �t� �tablie
        fermeConnection();//fermeture de la connection
        retourConnex = false;
    }
    else retourConnex= true;
    return retourConnex;
}

void fermeConnection(){
    PQfinish(laConnex);//fermeture de la connexion avec la base postgres
}

std::string retourneEtatConnection(){
    std::string mssgEtatConnect;
    //test le statut de la connexion
    if (PQstatus(laConnex) != CONNECTION_OK)
        mssgEtatConnect="Connection finished";
    else
        mssgEtatConnect="Connection established";
    return mssgEtatConnect;
}

bool retourneEtatRequete(std::string typeRequete){
    bool retourReq;
    if (typeRequete == "select"){
        if (PQresultStatus(leResult) != PGRES_TUPLES_OK){//test sur le statut du res pour voir si commande bien execute
            PQclear(leResult); //vide le PGResult
            retourReq= false;//var retour a false
        }
        else retourReq= true;
	}
	if (typeRequete=="commandeMaj"){
        if (PQresultStatus(leResult) != PGRES_COMMAND_OK){//test sur le statut du res pour voir si commande bien execute
            PQclear(leResult); //vide le PGResult
            retourReq= false;//var retour a false
        }
        else retourReq= true;
    }
    return retourReq;
}

void executeRequete(std::string laRequete){
    leResult = PQexec(laConnex,laRequete.c_str());//execution de la requete sur le curseur principal
}

void videCurseurRes(){
    
    PQclear(leResult);//vide la struct res principale
}

std::vector<std::string> decoupeLigneLueEnVecteur(std::string ligneLue){
    std::string valeurColonne,ligneTronque;
    std::size_t positionTabTrouve;
    std::vector<std::string> tabResLigneLue;

    positionTabTrouve = ligneLue.find("\t");//recherche la premiere tabulation
    if (positionTabTrouve!=std::string::npos){//si tab trouv� alors on peut traiter la ligne
        valeurColonne = ligneLue.substr(0,positionTabTrouve);//extraction de la colonne
        ligneTronque = ligneLue.substr(positionTabTrouve+1,ligneLue.length()-1);//on enleve la partie que l'on vient de traiter
        tabResLigneLue.push_back(valeurColonne);//ajout au tableau qui sera retourn� en r�sultat
        positionTabTrouve = ligneTronque.find("\t");//recherche du tab suivant
        while (positionTabTrouve!=std::string::npos){//boucle tant qu'on trouve des tab
            valeurColonne = ligneTronque.substr(0,positionTabTrouve);
            ligneTronque = ligneTronque.substr(positionTabTrouve+1,ligneTronque.length()-1);
            tabResLigneLue.push_back(valeurColonne);
            positionTabTrouve = ligneTronque.find("\t");
        }
        valeurColonne = ligneTronque.substr(0,ligneTronque.length());//on traite ici la derniere colonne
        tabResLigneLue.push_back(valeurColonne);
    }
    return tabResLigneLue;
}

int getNbResult(){
    int nb_enreg;
    nb_enreg = PQntuples(leResult);//retourne le nbre d enreg du curseur resultat principal
    return nb_enreg;
}

int getNbColonnes(){
    int nb_col;
	nb_col = PQnfields(leResult);//retourne le nbre d enreg du curseur resultat principal
    return nb_col;
}

std::string getValeurChampResultat(int ligne,int colonne){
    std::string valeurChampRes;
    valeurChampRes = PQgetvalue(leResult,ligne,colonne);//retourne la valeur d'un champ du curseur principal
    return valeurChampRes;
}

std::string getValeurChampResultatParNom(std::string nomChamp,int ligne){
    std::string valeurChampRes;
    int numColonneChamp;
    numColonneChamp=PQfnumber(leResult,nomChamp.c_str());
    valeurChampRes = PQgetvalue(leResult,ligne,numColonneChamp);//retourne la valeur d'un champ du curseur principal
    
	return valeurChampRes;
}

bool exportTablDansFichier(std::string nomFichier,std::string nomTable,std::string nomChampClePrimaire,std::string requete,int nombreClePrimaire,std::string nomChampClePrimaireDeux,std::string nomChampClePrimaireTrois,std::string nomChampClePrimaireQuatre){
    std::string ligneAEcrire,requeteExport,valeurChamp;
    std::ofstream fichierExport(nomFichier.c_str(), std::ios::out | std::ios::trunc); //declaration de flux sortie en ecriture
    Glib::ustring valeurID;
    if (fichierExport){//si la creation du fichier est ok

        executeRequete(requete);//exectution de la requete
        if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
            return  false;
        }
        for(int j=0;j<getNbColonnes();j++){//boucle sur le resultat de la requete precedente (nom des colonnes de la table)
            if (j == 0)
                ligneAEcrire = PQfname(leResult,j);
            else //recupere et concatene le nom des champs
                ligneAEcrire = ligneAEcrire + "\t" + PQfname(leResult,j);
        }
        fichierExport << ligneAEcrire << std::endl;//ecriture dans le fichier
        ligneAEcrire = "";//remise a zero de la ligneaecrire
        for(int i=0;i<getNbResult();i++){//boucle sur le nombre de ligne du resultat de la requete
            for(int j=0; j<getNbColonnes();j++){//boucle sur le nombre de colonne du resultat de la requete
                valeurChamp = getValeurChampResultat(i,j);//recupere la valeur du champ
                if (j == 0)
                    ligneAEcrire = valeurChamp;
                else //recupere et concatene les valeurs des champs de l enregistrement
                    ligneAEcrire = ligneAEcrire + "\t" + valeurChamp;
            }
            fichierExport << ligneAEcrire << std::endl;//ecriture dans le fichier
            ligneAEcrire = "";//vide la ligneaecrire
        }
        videCurseurRes();//vide le curseur pr�c�demment utilis�

        fichierExport.close();//fermeture du fichier
        return true;
    }
    else{
        Gtk::MessageDialog dialogue("Error while creating file " + nomFichier + ".", false, Gtk::MESSAGE_INFO);
        dialogue.run();
        return false;
    }
}

void onExportSim_clicked(std::string idSimuSelected){
    bool retourImport = true;

   	Glib::ustring requeteDebutSimu = "SELECT * FROM " + nomTabSim + " WHERE " + nomIdSim + "='";
	Glib::ustring requeteFinSimu = "'";
	Glib::ustring requeteCompleteSimu = requeteDebutSimu + idSimuSelected + requeteFinSimu;

	executeRequete(requeteCompleteSimu);//execution de la commande sur la connexion connex
	if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
		afficheRetourRequete();
	}

	idParcelle = getValeurChampResultatParNom(nomIdParc);
	idVariete = getValeurChampResultatParNom(nomIdVar);
	idItinTech = getValeurChampResultatParNom(nomIdItinTech);
	codeStationMeteoSimu = getValeurChampResultatParNom(nomCodeStatMetSim);
	codeStationPluvio = getValeurChampResultatParNom(nomCodeStatPrecSim);
	dateDebutComplet = getValeurChampResultatParNom(nomDateDebSim);//recupere la date complete YYYY-MM-DD
	dateFinComplet = getValeurChampResultatParNom(nomDateFnSim);
	idDossier = getValeurChampResultatParNom(nomIdDoss);

	std::string requeteParcelle = "SELECT * FROM " + nomTabParc + " WHERE " +
									nomIdParc + "='" + idParcelle + "'";
	executeRequete(requeteParcelle);//execution de la commande sur la connexion connex
	if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
		afficheRetourRequete();
	}
	codeTypSol = getValeurChampResultatParNom(nomCodeTypSol);
	idSite = getValeurChampResultatParNom(nomIdSite);

	std::string requeteSite = "SELECT * FROM " + nomTabSite + " WHERE " +
							nomIdSite + "='" + idSite + "'";
	executeRequete(requeteSite);//execution de la commande sur la connexion connex
	if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
		afficheRetourRequete();
	}
	codePays = getValeurChampResultatParNom(nomCodePays);

	std::string requetePays = "SELECT * FROM " + nomTabPays + " WHERE " + nomCodePays + "='" + codePays + "'";
	executeRequete(requetePays);//execution de la commande sur la connexion connex
	if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
		afficheRetourRequete();
	}
	codeContinent = getValeurChampResultatParNom(nomCodeConti);

	std::string requeteSol = "SELECT * FROM " + nomTabTypSol + " WHERE " +
							nomCodeTypSol + "='" + codeTypSol + "'";
	retourImport = exportTablDansFichier("Sol.txt",nomTabTypSol,
																 nomCodeTypSol,requeteSol);
	std::string requeteContinent = "SELECT * FROM " + nomTabConti + " WHERE " +
									nomCodeConti + "='" + codeContinent + "'";
	retourImport = exportTablDansFichier("Continent.txt",nomTabConti,
																 nomCodeConti,requeteContinent);
	retourImport = exportTablDansFichier("Pays.txt",nomTabPays,
																 nomCodePays,requetePays);
	retourImport = exportTablDansFichier("Site.txt",nomTabSite,
																 nomIdSite,requeteSite);
	retourImport = exportTablDansFichier("Parcelle.txt",nomTabParc,
																 nomIdParc,requeteParcelle);
	std::string requeteStation = "SELECT * FROM " + nomTabStation + " WHERE " +
								nomCodeStation + "='" + codeStationMeteoSimu + "'";
	retourImport = exportTablDansFichier("Station.txt",nomTabStation,
																 nomCodeStation,requeteStation);
	std::string requeteVariete = "SELECT * FROM " + nomTabVar + " WHERE " +
								nomIdVar + "='" + idVariete + "'";
	retourImport = exportTablDansFichier("Variete.txt",nomTabVar,
																 nomIdVar,requeteVariete);
	std::string requeteItineraireTech = "SELECT * FROM " + nomTabItinTech + " WHERE " +
										nomIdItinTech + "='" + idItinTech + "'";
	retourImport = exportTablDansFichier("Itineraire_Technique.txt",
																nomTabItinTech,nomIdItinTech,requeteItineraireTech);
	std::string requeteIrrigation = "SELECT * FROM " + nomTabIrrig + " WHERE " +
									nomIdParc + "='" + idParcelle + "' AND " + nomJour + ">='" + dateDebutComplet + "' AND " + nomJour + "<='" + dateFinComplet + "' ORDER BY " + nomJour;
	retourImport = exportTablDansFichier("Irrigation.txt",
																 nomTabIrrig,nomIdParc,requeteIrrigation);
	std::string requeteMeteo = "SELECT * FROM " + nomTabMet + " WHERE " +
								nomCodeStation + "='" + codeStationMeteoSimu + "' AND " + nomJour + ">='" + dateDebutComplet + "' AND " + nomJour + "<='" + dateFinComplet + "' ORDER BY " + nomCodeStation + "," + nomJour;
	retourImport = exportTablDansFichier("Meteo.txt",nomTabMet,
																 nomCodeStation,requeteMeteo);
	std::string requetePluie = "SELECT * FROM " + nomTabPrecip + " WHERE " +
								nomCodeStation + "='" + codeStationPluvio + "' AND " + nomJour + ">='" + dateDebutComplet + "' AND " + nomJour + "<='" + dateFinComplet + "' ORDER BY " + nomCodeStation + "," + nomJour;
	retourImport = exportTablDansFichier("Pluie.txt",nomTabPrecip,
																 nomCodeStation,requetePluie);
	std::string requeteObs = "SELECT * FROM " + nomTabObs + " WHERE " +
								nomIdParc + "='" + idParcelle + "' AND " +
								nomIdVar + "='" + idVariete + "' AND " +
								nomIdItinTech + "='" + idItinTech + "' AND " +
								nomJour + ">='" + dateDebutComplet + "' AND " +
								nomJour + "<='" + dateFinComplet + "' ORDER BY " +
								nomIdParc + "," + nomIdVar + "," +
								nomIdItinTech + "," + nomJour;
	retourImport = exportTablDansFichier("ValObs.txt",nomTabObs,
																 nomIdParc,requeteObs,4,
																 nomIdVar,nomIdItinTech,nomJour);
	retourImport = exportTablDansFichier("Simulation.txt",nomTabSim,
																 nomIdSim,requeteCompleteSimu);

	if (!(retourImport)){
		Gtk::MessageDialog dialogueErreurImport(*this,"Export problem",false,Gtk::MESSAGE_WARNING);
		dialogueErreurImport.run();
	}
	Gtk::MessageDialog messageActionTerminee(*this,"Simulation's export ended", false);
	messageActionTerminee.run();
        
    
}

void onExportResultats_clicked(std::string idSimSelected){
    std::string  nomFichier;
    bool retourImport;

    std::string tableResSelectionnee, requeteFrom=" FROM ", conditionWhere = " WHERE ";

	std::string requeteResultat = "SELECT *";
	Gtk::FileChooserDialog dialogueChoixFichResult(*this,"Please choose a text file to export result data",Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialogueChoixFichResult.set_transient_for(*this);
	//Demande de confirmation lors du remplacement d'un fichier existant
	dialogueChoixFichResult.set_do_overwrite_confirmation();
	//Permettre la cr?ation de dossiers
	dialogueChoixFichResult.set_create_folders();
	//ajout des boutons de choix de la boite de dialogue
	dialogueChoixFichResult.add_button("CANCEL", Gtk::RESPONSE_CANCEL);
	dialogueChoixFichResult.add_button("SAVE", Gtk::RESPONSE_OK);
	//ajout de filtre, seul les fichiers textes peuvent etre selectionnes
	Glib::RefPtr<Gtk::FileFilter>  filtreFichTexte = Gtk::FileFilter::create();
	filtreFichTexte->set_name("Text files");
	filtreFichTexte->add_mime_type("text/plain");
	dialogueChoixFichResult.add_filter(filtreFichTexte);
	//affiche la boite et attend reponse de l utilisateur
	int resultat = dialogueChoixFichResult.run();
	//gestion de la reponse
	if (resultat == Gtk::RESPONSE_OK){
		//recupere le nom du fichier selectionne
		nomFichier = dialogueChoixFichResult.get_filename();
		if (!(nomFichier.empty())){//test si un fichier a bien ete selectionne
			requeteFrom = requeteFrom + nomTabResultat;
			for(unsigned int i=0;i<vectorSimuSelectionnee.size();i++){
				conditionWhere = conditionWhere + nomIdSim + "='" +
				idSimSelected + "'";
			}
			requeteResultat = requeteResultat + requeteFrom + conditionWhere;

			retourImport = exportTableDansFichier(nomFichier,nomTabResultat,nomIdSim,requeteResultat,2,nomJour);
		}
		if (!(retourImport)){
			Gtk::MessageDialog dialogueErreurImport(*this,"Export problem",false,Gtk::MESSAGE_WARNING);
			dialogueErreurImport.run();
		}
	}

}

std::vector<std::string> importFichierDansTable(std::string nomFichier,std::string nomTable,std::string nomChampClePrimaire,int nombreClePrimaire,std::string nomChampClePrimaireDeux,std::string nomChampClePrimaireTrois,std::string nomChampClePrimaireQuatre){

    std::ifstream fichierLu(nomFichier.c_str(), std::ios::in); //declaration de flux ouverture en lecture
    std::string ligneLue,motCleInsertInto="INSERT INTO ",motCleInsertValues=" VALUES ",carSeparCol=",";
    std::string requeteComplete,requeteListeColonne,requeteValeurColonne,requeteTestExistant;
    std::size_t positionChaineTrouve;
    std::vector<std::string> tabLigneLue,tabLigneEntete,tabARetourner;
    std::vector<int> tabIndiceColValide;
    int nbChampCleTrouve = 0;

    if(fichierLu) { //si le fichier existe et est ouvert
        std::getline(fichierLu, ligneLue);//lecture de la premiere ligne
        switch (nombreClePrimaire)//selon nbre de cle primaire
        {
            case 1 :
                positionChaineTrouve = ligneLue.find(nomChampClePrimaire);
                if (positionChaineTrouve!=std::string::npos)//test si la cle se trouve dans la ligne d entete
                    nbChampCleTrouve++;//incremente le nbre de cle trouve
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }

                break;

            case 2 :
                positionChaineTrouve = ligneLue.find(nomChampClePrimaire);
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }
                positionChaineTrouve = ligneLue.find(nomChampClePrimaireDeux);//test sur la presence de la deuxieme cle primaire
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }

                break;

            case 3 :
                positionChaineTrouve = ligneLue.find(nomChampClePrimaire);
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }
                positionChaineTrouve = ligneLue.find(nomChampClePrimaireDeux);
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }
                positionChaineTrouve = ligneLue.find(nomChampClePrimaireTrois);//test sur la presence de la troisieme cle primaire
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }

                break;

            case 4 :
                positionChaineTrouve = ligneLue.find(nomChampClePrimaire);
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }
                positionChaineTrouve = ligneLue.find(nomChampClePrimaireDeux);
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }
                positionChaineTrouve = ligneLue.find(nomChampClePrimaireTrois);
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }
                positionChaineTrouve = ligneLue.find(nomChampClePrimaireQuatre);//test sur la presence de la quatrieme cle primaire
                if (positionChaineTrouve!=std::string::npos)
                    nbChampCleTrouve++;
                else {
                    tabARetourner.push_back("probleme");
                    return tabARetourner;
                }

                break;

        }



        if (nombreClePrimaire != nbChampCleTrouve){//si le nbre de cle ne correspond pas
            tabARetourner.push_back("probleme");
            return tabARetourner;                        //on retourne false
        }



        tabLigneEntete = decoupeLigneLueEnVecteur(ligneLue);//decoupe la chaine de caractere contenant la ligne complete dans un tableau
                                                            //ce qui facilitera les traitements futurs pour la g�n�ration dynamqiue de la requete


        //requete permettant de recuperer le nom de tous les champs de la table
        executeRequete("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='" + nomTable + "'");
        if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
            tabARetourner.push_back("probleme");
            return tabARetourner;
        }
        for(std::size_t j = 0;j<tabLigneEntete.size();j++){//boucle sur les colonnes indiqu�es dans l entete du fichier
            for(int i = 0; i<getNbResult();i++){//boucle sur le resultat de la requete precedente (nom des colonnes de la table)
                if (tabLigneEntete[j] == getValeurChampResultat(i,0)){//si correspondance entre fichier et table
                    tabIndiceColValide.push_back(j);//alors on ajoute indice ou se trouve la colonne dans le tab d entete
                    break;
                }
            }
        }
        videCurseurRes();//vide le curseur pr�c�demment utilis�




        while(std::getline(fichierLu, ligneLue)) {//lecture ligne a ligne
            tabLigneLue = decoupeLigneLueEnVecteur(ligneLue);//decoupe la ligne de donn�es
            //pour chaque lignelue on va tester si l'occurrence est d�j� pr�sente
            requeteTestExistant = "SELECT * FROM " + nomTable + " WHERE ";//init de la requete de test d'existence de l'occurrence
            for (int indice=0;indice<nombreClePrimaire-1;indice++){//parcours des cl� primaires
                requeteTestExistant = requeteTestExistant + tabLigneEntete[indice] + "='" + tabLigneLue[indice] + "' AND ";
            } //on traite tous les identifiants et leur valeur
            //on s'occupe ici du dernier identifiant
            requeteTestExistant = requeteTestExistant + tabLigneEntete[nombreClePrimaire-1] + "='" + tabLigneLue[nombreClePrimaire-1] + "'";

            executeRequete(requeteTestExistant);//execution de la requete
            if (!(retourneEtatRequete("select"))){//test si l'execution c'est bien d�roul�e
                tabARetourner.push_back("probleme");
                return tabARetourner;
            }
            if (getNbResult() < 1){//s'il n'y a pas d enregistrement deja existant
                //on utilise insert
                requeteComplete = motCleInsertInto + nomTable;
                //parcours du tableau des indices de colonnes valides
                for (std::size_t k=0;k<tabIndiceColValide.size();k++){

                    if (tabLigneLue[tabIndiceColValide[k]].empty()) //si le champ est vide
                        tabLigneLue[tabIndiceColValide[k]] = "NULL"; //alors on affecte la valeur nulle
                    if (k==0){//cas particulier du traitement du premier champ de l'enregistrement
                        requeteListeColonne = tabLigneEntete[tabIndiceColValide[k]];//on traite ici la liste des colonnes
                        //test si le champ est une chaine de caractere grace a son nom
                        if (tabLigneEntete[tabIndiceColValide[k]].substr(0,2) == nomMotCleId || //find() != std::string::npos
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomMotCleCode ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomJour ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomMotCleDate ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,3) == nomChampNom ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,11) == nomDescrDossier)
                            if (tabLigneLue[tabIndiceColValide[k]] != "NULL")
                                requeteValeurColonne = "'" + tabLigneLue[tabIndiceColValide[k]] + "'";//gestion de la valeur des colonnes
                            else requeteValeurColonne = tabLigneLue[tabIndiceColValide[k]];
                        else requeteValeurColonne = tabLigneLue[tabIndiceColValide[k]];
                    } //avec l indice k on recupere a la fois le nom de la colonne et sa valeur
                    else{
                        //traitement du nom des colonnes
                        requeteListeColonne = requeteListeColonne + carSeparCol + tabLigneEntete[tabIndiceColValide[k]];
                        //traitement de la valeur des colonnes
                        if (tabLigneEntete[tabIndiceColValide[k]].substr(0,2) == nomMotCleId || //find() != std::string::npos
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomMotCleCode ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomJour ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomMotCleDate ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,3) == nomChampNom ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,11) == nomDescrDossier)
                            if (tabLigneLue[tabIndiceColValide[k]] != "NULL")
                                requeteValeurColonne = requeteValeurColonne + carSeparCol + "'" + tabLigneLue[tabIndiceColValide[k]] + "'";
                            else requeteValeurColonne = requeteValeurColonne + carSeparCol + tabLigneLue[tabIndiceColValide[k]];
                        else requeteValeurColonne = requeteValeurColonne + carSeparCol + tabLigneLue[tabIndiceColValide[k]];
                    }
                }//on concatene listecolonne et liste valeur pour former la requete dans son ensemble
                requeteComplete = requeteComplete + " (" + requeteListeColonne + ")" + motCleInsertValues + "(" + requeteValeurColonne + ")";
            }
            else{ //si il y a un enregistrement deja existant
                //alors on utilise l'instruction update
                requeteComplete = "UPDATE " + nomTable + " SET ";
                for (std::size_t k=0;k<tabIndiceColValide.size();k++){//parcours des colonnes � importer
                    if (k>=nombreClePrimaire){//on ne s'occupe que des colonnes qui ne sont pas des identifiants
                        if (tabLigneLue[tabIndiceColValide[k]].empty())//si la valeur de la colonne est vide
                            tabLigneLue[tabIndiceColValide[k]] = "NULL";//on affecte la valeur NULL
                        //test si le champ est une chaine de caractere
                        if (tabLigneEntete[tabIndiceColValide[k]].substr(0,2) == nomMotCleId || //find() != std::string::npos
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomMotCleCode ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomJour ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,4) == nomMotCleDate ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,3) == nomChampNom ||
                            tabLigneEntete[tabIndiceColValide[k]].substr(0,11) == nomDescrDossier)
                            if (tabLigneLue[tabIndiceColValide[k]] != "NULL")
                                requeteComplete = requeteComplete + tabLigneEntete[tabIndiceColValide[k]] + "='" + tabLigneLue[tabIndiceColValide[k]] + "', ";
                            else requeteComplete = requeteComplete + tabLigneEntete[tabIndiceColValide[k]] + "=" + tabLigneLue[tabIndiceColValide[k]] + ", ";
                        else requeteComplete = requeteComplete + tabLigneEntete[tabIndiceColValide[k]] + "=" + tabLigneLue[tabIndiceColValide[k]] + ", ";
                    }
                }

                requeteComplete = requeteComplete.substr(0,requeteComplete.length()-2);//on supprime le ", "
                requeteComplete = requeteComplete + " WHERE ";//ajout de la condition avec les valeurs de cle primaire

                for (int indice=0;indice<nombreClePrimaire-1;indice++){//parcours des cl�s
                    requeteComplete = requeteComplete + tabLigneEntete[indice] + "='" + tabLigneLue[indice] + "' AND ";
                }
                //traitement de la derniere cle
                requeteComplete = requeteComplete + tabLigneEntete[nombreClePrimaire-1] + "='" + tabLigneLue[nombreClePrimaire-1] + "'";
            }
            videCurseurRes();//vide le curseur contenant la requete exitence d'enregistrement


            //excute la requete
            executeRequete(requeteComplete);
            if (!(retourneEtatRequete("commandeMaj"))){
                tabARetourner.push_back("probleme");
                return tabARetourner;
            }
            videCurseurRes();
            requeteComplete = ""; //remise � vide pour requete suivante
            requeteValeurColonne = "";
            requeteListeColonne = "";
            requeteTestExistant="";
        }
        fichierLu.close();//fermeture du fichier

        if (nomTable == nomTabMet || nomTable == nomTabPrecip || nomTable == nomTabObs){
            for(int nbCle=0;nbCle<nombreClePrimaire;nbCle++){
                tabARetourner.push_back(tabLigneLue[nbCle]);
            }
        }
        else
            tabARetourner.push_back("OK");
        return tabARetourner;
    }
    else{
        Gtk::MessageDialog dialogue("Error while opening file " + nomFichier + ".", false, Gtk::MESSAGE_INFO);
        dialogue.run();
        tabARetourner.push_back("probleme");
        return tabARetourner;
    }
}