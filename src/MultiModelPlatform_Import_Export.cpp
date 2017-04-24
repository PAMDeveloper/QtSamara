
bool CommunicationBDD::exportTableDansFichier(std::string nomFichier,std::string nomTable,Gtk::TreeView& treeViewExport,std::map < std::string,Gtk::TreeModelColumn<Glib::ustring>* >& mapNomChampTreeModelCol,std::string nomChampClePrimaire,std::string requete,int nombreClePrimaire,std::string nomChampClePrimaireDeux,std::string nomChampClePrimaireTrois,std::string nomChampClePrimaireQuatre){
    Gtk::ListStore::Row ligneSelectionneeTreeView;
    std::string ligneAEcrire,requeteExport,valeurChamp;
    std::ofstream fichierExport(nomFichier.c_str(), std::ios::out | std::ios::trunc); //declaration de flux sortie en ecriture
    Glib::ustring valeurID;
    if (fichierExport){//si la creation du fichier est ok
        if(requete==""){
            //recupere tte la gestion de selection du treeview
            Glib::RefPtr<Gtk::TreeSelection> selectionTreeView = treeViewExport.get_selection();
            //recupere un iterator de la ligne selectionnee du treeview
            Gtk::ListStore::iterator iterTreeModel = selectionTreeView->get_selected();
            if(iterTreeModel){//si il y a une ligne selectionnee
                //depointe l iterator pour obtenir la ligne
                ligneSelectionneeTreeView = *iterTreeModel;
                //recupere la valeur de la colonne id du treemodel
                valeurID=ligneSelectionneeTreeView[*(mapNomChampTreeModelCol.find(nomChampClePrimaire)->second)];
            }
            if (!(valeurID.empty())){//si une ligne a bien ete selectionnee
                Glib::ustring valeurIDDeux,valeurIDTrois,valeurIDQuatre;
                //requete select pour extraire l enregistrement selectionne
                requeteExport = "SELECT * FROM " + nomTable + " WHERE " + nomChampClePrimaire + "='" + valeurID + "'";
                if (nombreClePrimaire>1){
                    valeurIDDeux=ligneSelectionneeTreeView[*(mapNomChampTreeModelCol.find(nomChampClePrimaireDeux)->second)];
                    requeteExport = requeteExport + " AND " + nomChampClePrimaireDeux + "='" + valeurIDDeux + "'";
                }
                if (nombreClePrimaire>2){
                    valeurIDTrois=ligneSelectionneeTreeView[*(mapNomChampTreeModelCol.find(nomChampClePrimaireTrois)->second)];
                    requeteExport = requeteExport + " AND " + nomChampClePrimaireTrois + "='" + valeurIDTrois + "'";
                }
                if (nombreClePrimaire>3){
                    valeurIDQuatre=ligneSelectionneeTreeView[*(mapNomChampTreeModelCol.find(nomChampClePrimaireQuatre)->second)];
                    requeteExport = requeteExport + " AND " + nomChampClePrimaireQuatre + "='" + valeurIDQuatre + "'";
                }
            }
        }
        else {
            requeteExport = requete;
        }
        executeRequete(requeteExport);//exectution de la requete
        if (!(retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
            return  false;
        }
        for(int j=0;j<getNbColonnes();j++){//boucle sur le resultat de la requete precedente (nom des colonnes de la table)
            if (j == 0)
                ligneAEcrire = PQfname(leResultPrimaire,j);
            else //recupere et concatene le nom des champs
                ligneAEcrire = ligneAEcrire + "\t" + PQfname(leResultPrimaire,j);
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


bool CommunicationBDD::exportTableOptim_SensiDansFichier(std::string nomFichier,std::string nomTable,std::string nomChampClePrimaire,std::string requete,int nombreClePrimaire,std::string nomChampClePrimaireDeux,std::string nomChampClePrimaireTrois,std::string nomChampClePrimaireQuatre){
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
                ligneAEcrire = PQfname(leResultPrimaire,j);
            else //recupere et concatene le nom des champs
                ligneAEcrire = ligneAEcrire + "\t" + PQfname(leResultPrimaire,j);
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


void Fenetre::on_boutonExportSim_clicked(){
    bool retourImport = true;

    std::vector<int> vectorSimuSelectionnee;
    vectorSimuSelectionnee = listeSimulation.get_selected();//recupere les index des simu selectionnees
    Glib::ustring idSimuSelected;

    if (!(vectorSimuSelectionnee.empty())){ //si au moins une simu select
        if (vectorSimuSelectionnee.size()==1){

            std::string idParcelle,idVariete, idItinTech,codeStationMeteoSimu,codeStationPluvio,idDossier,
                            codeTypSol,codePays,codeContinent,idSite,dateDebutComplet,dateFinComplet;

            idSimuSelected=listeSimulation.get_text(vectorSimuSelectionnee[0]);//si 1 simu
            Glib::ustring requeteDebutSimu = "SELECT * FROM " + CommunicationBDD::nomTabSim + " WHERE " + CommunicationBDD::nomIdSim + "='";
            Glib::ustring requeteFinSimu = "'";
            Glib::ustring requeteCompleteSimu = requeteDebutSimu + idSimuSelected + requeteFinSimu;

            controleBDD.executeRequete(requeteCompleteSimu);//execution de la commande sur la connexion connex
            if (!(controleBDD.retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
                afficheRetourRequete();
            }

            idParcelle = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomIdParc);
            idVariete = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomIdVar);
            idItinTech = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomIdItinTech);
            codeStationMeteoSimu = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomCodeStatMetSim);
            codeStationPluvio = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomCodeStatPrecSim);
            dateDebutComplet = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomDateDebSim);//recupere la date complete YYYY-MM-DD
            dateFinComplet = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomDateFnSim);
            idDossier = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomIdDoss);

            std::string requeteParcelle = "SELECT * FROM " + CommunicationBDD::nomTabParc + " WHERE " +
                                            CommunicationBDD::nomIdParc + "='" + idParcelle + "'";
            controleBDD.executeRequete(requeteParcelle);//execution de la commande sur la connexion connex
            if (!(controleBDD.retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
                afficheRetourRequete();
            }
            codeTypSol = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomCodeTypSol);
            idSite = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomIdSite);

            std::string requeteSite = "SELECT * FROM " + CommunicationBDD::nomTabSite + " WHERE " +
                                    CommunicationBDD::nomIdSite + "='" + idSite + "'";
            controleBDD.executeRequete(requeteSite);//execution de la commande sur la connexion connex
            if (!(controleBDD.retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
                afficheRetourRequete();
            }
            codePays = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomCodePays);

            std::string requetePays = "SELECT * FROM " + CommunicationBDD::nomTabPays + " WHERE " + CommunicationBDD::nomCodePays + "='" + codePays + "'";
            controleBDD.executeRequete(requetePays);//execution de la commande sur la connexion connex
            if (!(controleBDD.retourneEtatRequete("select"))) {//test sur le statut du res pour voir si commande bien execute
                afficheRetourRequete();
            }
            codeContinent = controleBDD.getValeurChampResultatParNom(CommunicationBDD::nomCodeConti);

            std::string requeteSol = "SELECT * FROM " + CommunicationBDD::nomTabTypSol + " WHERE " +
                                    CommunicationBDD::nomCodeTypSol + "='" + codeTypSol + "'";
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Sol.txt",CommunicationBDD::nomTabTypSol,
                                                                         CommunicationBDD::nomCodeTypSol,requeteSol);
            std::string requeteContinent = "SELECT * FROM " + CommunicationBDD::nomTabConti + " WHERE " +
                                            CommunicationBDD::nomCodeConti + "='" + codeContinent + "'";
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Continent.txt",CommunicationBDD::nomTabConti,
                                                                         CommunicationBDD::nomCodeConti,requeteContinent);
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Pays.txt",CommunicationBDD::nomTabPays,
                                                                         CommunicationBDD::nomCodePays,requetePays);
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Site.txt",CommunicationBDD::nomTabSite,
                                                                         CommunicationBDD::nomIdSite,requeteSite);
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Parcelle.txt",CommunicationBDD::nomTabParc,
                                                                         CommunicationBDD::nomIdParc,requeteParcelle);
            std::string requeteStation = "SELECT * FROM " + CommunicationBDD::nomTabStation + " WHERE " +
                                        CommunicationBDD::nomCodeStation + "='" + codeStationMeteoSimu + "'";
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Station.txt",CommunicationBDD::nomTabStation,
                                                                         CommunicationBDD::nomCodeStation,requeteStation);
            std::string requeteVariete = "SELECT * FROM " + CommunicationBDD::nomTabVar + " WHERE " +
                                        CommunicationBDD::nomIdVar + "='" + idVariete + "'";
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Variete.txt",CommunicationBDD::nomTabVar,
                                                                         CommunicationBDD::nomIdVar,requeteVariete);
            std::string requeteItineraireTech = "SELECT * FROM " + CommunicationBDD::nomTabItinTech + " WHERE " +
                                                CommunicationBDD::nomIdItinTech + "='" + idItinTech + "'";
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Itineraire_Technique.txt",
                                                                        CommunicationBDD::nomTabItinTech,CommunicationBDD::nomIdItinTech,requeteItineraireTech);
            std::string requeteIrrigation = "SELECT * FROM " + CommunicationBDD::nomTabIrrig + " WHERE " +
                                            CommunicationBDD::nomIdParc + "='" + idParcelle + "' AND " + CommunicationBDD::nomJour + ">='" + dateDebutComplet + "' AND " + CommunicationBDD::nomJour + "<='" + dateFinComplet + "' ORDER BY " + CommunicationBDD::nomJour;
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Irrigation.txt",
                                                                         CommunicationBDD::nomTabIrrig,CommunicationBDD::nomIdParc,requeteIrrigation);
            std::string requeteMeteo = "SELECT * FROM " + CommunicationBDD::nomTabMet + " WHERE " +
                                        CommunicationBDD::nomCodeStation + "='" + codeStationMeteoSimu + "' AND " + CommunicationBDD::nomJour + ">='" + dateDebutComplet + "' AND " + CommunicationBDD::nomJour + "<='" + dateFinComplet + "' ORDER BY " + CommunicationBDD::nomCodeStation + "," + CommunicationBDD::nomJour;
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Meteo.txt",CommunicationBDD::nomTabMet,
                                                                         CommunicationBDD::nomCodeStation,requeteMeteo);
            std::string requetePluie = "SELECT * FROM " + CommunicationBDD::nomTabPrecip + " WHERE " +
                                        CommunicationBDD::nomCodeStation + "='" + codeStationPluvio + "' AND " + CommunicationBDD::nomJour + ">='" + dateDebutComplet + "' AND " + CommunicationBDD::nomJour + "<='" + dateFinComplet + "' ORDER BY " + CommunicationBDD::nomCodeStation + "," + CommunicationBDD::nomJour;
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Pluie.txt",CommunicationBDD::nomTabPrecip,
                                                                         CommunicationBDD::nomCodeStation,requetePluie);
            std::string requeteObs = "SELECT * FROM " + CommunicationBDD::nomTabObs + " WHERE " +
                                        CommunicationBDD::nomIdParc + "='" + idParcelle + "' AND " +
                                        CommunicationBDD::nomIdVar + "='" + idVariete + "' AND " +
                                        CommunicationBDD::nomIdItinTech + "='" + idItinTech + "' AND " +
                                        CommunicationBDD::nomJour + ">='" + dateDebutComplet + "' AND " +
                                        CommunicationBDD::nomJour + "<='" + dateFinComplet + "' ORDER BY " +
                                        CommunicationBDD::nomIdParc + "," + CommunicationBDD::nomIdVar + "," +
                                        CommunicationBDD::nomIdItinTech + "," + CommunicationBDD::nomJour;
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("ValObs.txt",CommunicationBDD::nomTabObs,
                                                                         CommunicationBDD::nomIdParc,requeteObs,4,
                                                                         CommunicationBDD::nomIdVar,CommunicationBDD::nomIdItinTech,CommunicationBDD::nomJour);
            retourImport = controleBDD.exportTableOptim_SensiDansFichier("Simulation.txt",CommunicationBDD::nomTabSim,
                                                                         CommunicationBDD::nomIdSim,requeteCompleteSimu);

            if (!(retourImport)){
                Gtk::MessageDialog dialogueErreurImport(*this,"Export problem",false,Gtk::MESSAGE_WARNING);
                dialogueErreurImport.run();
            }
            Gtk::MessageDialog messageActionTerminee(*this,"Simulation's export ended", false);
            messageActionTerminee.run();
        }
    }
}


void Fenetre::on_boutonExportResultats_clicked(){
    std::string  nomFichier;
    bool retourImport;

    std::vector<int> vectorSimuSelectionnee;
    std::string tableResSelectionnee, requeteFrom=" FROM ", conditionWhere = " WHERE ";;
    vectorSimuSelectionnee = listeSimulationResultat.get_selected();
    tableResSelectionnee = listeDeroulanteTableRes.get_active_text();

    if((vectorSimuSelectionnee.size()>0) && (tableResSelectionnee.size() > 0)){
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
                if (tableResSelectionnee == CommunicationBDD::nomTabResultat){
                    requeteFrom = requeteFrom + CommunicationBDD::nomTabResultat;
                    for(unsigned int i=0;i<vectorSimuSelectionnee.size();i++){
                        conditionWhere = conditionWhere + CommunicationBDD::nomIdSim + "='" +
                        listeSimulationResultat.get_text(vectorSimuSelectionnee[i]) + "' OR ";
                    }
                    conditionWhere = conditionWhere.substr(0,conditionWhere.length()-4);
                    requeteResultat = requeteResultat + requeteFrom + conditionWhere;

                    retourImport = controleBDD.exportTableDansFichier(nomFichier,tableResSelectionnee,treeViewResultat,mapResultatNomChampTreeModelCol,CommunicationBDD::nomIdSim,requeteResultat,2,CommunicationBDD::nomJour);
                }
                else if (tableResSelectionnee == CommunicationBDD::nomTabObs){

                    requeteFrom = requeteFrom + CommunicationBDD::nomTabObs;
                    requeteFrom = requeteFrom + "," + CommunicationBDD::nomTabSim;
                    conditionWhere = conditionWhere + "(";
                    for(unsigned int j=0;j<vectorSimuSelectionnee.size();j++){
                        conditionWhere = conditionWhere + CommunicationBDD::nomTabSim + "." +
                        CommunicationBDD::nomIdSim + "='" +
                        listeSimulationResultat.get_text(vectorSimuSelectionnee[j]) + "' OR ";
                    }
                    conditionWhere = conditionWhere.substr(0,conditionWhere.length()-4);
                    conditionWhere = conditionWhere + ") AND " + CommunicationBDD::nomTabObs + "." +
                                    CommunicationBDD::nomIdParc + "=" + CommunicationBDD::nomTabSim + "." +
                                    CommunicationBDD::nomIdParc + " AND " + CommunicationBDD::nomTabObs + "." +
                                    CommunicationBDD::nomIdVar + "=" + CommunicationBDD::nomTabSim + "." +
                                    CommunicationBDD::nomIdVar + " AND " + CommunicationBDD::nomTabObs + "." +
                                    CommunicationBDD::nomIdItinTech + "=" + CommunicationBDD::nomTabSim + "." +
                                    CommunicationBDD::nomIdItinTech + " AND " + CommunicationBDD::nomTabObs + "." +
                                    CommunicationBDD::nomJour + ">=" + CommunicationBDD::nomTabSim + "." +
                                    CommunicationBDD::nomDateDebSim + " AND " + CommunicationBDD::nomTabObs + "." +
                                    CommunicationBDD::nomJour + "<=" + CommunicationBDD::nomTabSim + "." +
                                    CommunicationBDD::nomDateFnSim;
                    requeteResultat = requeteResultat + requeteFrom + conditionWhere;

                    retourImport = controleBDD.exportTableDansFichier(nomFichier,tableResSelectionnee,treeViewResultat,mapResultatNomChampTreeModelCol,CommunicationBDD::nomIdParc,requeteResultat,4,CommunicationBDD::nomIdVar,CommunicationBDD::nomIdItinTech,CommunicationBDD::nomJour);
                }
            }
            if (!(retourImport)){
                Gtk::MessageDialog dialogueErreurImport(*this,"Export problem",false,Gtk::MESSAGE_WARNING);
                dialogueErreurImport.run();
            }
        }
    }
}


std::vector<std::string> CommunicationBDD::importFichierDansTable(std::string nomFichier,std::string nomTable,std::string nomChampClePrimaire,int nombreClePrimaire,std::string nomChampClePrimaireDeux,std::string nomChampClePrimaireTrois,std::string nomChampClePrimaireQuatre){

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


void Fenetre::on_boutonImportVar_clicked(){
    std::string  nomFichier,requeteVar;
    std::vector<std::string> retourImport;
    Gtk::FileChooserDialog dialogueChoixFichVar(*this,"Please choose a culture text file",Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialogueChoixFichVar.set_transient_for(*this);
    //ajout des boutons de choix de la boite de dialogue
    dialogueChoixFichVar.add_button("CANCEL", Gtk::RESPONSE_CANCEL);
    dialogueChoixFichVar.add_button("OPEN", Gtk::RESPONSE_OK);
    //ajout de filtre, seul les fichiers textes peuvent etre selectionnes
    Glib::RefPtr<Gtk::FileFilter>  filtreFichTexte = Gtk::FileFilter::create();
    filtreFichTexte->set_name("Text files");
    filtreFichTexte->add_mime_type("text/plain");
    dialogueChoixFichVar.add_filter(filtreFichTexte);
    //affiche la boite et attend reponse de l utilisateur
    int resultat = dialogueChoixFichVar.run();
    //gestion de la reponse
    if (resultat == Gtk::RESPONSE_OK){
        //recupere le nom du fichier selectionne
        nomFichier = dialogueChoixFichVar.get_filename();
        if (!(nomFichier.empty())){//test si un fichier a bien ete selectionne
            retourImport = controleBDD.importFichierDansTable(nomFichier,CommunicationBDD::nomTabVar,CommunicationBDD::nomIdVar);
            if (retourImport[0] == "probleme"){
                Gtk::MessageDialog dialogueErreurImport(*this,"Import problem",false,Gtk::MESSAGE_WARNING);
                dialogueErreurImport.run();
            }
            else{
                if (!(retourImport[0].empty())){
                    requeteVar = "SELECT * FROM " + CommunicationBDD::nomTabVar + " ORDER BY " + CommunicationBDD::nomIdVar;
                    if (!(controleBDD.remplirGrilleDeDonnees(treeViewVariete,treeModelVariete,mapVarieteNomChampTreeModelCol,requeteVar))){
                        afficheRetourRequete();
                        return;
                    }
                }
            }
        }
    }
}