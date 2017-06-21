/**   @file  
     @brief  
*/
#include "Requetes.h"



//modif CB 30/01/07 copie proc dans UimportExport dans gestionbdd

void SupprEnreg( TTable const& maTable,  std::string nomCol, std::string valChamps)
// suppression d'un enregistrement dans la table maTable.
// Condition : la valeur du champs correspondant à la colonne nomCol est égale à valChamps
{
  DBModule1.ReqSupprEnr.SQL.Clear;
  DBModule1.ReqSupprEnr.DatabaseName = "DBResult";
  /*     DBModule1.ReqSupprEnr.DatabaseName:=maTable.DatabaseName;
  */
  DBModule1.ReqSupprEnr.SQL.Add( "DELETE FROM " + maTable.Name );
  DBModule1.ReqSupprEnr.SQL.Add( " WHERE " + nomCol + "=\'" + valChamps + "\'" );

  DBModule1.ReqSupprEnr.ExecSQL;
}

void initTemp()
{
  int i;
  TextFile F;

  //Recupération du chemin de la BDD
  DBModule1.ListeRequete.First;
  DBModule1.Station.open;
  cheminBdd = DBModule1.Station.Database.Directory;
  i = AnsiPos( "DBEcosys" , cheminBdd );
  cheminBdd = copy( cheminBdd , 0 , i - 1 ) + "DBEcosys\\";

  // Copie de la structure de Resjour dans ResTemp par copie de la sauvegarde de ResJour
  CopyFile( char*( cheminBdd + "DBResult\\Resjour_vide.DB" ) , char*( cheminBdd +
    "DBTemp\\resTemp.DB" ) , false ); //false indique qu'on écrase si existe
  CopyFile( char*( cheminBdd + "DBResult\\Resjour_vide.PX" ) , char*( cheminBdd +
    "DBTemp\\resTemp.PX" ) , false ); //false indique qu'on écrase si existe

  // On filtre les requetes pour ne prendre que celles concernant le dossier courant
  DBModule1.Simule.Filter = "Id=" +
    QuotedStr( DBModule1.RequeteSimuleIdSimule.Value );
  DBModule1.Simule.Filtered = true;
  DBModule1.ListeRequete.Filter = "Dossier=" +
    QuotedStr( DBModule1.SimuleIdDossier.Value );
  DBModule1.ListeRequete.Filtered = true;
  DBModule1.Simule.Filtered = false;

  DBModule1.ListeRequete.First;
  while(  !DBModule1.ListeRequete.eof )
  {
    
    // et on le recréé
    ForceDirectories( cheminBdd + "DBResult\\Requetes\\" );
    AssignFile( F , cheminBdd + "DBResult\\Requetes\\" +
      DBModule1.ListeRequete.FindField( "IdRequete" ).Value + ".txt" );
    ReWrite( F );
    Write( F , " " );
    closeFile( F );
    DBModule1.ListeRequete.Next;
  }
}

void execRequeteScenar( int const& CptSimul)
// Execute toutes les requetes contenues dans ListeRequete pour la simulation en cours

{
  int i; int j; int k;
  TextFile F;
  std::string nom; std::string req; std::string reqFrom;
  bool virgule;
  bool XportTexte; bool XportTable;


  //showmessage('woohoo');
  // On filtre les requetes pour ne prendre que celles concernant le dossier courant
  DBModule1.Simule.Filter = "Id=" +
    QuotedStr( DBModule1.RequeteSimuleIdSimule.Value );
  DBModule1.Simule.Filtered = true;
  DBModule1.ListeRequete.Filter = "Dossier=" +
    QuotedStr( DBModule1.SimuleIdDossier.Value );
  DBModule1.ListeRequete.Filtered = true;
  DBModule1.Simule.Filtered = false;

  DBModule1.ResTemp.active = false;
  DBModule1.ResJour.active = false;
  DBModule1.ListeRequete.open;
  DBModule1.ListeRequete.First;
  i = 1;

  // On selectionne dans la table requetes les lignes qui ont un Id égal à celui de la table ListeRequete
  while(  !DBModule1.ListeRequete.eof )
  {
    DBModule1.TableRequete.Filter = "Id=" +
      QuotedStr( DBModule1.ListeRequete.FindField( "IdRequete" ).Value );
    DBModule1.TableRequete.Filtered = true;
    DBModule1.TableRequete.open;
    DBModule1.TableRequete.First;

    { long j_end = 16 ; for( j = 0 ; j < j_end ; ++j )
    {
      tabFrom[ j ] = false;
    }}

    if(  DBModule1.TableRequete.FieldByName( "Id" ).Value == " Résultats journaliers" )
    {
      if(  DBModule1.ListeRequete.FieldByName( "Table" ).AsBoolean == true )
      {
        EnlevederiveResJour;
        BatchResJour(  );
        deriveResJour(  );
      }

      if(  DBModule1.ListeRequete.FieldByName( "Texte" ).AsBoolean == true )
      {
        DBModule1.QueryTemp.close;
        DBModule1.QueryTemp.DatabaseName = "DBTemp";
        DBModule1.QueryTemp.SQL.Clear;
        DBModule1.QueryTemp.SQL.Add( "SELECT * FROM ResTemp" );
        DBModule1.QueryTemp.open;

        DBModule1.QueryTemp.DisableControls;
        
        AssignFile( F , cheminBdd + "\\DBResult\\Requetes\\" +
          DBModule1.ListeRequete.FindField( "IdRequete" ).Value + ".txt" );
        Append( F );
        // Ecriture des noms des champs
        if(  CptSimul == 0 )
        {
          Write( F , "Id_Simule" + chr( 9 ) );
          { long j_end = DBModule1.QueryTemp.FieldCount ; for( j = 0 ; j < j_end ; ++j )
            Write( F , DBModule1.QueryTemp.Fields[ j ].DisplayName + chr( 9 ) );}
          writeln( F , "" );
        }
        { long k_end = DBModule1.QueryTemp.RecordCount ; for( k = 0 ; k < k_end ; ++k )
        {
          Write( F , DBModule1.RequeteSimuleIdSimule.Value + chr( 9 ) );
          { long j_end = DBModule1.QueryTemp.FieldCount ; for( j = 0 ; j < j_end ; ++j )
            Write( F , DBModule1.QueryTemp.Fields[ j ].AsString + chr( 9 ) );}
          writeln( F , "" );
          DBModule1.QueryTemp.Next;
        }}
        DBModule1.QueryTemp.EnableControls;
        closeFile( F );
      }

    }
    else
    {
      if(  ( DBModule1.TableRequete.FieldByName( "Req_where" ).IsNull ) )
        req =
          FormeReqTemp( DBModule1.TableRequete.FieldByName( "Req_select" ).Value , "" );else
        req =
          FormeReqTemp( DBModule1.TableRequete.FieldByName( "Req_select" ).Value
          , DBModule1.TableRequete.FieldByName( "Req_where" ).Value );
      reqFrom = " FROM ";
      if(  ( DBModule1.TableRequete.FieldByName( "Pas_nc" ).Value != 0 ) &&
        ( pos( DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value , req ) == 0 ) )
        req = stringReplace( req , "SELECT " , "SELECT " +
          DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value + "," , [  ] );
      // Creation de la clause FROM en fonction du TabFrom

      virgule = false;
      { long j_end = 16 ; for( j = 0 ; j < j_end ; ++j )
      {
        if(  ( tabFrom[ j ] == true ) )
        {
          DBModule1.TableEntity.close;
          DBModule1.TableEntity.Filter = "";
          DBModule1.TableEntity.Filter = "NumTable = " + inttostr( j );
          DBModule1.TableEntity.Filtered = true;
          DBModule1.TableEntity.open;
          DBModule1.TableEntity.First;
          if(  virgule == true )
            reqFrom = reqFrom + " , ";

          //On remplace ResJOur par ResTemp
          if(  j == 1 )
            reqFrom = reqFrom + "\"" + cheminBdd + "DBTemp\\ResTemp\" AS T1"
              //Pour les autres tables, on récupère leur nom et la base à laquelle elles appartienent sur TableEntity;else
            reqFrom = reqFrom + "\"" + cheminBdd +
              DBModule1.TableEntity.FieldByName( "DataBaseSource" ).Value + "\\" +
              DBModule1.TableEntity.FieldByName( "Source" ).Value + "\" AS T" +
              inttostr( j );

          virgule = true;
        }
      }} //fin for j0 to 15

      //Concaténation de la clause FROM au reste de la requête
      if(  pos( "WHERE" , req ) > 0 )
        req = stringReplace( req , "WHERE" , reqFrom + " WHERE" , [  ] );else
        req = req + reqFrom;

      req = stringReplace( req , "DBResult\\Resjour" , "DBTemp\\ResTemp"
        , [ rfReplaceAll ] );
      req = stringReplace( req , "Resjour" , "ResTemp" , [ rfReplaceAll ] );

      if(  DBModule1.TableRequete.FieldByName( "Type" ).Value == "Echantillon" )
        req = stringReplace( req , "FROM" , "," +
          DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value + " FROM"
          , [ rfReplaceAll ] );

      DBModule1.ResTemp.open;
      //MainForm.memDeroulement.Lines.Add(inttostr(i)+') '+DBmodule1.ListeRequete.FindField('IdRequete').Value);

      DBModule1.ResTemp.close;
      DBModule1.QueryTemp.close;

      DBModule1.QueryTemp.DatabaseName = "DBTemp";
      DBModule1.QueryTemp.SQL.Clear;
      DBModule1.QueryTemp.SQL.Add( req );
      if(  ( DBModule1.TableRequete.FieldByName( "Type" ).Value == "Normal" ) )
        req = req + DBModule1.TableRequete.FindField( "Req_Group" ).Value;
      DBModule1.QueryTemp.open;

      //if (DBModule1.QueryTemp.FieldCount=0) AND (DBModule1.QueryTemp.RecordCount=0)then
      //    MainForm.memDeroulement.Lines.add('Pas de résultats pour cette requête');

      if(  ( DBModule1.TableRequete.FieldByName( "Type" ).Value != "Normal" ) )
      {
        // On copie le résultat de la requête temporaire dans ResultRequete
        //C'est sur cette table qu'on va travailler (ajout des champs...)
        DBModule1.ResultRequete.close;
        DBModule1.BatchMoveTemp.Source = DBModule1.QueryTemp;
        DBModule1.ResultRequete.TableName = "ResultRequete";
        DBModule1.ResultRequete.DatabaseName = "DBRequete";
        DBModule1.BatchMoveTemp.Destination = DBModule1.ResultRequete;
        DBModule1.BatchMoveTemp.mode = batcopy;
        DBModule1.BatchMoveTemp.Execute;
        DBModule1.QueryTemp.close;
        DBModule1.ResultRequete.close;

        DBModule1.QueryTemp.DatabaseName = "DBRequete";

        requete( "ALTER TABLE ResultRequete ADD annee INTEGER" );
        requete( "ALTER TABLE ResultRequete ADD Prem_jour DATE" );
        requete( "ALTER TABLE ResultRequete ADD modulo INTEGER" );
        requete( "ALTER TABLE ResultRequete ADD num_agreg INTEGER" );

        if(  DBModule1.TableRequete.FieldByName( "Type" ).Value == "Echantillon" )
        {
          requete( "UPDATE ResultRequete SET modulo = (" +
            DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value + " / " +
            inttostr( DBModule1.TableRequete.FieldByName( "Pas_nc" ).Value ) + ")" );

          //Création d'une table annexe contenant seulement les valeur de la variable servant de pas
          //pour chaque pas de l'échantillon
          DBModule1.QueryTemp.SQL.Clear;
          if(  tabFrom[ 1 ] == true )
            DBModule1.QueryTemp.SQL.Add( "SELECT MIN(jour) AS jour, MIN(" +
              DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value + ")AS " +
              DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value +
              " ,numeroSimule, modulo FROM ResultRequete GROUP BY numeroSimule, modulo" );
          if(  tabFrom[ 1 ] == false )
            DBModule1.QueryTemp.SQL.Add( "SELECT MIN(jour) AS jour,MIN(" +
              DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value + ")AS " +
              DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value +
              " , modulo FROM ResultRequete GROUP BY modulo" );

          DBModule1.QueryTemp.ExecSQL;

          DBModule1.TableTemp.close;
          DBModule1.TableTemp.DatabaseName = "DBRequete";
          DBModule1.TableTemp.TableName = "Temp";
          DBModule1.BatchMoveTemp.Destination = DBModule1.TableTemp;
          DBModule1.BatchMoveTemp.mode = batcopy;
          DBModule1.BatchMoveTemp.Execute;

          DBModule1.TableTemp.close;
          DBModule1.BatchMoveTemp.Destination = DBModule1.ResultRequete;

        }

        if(  DBModule1.TableRequete.FieldByName( "Type" ).Value == "Agregation" )
        {
          if(  ( mode != "Pentade" ) && ( mode != "Decade" ) && ( mode != "Quinzaine" )
            && ( mode != "Mois" ) )
          { //mode non calendaire
            requete( "UPDATE ResultRequete SET num_agreg = (" +
              DBModule1.TableRequete.FieldByName( "Champ_nc" ).Value + " / " +
              inttostr( DBModule1.TableRequete.FieldByName( "Pas_nc" ).Value ) + ")" );
          }

          else
          { // Mode calendaire
            requete( "UPDATE ResultRequete SET Prem_jour = Jour" );

            if(  ( mode == "Pentade" ) )
            {
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-5, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))<=5" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-4, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>5 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=10" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-3, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>10 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=15" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-2, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>15 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=20" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-1, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>20 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=25" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>25" );
            }

            if(  ( mode == "Decade" ) )
            {
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*3-2, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))<=10" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*3-1, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>10 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=20" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*3, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>20" );
            }

            if(  ( mode == "Quinzaine" ) )
            {
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*2-1,annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))<=15" );
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*2, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>15" );
            }

            if(  ( mode == "Mois" ) )
            {
              requete( "UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour), annee=EXTRACT(YEAR FROM ResultRequete.Jour)" );
            }
          } //fin calendaire
        } //fin agreg
        req = "";
        if(  DBModule1.TableRequete.FieldByName( "Type" ).Value != "Echantillon" )
          req = DBModule1.TableRequete.FindField( "Req_Select" ).Value + " FROM \""
            + cheminBdd + "DBRequete\\ResultRequete\" AS T1 " +
            DBModule1.TableRequete.FindField( "Req_group" ).Value;else
        {
          if(  tabFrom[ 1 ] == false )
            req = DBModule1.TableRequete.FieldByName( "Req_select" ).Value +
              ",min(Temp.jour),modulo FROM \"" + cheminBdd +
              "DBRequete\\ResultRequete\" AS T1,\"" + cheminBdd +
              "DBRequete\\Temp\" AS Temp WHERE T1.jour=Temp.jour AND T1.NumeroSimule=Temp.NumeroSimule " +
              stringReplace( DBModule1.TableRequete.FindField( "Req_group" ).Value
              , "IdSimule," , "" , [ rfReplaceAll ] );
          if(  ( tabFrom[ 1 ] == true ) && ( pos( "NumeroSimule"
            , DBModule1.TableRequete.FindField( "Req_group" ).Value ) == 0 ) )
            req =
              stringReplace( DBModule1.TableRequete.FieldByName( "Req_select" ).Value
              , "T1.IdSimule," , "" , [ rfReplaceAll ] ) +
              ",numeroSimule,min(Temp.jour), modulo FROM \"" + cheminBdd +
              "DBRequete\\ResultRequete\" AS T1,\"" + cheminBdd +
              "DBRequete\\Temp\" AS Temp WHERE T1.jour=Temp.jour AND T1.NumeroSimule=Temp.NumeroSimule " +
              stringReplace( stringReplace( DBModule1.TableRequete.FindField( "Req_group" ).Value
              , "T1.IdSimule," , "" , [ rfReplaceAll ] ) , "GROUP BY "
              , "GROUP BY T1.numeroSimule," , [  ] );
          if(  tabFrom[ 1 ] == true && ( pos( "NumeroSimule"
            , DBModule1.TableRequete.FindField( "Req_group" ).Value ) > 0 ) )
            req =
              stringReplace( DBModule1.TableRequete.FieldByName( "Req_select" ).Value
              , "T1.IdSimule," , "" , [ rfReplaceAll ] ) + ", T1.modulo,T1.Jour FROM \"" +
              cheminBdd + "DBRequete\\ResultRequete\" AS T1,\"" + cheminBdd +
              "DBRequete\\Temp\" AS Temp WHERE T1.jour=Temp.jour AND T1.NumeroSimule=Temp.NumeroSimule  " +
              stringReplace( DBModule1.TableRequete.FindField( "Req_group" ).Value
              , "T1.IdSimule," , "" , [ rfReplaceAll ] ); // AND T1.Modulo=Temp.Modulo

        }
        { long j_end = 16 ; for( j = 2 ; j < j_end ; ++j )
        {
          req = stringReplace( req , "T" + inttostr( j ) + "." , "T1."
            , [ rfReplaceAll ] );
        }}

        DBModule1.QueryTemp.DatabaseName = "DBRequete";
        DBModule1.QueryTemp.SQL.Clear;
        if(  ( mode == "Pentade" ) || ( mode == "Decade" ) || ( mode == "Quinzaine" ) ||
          ( mode == "Mois" ) && ( pos( req , "min(Jour)" ) == 0 ) )
          req = stringReplace( req , "Select " , "Select min(Jour) AS Jour," , [  ] );
        //MainForm.memDeroulement.Lines.add(req);                 //affichage de la requête
        DBModule1.QueryTemp.SQL.Add( req );
        DBModule1.QueryTemp.open;
      } // fin if (DBModule1.TableRequete.FieldByName('Type').value<>'Normal') then

      //MainForm.memDeroulement.Lines.add('Requete terminée');
      XportTable = DBModule1.ListeRequete.FieldByName( "Table" ).AsBoolean;
      XportTexte = DBModule1.ListeRequete.FieldByName( "Texte" ).AsBoolean;
      //ecriture des résultats dans le fichier txt
      if(  ( DBModule1.QueryTemp.FieldCount != 0 ) &&
        ( DBModule1.QueryTemp.RecordCount != 0 ) )
      {
        if(  XportTexte )
        {
          DBModule1.QueryTemp.DisableControls;
          //AssignFile(F, ExtractFilePath(Application.ExeName)+'\'+DBModule1.ListeRequete.FindField('IdRequete').Value+'.txt');
          AssignFile( F , cheminBdd + "\\DBResult\\Requetes\\" +
            DBModule1.ListeRequete.FindField( "IdRequete" ).Value + ".txt" );
          Append( F );
          // Ecriture des noms des champs
          if(  CptSimul == 0 )
          {
            Write( F , "Id_Simule" + chr( 9 ) );
            { long j_end = DBModule1.QueryTemp.FieldCount ; for( j = 0 ; j < j_end ; ++j )
              Write( F , DBModule1.QueryTemp.Fields[ j ].DisplayName + chr( 9 ) );}
            writeln( F , "" );
          }
          { long k_end = DBModule1.QueryTemp.RecordCount ; for( k = 0 ; k < k_end ; ++k )
          {
            Write( F , DBModule1.RequeteSimuleIdSimule.Value + chr( 9 ) );
            { long j_end = DBModule1.QueryTemp.FieldCount ; for( j = 0 ; j < j_end ; ++j )
              Write( F , DBModule1.QueryTemp.Fields[ j ].AsString + chr( 9 ) );}
            writeln( F , "" );
            DBModule1.QueryTemp.Next;
          }}
          DBModule1.QueryTemp.EnableControls;
          closeFile( F );
        }

        if(  XportTable )
        {
          nom = DBModule1.ListeRequete.FindField( "IdRequete" ).Value;
          if(  CptSimul == 0 )
            CopyFile( char*( cheminBdd + "DBRequete\\Temp.DB" ) , char*( cheminBdd +
              "DBResult\\Requetes\\" + nom + ".DB" ) , false );
          DBModule1.TableTemp.close;
          DBModule1.TableTemp.DatabaseName = "DBResultRequetes";
          DBModule1.TableTemp.TableName = nom;
          DBModule1.BatchMoveTemp.Destination = DBModule1.TableTemp;
          if(  CptSimul == 0 )
            DBModule1.BatchMoveTemp.mode = batcopy;else
            DBModule1.BatchMoveTemp.mode = batAppend;
          DBModule1.BatchMoveTemp.Execute;
        }
      }
    } // Fin du else (si Id=' Résultats Journaliers -> else)

    ++i;
    DBModule1.ListeRequete.Next;

  } //fin listeRequete.EOF
   
      
  DBModule1.ListeRequete.Filtered = false;
 
}

// Execute une requete qui modifie la base

void requete(std::string r)
{
  DBModule1.QueryTemp.SQL.Clear;
  DBModule1.QueryTemp.SQL.Add( r );
  DBModule1.QueryTemp.ExecSQL;
}

void BatchResJour()
//Recopie si demandé les resultats dans Resjour
{
  
  DBModule1.BatchmoveFinRequete.mode = batAppendUpdate;
  DBModule1.BatchmoveFinRequete.Execute;

}

void deriveResJour()
// On derive le flot de ResJour vers ResTemp
{
  //showmessage('debut deriveresjour');
  DBModule1.ResJour.active = false;
  DBModule1.DSResJour.Enabled = false;
  DBModule1.ResTemp.active = false;
  DBModule1.ResJour.DatabaseName = "DBTemp";
  DBModule1.ResJour.TableName = "ResTemp.db";
  DBModule1.DSResJour.Enabled = true;
  DBModule1.ResJour.active = true;
  
}

void EnlevederiveResJour()
// On remet en place le flot de ResJour
{
  DBModule1.ResJour.active = false;
  DBModule1.DSResJour.Enabled = false;
  DBModule1.ResTemp.active = false;
  DBModule1.ResJour.DatabaseName = "DBResult";
  DBModule1.ResJour.TableName = "ResJour";
  DBModule1.DSResJour.Enabled = true;
  DBModule1.ResJour.active = true;
}

//Formation de la requete temporaire

std::string FormeReqTemp(std::string ReqSel2, std::string ReqWhere2)
{   std::string result;
  TstringList SLselect; TstringList SLwhere;
  std::string ReqSel; std::string ReqWhere;
  int nb; int i; int j;
  char nbT; char nbT2;


  result = "SELECT ";
  mode = "";

  // On décompose la requête en la découpant en tronçons au niveau des virgules
  ReqSel = stringReplace( ReqSel2 , "Select" , "" , [  ] );
  SLselect = TstringList.Create;
  SLselect.Text = stringReplace( ReqSel , "," , (char)13(char)10 , [ rfReplaceAll ] );

  //Ici, chaque partie de la requête est dans une case de la StringList
  //Mais on ne prends pas les champs num_agreg, année, ... qui seront ajoutés à la table temporaire plus tard
  { long i_end = ( SLselect.Count - 1 )+1 ; for( i = 0 ; i < i_end ; ++i )
  {
    if(  ( pos( " AS " , SLselect[ i ] ) == 0 ) && ( pos( "num_agreg " , SLselect[ i ] ) == 0 )
      && ( pos( "annee " , SLselect[ i ] ) == 0 ) && ( pos( "T" , SLselect[ i ] ) != 0 ) &&
      ( pos( "jour" , SLselect[ i ] ) == 0 ) && ( pos( "numeroSimule " , SLselect[ i ] ) == 0 ) )
    {
      SLselect[ i ] = stringReplace( SLselect[ i ] , " " , "" , [ rfReplaceAll ] );
      if(  i != 0 )
        result = result + ", ";

      if(  ( pos( "(" , SLselect[ i ] ) == 0 ) )
      {
        if(  pos( SLselect[ i ] + "," , result ) == 0 )
          result = result + SLselect[ i ];else
          result = copy( result , 0 , Length( result ) - 2 );
      }

      // Si il y a une parenthèse -> c'est une fonction
      //on ne prend que ce qu'il y a entre ( et )
      if(  ( pos( "(" , SLselect[ i ] ) > 0 ) )
      {
        if(  pos( stringReplace( copy( SLselect[ i ] , pos( "(" , SLselect[ i ] ) + 1
          , Length( SLselect[ i ] ) ) , ")" , "" , [ rfReplaceAll ] ) + "," , result ) == 0 )
          result = result + stringReplace( copy( SLselect[ i ] , pos( "("
            , SLselect[ i ] ) + 1 , Length( SLselect[ i ] ) ) , ")" , "" , [ rfReplaceAll ] );else
          result = copy( result , 0 , Length( result ) - 2 );
      }

      //mise à jour du TabFrom
      // On recupère les 2 chiffres aprés le T, et on les accole
      if(  ( SLselect[ i ][ 1 ] == "T" ) )
      {
        nbT = SLselect[ i ][ 2 ];
        nbT2 = SLselect[ i ][ 3 ];
      }
      else //Si il y a un opérateur devant, le T est en 6ème position
      {
        nbT = SLselect[ i ][ 6 ];
        nbT2 = SLselect[ i ][ 7 ];
      }
      if(  nbT2 == "." )
        nb = strtoint( nbT );else
        nb = strtoint( nbT + nbT2 );
          // ! c'est une concatenation, pas une addition
      tabFrom[ nb ] = true;
    } // fin du if pos() multiple

    //Dans le cas d'une agreg calendaire, on a un num_agreg AS pentade ou mois, ou quinzaine...
    //On recupère ce qu'il y a aprés le AS pour avoir le pas
    if(  ( pos( "num_agreg AS " , SLselect[ i ] ) != 0 ) )
    {
      mode = stringReplace( copy( SLselect[ i ] , pos( " AS " , SLselect[ i ] ) + 3
        , Length( SLselect[ i ] ) ) , " " , "" , [  ] );
    }

  }} //Fin du for i:=0 to (SLselect.Count-1)do

  i = 1;
  while(  tabFrom[ i ] == false )
    ++i;

  result = result + ",T" + inttostr( i ) + ".jour";

  //mode non calendaire
//  if (mode<>'Pentade') AND (mode<>'Decade') AND (mode<>'Quinzaine') AND (mode<>'Mois') AND (tabFrom[1]=true) then
//  begin
//      result:=result+',T1.NumeroSimule';
//  end;
  if(  tabFrom[ 1 ] == true )
  {
    result = result + ",T1.NumeroSimule";
    result = result + " WHERE T1.idSimule=\"" +
      DBModule1.RequeteSimuleIdSimule.Value + "\"";
  }

    // Création de la clause WHERE avec le meme principe
    //Sauf que là, on découpe sur les AND et OR
  else
    result = result + " WHERE ";
  if(  ( ReqWhere2 != "" ) && ( ReqWhere2 != " " ) )
  {
    ReqWhere = stringReplace( ReqWhere2 , "WHERE" , "" , [ rfReplaceAll ] );
    ReqWhere = stringReplace( ReqWhere , "AND" , "§§AND" , [ rfReplaceAll ] );
    ReqWhere = stringReplace( ReqWhere , "OR" , "§§OR" , [ rfReplaceAll ] );

    SLwhere = TstringList.Create;
    SLwhere.Text = stringReplace( ReqWhere , "§§" , (char)13(char)10 , [ rfReplaceAll ] );
    { long i_end = ( SLwhere.Count - 1 )+1 ; for( i = 0 ; i < i_end ; ++i )
    {
      // if i=0 then result:=result+' WHERE ';

      if(  ( pos( "Resjour.IdSimule=" , SLwhere[ i ] ) == 0 ) && ( pos( "T1.IdSimule="
        , SLwhere[ i ] ) == 0 ) )
      {
        if(  ( tabFrom[ 1 ] == true ) && ( i == 0 ) )
          result = result + " AND ";
        //if (i<>0) then result:=result+' AND ';
        result = result + SLwhere[ i ];
        //mise à jour du TabFrom
        { long j_end = 16 ; for( j = 1 ; j < j_end ; ++j )
        {
          if(  pos( "T" + inttostr( j ) + "." , SLwhere[ i ] ) > 0 )
            tabFrom[ j ] = true;
        }}
      }
    }}
  }
return result;
}



//END
