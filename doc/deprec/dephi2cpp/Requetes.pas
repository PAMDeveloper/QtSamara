{*------------------------------------------------------------------------------
  Requetes
  Requetes du moteur de Sarrah

  @Author  CIRAD
  @Version v_3.0
-------------------------------------------------------------------------------}
unit Requetes;

interface
uses
  
  DBModule, Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms,
    Dialogs,
  Db, DBTables;

procedure initTemp(); //AP requete
procedure execRequeteScenar(const CptSimul: integer); // A Pas Requete
procedure requete(r: string); //AP requete
procedure BatchResJour(); //AP requete
procedure deriveResJour(); //AP requete
procedure EnlevederiveResJour(); //AP requete
function FormeReqTemp(ReqSel2, ReqWhere2: string): string;
//modif CB 30/01/07 copie proc dans UimportExport dans gestionbdd
procedure SupprEnreg(const maTable: TTable; nomCol, valChamps: string);
// suppression d'un enregistrement dans la table maTable.
// Condition : la valeur du champs correspondant à la colonne nomCol est égale à valChamps

var
  tabFrom: array[0..15] of boolean;
    //liste les tables qu'on a besoin de mettre dans le FROM
  cheminBdd, mode: string;

implementation

//modif CB 30/01/07 copie proc dans UimportExport dans gestionbdd

procedure SupprEnreg(const maTable: TTable; nomCol, valChamps: string);
// suppression d'un enregistrement dans la table maTable.
// Condition : la valeur du champs correspondant à la colonne nomCol est égale à valChamps
begin
  DBModule1.ReqSupprEnr.SQL.Clear;
  DBModule1.ReqSupprEnr.DatabaseName := 'DBResult';
  {     DBModule1.ReqSupprEnr.DatabaseName:=maTable.DatabaseName;
  }
  DBModule1.ReqSupprEnr.SQL.Add('DELETE FROM ' + maTable.Name);
  DBModule1.ReqSupprEnr.SQL.Add(' WHERE ' + nomCol + '=''' + valChamps + '''');

  DBModule1.ReqSupprEnr.ExecSQL;
end;

procedure initTemp();
var
  i: integer;
  F: TextFile;
begin
  //Recupération du chemin de la BDD
  DBModule1.ListeRequete.First;
  DBModule1.Station.open;
  cheminBDD := DBModule1.Station.Database.Directory;
  i := AnsiPos('DBEcosys', cheminBDD);
  cheminBDD := copy(cheminBDD, 0, i - 1) + 'DBEcosys\';

  // Copie de la structure de Resjour dans ResTemp par copie de la sauvegarde de ResJour
  CopyFile(Pchar(CheminBDD + 'DBResult\Resjour_vide.DB'), Pchar(CheminBDD +
    'DBTemp\resTemp.DB'), false); //false indique qu'on écrase si existe
  CopyFile(Pchar(CheminBDD + 'DBResult\Resjour_vide.PX'), Pchar(CheminBDD +
    'DBTemp\resTemp.PX'), false); //false indique qu'on écrase si existe

  // On filtre les requetes pour ne prendre que celles concernant le dossier courant
  DBModule1.Simule.Filter := 'Id=' +
    QuotedStr(DBModule1.RequeteSimuleIdSimule.Value);
  DBModule1.Simule.Filtered := true;
  DBModule1.ListeRequete.Filter := 'Dossier=' +
    QuotedStr(DBModule1.SimuleIdDossier.Value);
  DBModule1.ListeRequete.Filtered := true;
  DBModule1.Simule.Filtered := false;

  DBModule1.ListeRequete.First;
  while not DBModule1.listeRequete.eof do
  begin
    
    // et on le recréé
    ForceDirectories(cheminBDD + 'DBResult\Requetes\');
    AssignFile(F, cheminBDD + 'DBResult\Requetes\' +
      DBModule1.ListeRequete.FindField('IdRequete').Value + '.txt');
    ReWrite(F);
    Write(F, ' ');
    closeFile(F);
    DBmodule1.ListeRequete.Next;
  end;
end;

procedure execRequeteScenar(const CptSimul: integer);
// Execute toutes les requetes contenues dans ListeRequete pour la simulation en cours

var
  i, j, k: integer;
  F: TextFile;
  nom, req, reqFrom: string;
  virgule: boolean;
  XportTexte, XportTable: boolean;

begin
  //showmessage('woohoo');
  // On filtre les requetes pour ne prendre que celles concernant le dossier courant
  DBModule1.Simule.Filter := 'Id=' +
    QuotedStr(DBModule1.RequeteSimuleIdSimule.Value);
  DBModule1.Simule.Filtered := true;
  DBModule1.ListeRequete.Filter := 'Dossier=' +
    QuotedStr(DBModule1.SimuleIdDossier.Value);
  DBModule1.ListeRequete.Filtered := true;
  DBModule1.Simule.Filtered := false;

  DBModule1.ResTemp.active := false;
  DBModule1.ResJour.active := false;
  DbModule1.ListeRequete.open;
  DbModule1.ListeRequete.First;
  i := 1;

  // On selectionne dans la table requetes les lignes qui ont un Id égal à celui de la table ListeRequete
  while not DbModule1.ListeRequete.Eof do
  begin
    DBModule1.TableRequete.Filter := 'Id=' +
      QuotedStr(DBmodule1.ListeRequete.FindField('IdRequete').Value);
    DBModule1.TableRequete.Filtered := true;
    DBModule1.TableRequete.open;
    DBModule1.TableRequete.First;

    for j := 0 to 15 do
    begin
      tabFrom[j] := false;
    end;

    if DBModule1.TableRequete.FieldByName('Id').value = ' Résultats journaliers'
      then
    begin
      if DbModule1.ListeRequete.FieldByName('Table').AsBoolean = true then
      begin
        EnleveDeriveResjour;
        BatchResJour();
        DeriveResJour();
      end;

      if DbModule1.ListeRequete.FieldByName('Texte').AsBoolean = true then
      begin
        DBModule1.QueryTemp.close;
        DBModule1.QueryTemp.DatabaseName := 'DBTemp';
        DBModule1.QueryTemp.SQL.clear;
        DBModule1.QueryTemp.SQL.add('SELECT * FROM ResTemp');
        DBModule1.QueryTemp.open;

        DBModule1.QueryTemp.DisableControls;
        
        AssignFile(F, cheminBDD + '\DBResult\Requetes\' +
          DBModule1.ListeRequete.FindField('IdRequete').Value + '.txt');
        Append(F);
        // Ecriture des noms des champs
        if cptSimul = 0 then
        begin
          Write(F, 'Id_Simule' + chr(9));
          for j := 0 to DBModule1.QueryTemp.FieldCount - 1 do
            Write(F, DBModule1.QueryTemp.Fields[j].DisplayName + chr(9));
          writeln(F, '');
        end;
        for k := 0 to DBModule1.QueryTemp.RecordCount - 1 do
        begin
          Write(F, DBModule1.RequeteSimuleIdSimule.Value + chr(9));
          for j := 0 to DBModule1.QueryTemp.FieldCount - 1 do
            Write(F, DBModule1.QueryTemp.Fields[j].AsString + chr(9));
          writeln(F, '');
          DBModule1.QueryTemp.Next;
        end;
        DBModule1.QueryTemp.EnableControls;
        CloseFile(F);
      end;

    end
    else
    begin
      if (DBModule1.TableRequete.FieldByName('Req_where').IsNull) then
        req :=
          formeReqTemp(DBModule1.TableRequete.FieldByName('Req_select').value, '')
      else
        req :=
          formeReqTemp(DBModule1.TableRequete.FieldByName('Req_select').value,
          DBModule1.TableRequete.FieldByName('Req_where').value);
      reqFrom := ' FROM ';
      if (DBModule1.TableRequete.FieldByName('Pas_nc').value <> 0) and
        (pos(DBModule1.TableRequete.FieldByName('Champ_nc').value, req) = 0) then
        req := stringReplace(req, 'SELECT ', 'SELECT ' +
          DBModule1.TableRequete.FieldByName('Champ_nc').value + ',', []);
      // Creation de la clause FROM en fonction du TabFrom

      virgule := false;
      for j := 0 to 15 do
      begin
        if (TabFrom[j] = true) then
        begin
          DBModule1.TableEntity.Close;
          DBModule1.TableEntity.Filter := '';
          DBModule1.TableEntity.Filter := 'NumTable = ' + inttostr(j);
          DBModule1.TableEntity.Filtered := true;
          DBModule1.TableEntity.open;
          DBModule1.TableEntity.First;
          if virgule = true then
            reqFrom := reqFrom + ' , ';

          //On remplace ResJOur par ResTemp
          if j = 1 then
            reqFrom := reqFrom + '"' + cheminBDD + 'DBTemp\ResTemp" AS T1'
              //Pour les autres tables, on récupère leur nom et la base à laquelle elles appartienent sur TableEntity
          else
            reqFrom := reqFrom + '"' + cheminBDD +
              DBModule1.TableEntity.FieldByName('DataBaseSource').value + '\' +
              DBModule1.TableEntity.FieldByName('Source').value + '" AS T' +
              inttostr(j);

          virgule := true;
        end;
      end; //fin for j0 to 15

      //Concaténation de la clause FROM au reste de la requête
      if pos('WHERE', req) > 0 then
        req := stringReplace(req, 'WHERE', reqFrom + ' WHERE', [])
      else
        req := req + reqFrom;

      req := stringReplace(req, 'DBResult\Resjour', 'DBTemp\ResTemp',
        [rfReplaceAll]);
      req := stringReplace(req, 'Resjour', 'ResTemp', [rfReplaceAll]);

      if DBmodule1.TableRequete.FieldByName('Type').Value = 'Echantillon' then
        req := stringReplace(req, 'FROM', ',' +
          DBmodule1.TableRequete.FieldByName('Champ_nc').Value + ' FROM',
          [rfReplaceAll]);

      DBmodule1.resTemp.open;
      //MainForm.memDeroulement.Lines.Add(inttostr(i)+') '+DBmodule1.ListeRequete.FindField('IdRequete').Value);

      DBModule1.resTemp.close;
      DBModule1.QueryTemp.close;

      DBModule1.QueryTemp.DatabaseName := 'DBTemp';
      DBModule1.QueryTemp.SQL.clear;
      DBModule1.QueryTemp.SQL.add(req);
      if (DBModule1.TableRequete.FieldByName('Type').value = 'Normal') then
        req := req + DBmodule1.TableRequete.FindField('Req_Group').Value;
      DBModule1.QueryTemp.open;

      //if (DBModule1.QueryTemp.FieldCount=0) AND (DBModule1.QueryTemp.RecordCount=0)then
      //    MainForm.memDeroulement.Lines.add('Pas de résultats pour cette requête');

      if (DBModule1.TableRequete.FieldByName('Type').value <> 'Normal') then
      begin
        // On copie le résultat de la requête temporaire dans ResultRequete
        //C'est sur cette table qu'on va travailler (ajout des champs...)
        DBModule1.ResultRequete.Close;
        DBModule1.BatchMoveTemp.Source := DBModule1.QueryTemp;
        DBModule1.ResultRequete.TableName := 'ResultRequete';
        DBModule1.ResultRequete.DataBaseName := 'DBRequete';
        DBModule1.BatchMoveTemp.Destination := DBModule1.ResultRequete;
        DBModule1.BatchMoveTemp.Mode := batcopy;
        DBModule1.BatchMoveTemp.Execute;
        DBModule1.QueryTemp.close;
        DBModule1.ResultRequete.Close;

        DBModule1.QueryTemp.DatabaseName := 'DBRequete';

        requete('ALTER TABLE ResultRequete ADD annee INTEGER');
        requete('ALTER TABLE ResultRequete ADD Prem_jour DATE');
        requete('ALTER TABLE ResultRequete ADD modulo INTEGER');
        requete('ALTER TABLE ResultRequete ADD num_agreg INTEGER');

        if DBmodule1.TableRequete.FieldByName('Type').Value = 'Echantillon' then
        begin
          requete('UPDATE ResultRequete SET modulo = (' +
            DBModule1.TableRequete.FieldByName('Champ_nc').value + ' / ' +
            inttostr(DBModule1.TableRequete.FieldByName('Pas_nc').value) + ')');

          //Création d'une table annexe contenant seulement les valeur de la variable servant de pas
          //pour chaque pas de l'échantillon
          DBModule1.QueryTemp.SQL.Clear;
          if tabfrom[1] = true then
            DBModule1.QueryTemp.SQL.Add('SELECT MIN(jour) AS jour, MIN(' +
              DBModule1.TableRequete.FieldByName('Champ_nc').value + ')AS ' +
              DBModule1.TableRequete.FieldByName('Champ_nc').value +
              ' ,numeroSimule, modulo FROM ResultRequete GROUP BY numeroSimule, modulo');
          if tabfrom[1] = false then
            DBModule1.QueryTemp.SQL.Add('SELECT MIN(jour) AS jour,MIN(' +
              DBModule1.TableRequete.FieldByName('Champ_nc').value + ')AS ' +
              DBModule1.TableRequete.FieldByName('Champ_nc').value +
              ' , modulo FROM ResultRequete GROUP BY modulo');

          DBModule1.QueryTemp.ExecSQL;

          DBModule1.TableTemp.close;
          DBModule1.TableTemp.DatabaseName := 'DBRequete';
          DBModule1.TableTemp.TableName := 'Temp';
          DBModule1.BatchMoveTemp.Destination := DBModule1.TableTemp;
          DBModule1.BatchMoveTemp.Mode := BatCopy;
          DBModule1.BatchMoveTemp.execute;

          DBModule1.TableTemp.close;
          DBModule1.BatchMoveTemp.Destination := DBModule1.ResultRequete;

        end;

        if DBmodule1.TableRequete.FieldByName('Type').Value = 'Agregation' then
        begin
          if (mode <> 'Pentade') and (mode <> 'Decade') and (mode <> 'Quinzaine')
            and (mode <> 'Mois') then
          begin //mode non calendaire
            requete('UPDATE ResultRequete SET num_agreg = (' +
              DBModule1.TableRequete.FieldByName('Champ_nc').value + ' / ' +
              inttostr(DBModule1.TableRequete.FieldByName('Pas_nc').value) + ')');
          end

          else
          begin // Mode calendaire
            Requete('UPDATE ResultRequete SET Prem_jour = Jour');

            if (mode = 'Pentade') then
            begin
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-5, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))<=5');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-4, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>5 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=10');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-3, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>10 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=15');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-2, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>15 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=20');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6-1, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>20 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=25');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*6, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>25');
            end;

            if (mode = 'Decade') then
            begin
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*3-2, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))<=10');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*3-1, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>10 AND (EXTRACT(DAY FROM ResultRequete.Jour))<=20');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*3, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>20');
            end;

            if (mode = 'Quinzaine') then
            begin
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*2-1,annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))<=15');
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour)*2, annee=EXTRACT(YEAR FROM ResultRequete.Jour) WHERE (EXTRACT(DAY FROM ResultRequete.Jour))>15');
            end;

            if (mode = 'Mois') then
            begin
              Requete('UPDATE ResultRequete SET num_agreg = EXTRACT(MONTH FROM ResultRequete.Jour), annee=EXTRACT(YEAR FROM ResultRequete.Jour)');
            end;
          end; //fin calendaire
        end; //fin agreg
        req := '';
        if DBmodule1.TableRequete.FieldByName('Type').Value <> 'Echantillon'
          then
          req := DBmodule1.TableRequete.FindField('Req_Select').Value + ' FROM "'
            + cheminBDD + 'DBRequete\ResultRequete" AS T1 ' +
            DBmodule1.TableRequete.FindField('Req_group').Value
        else
        begin
          if tabfrom[1] = false then
            req := DBModule1.TableRequete.FieldByName('Req_select').value +
              ',min(Temp.jour),modulo FROM "' + cheminBDD +
              'DBRequete\ResultRequete" AS T1,"' + cheminBDD +
              'DBRequete\Temp" AS Temp WHERE T1.jour=Temp.jour AND T1.NumeroSimule=Temp.NumeroSimule ' +
              stringreplace(DBmodule1.TableRequete.FindField('Req_group').Value,
              'IdSimule,', '', [rfReplaceAll]);
          if (tabfrom[1] = true) and (pos('NumeroSimule',
            DBmodule1.TableRequete.FindField('Req_group').Value) = 0) then
            req :=
              stringreplace(DBModule1.TableRequete.FieldByName('Req_select').value,
              'T1.IdSimule,', '', [rfReplaceAll]) +
              ',numeroSimule,min(Temp.jour), modulo FROM "' + cheminBDD +
              'DBRequete\ResultRequete" AS T1,"' + cheminBDD +
              'DBRequete\Temp" AS Temp WHERE T1.jour=Temp.jour AND T1.NumeroSimule=Temp.NumeroSimule ' +
              stringReplace(stringreplace(DBmodule1.TableRequete.FindField('Req_group').Value,
              'T1.IdSimule,', '', [rfReplaceAll]), 'GROUP BY ',
              'GROUP BY T1.numeroSimule,', []);
          if tabfrom[1] = true and (pos('NumeroSimule',
            DBmodule1.TableRequete.FindField('Req_group').Value) > 0) then
            req :=
              stringreplace(DBModule1.TableRequete.FieldByName('Req_select').value,
              'T1.IdSimule,', '', [rfReplaceAll]) + ', T1.modulo,T1.Jour FROM "' +
              cheminBDD + 'DBRequete\ResultRequete" AS T1,"' + cheminBDD +
              'DBRequete\Temp" AS Temp WHERE T1.jour=Temp.jour AND T1.NumeroSimule=Temp.NumeroSimule  ' +
              stringreplace(DBmodule1.TableRequete.FindField('Req_group').Value,
              'T1.IdSimule,', '', [rfReplaceAll]); // AND T1.Modulo=Temp.Modulo

        end;
        for j := 2 to 15 do
        begin
          req := stringreplace(req, 'T' + intToStr(j) + '.', 'T1.',
            [rfReplaceAll]);
        end;

        DBModule1.QueryTemp.DatabaseName := 'DBRequete';
        DBModule1.QueryTemp.SQL.clear;
        if (mode = 'Pentade') or (mode = 'Decade') or (mode = 'Quinzaine') or
          (mode = 'Mois') and (pos(req, 'min(Jour)') = 0) then
          req := stringreplace(req, 'Select ', 'Select min(Jour) AS Jour,', []);
        //MainForm.memDeroulement.Lines.add(req);                 //affichage de la requête
        DBModule1.QueryTemp.SQL.add(req);
        DBModule1.QueryTemp.open;
      end; // fin if (DBModule1.TableRequete.FieldByName('Type').value<>'Normal') then

      //MainForm.memDeroulement.Lines.add('Requete terminée');
      XportTable := DbModule1.ListeRequete.FieldByName('Table').AsBoolean;
      XportTexte := DbModule1.ListeRequete.FieldByName('Texte').AsBoolean;
      //ecriture des résultats dans le fichier txt
      if (DBModule1.QueryTemp.FieldCount <> 0) and
        (DBModule1.QueryTemp.RecordCount <> 0) then
      begin
        if XportTexte then
        begin
          DBModule1.QueryTemp.DisableControls;
          //AssignFile(F, ExtractFilePath(Application.ExeName)+'\'+DBModule1.ListeRequete.FindField('IdRequete').Value+'.txt');
          AssignFile(F, cheminBDD + '\DBResult\Requetes\' +
            DBModule1.ListeRequete.FindField('IdRequete').Value + '.txt');
          Append(F);
          // Ecriture des noms des champs
          if cptSimul = 0 then
          begin
            Write(F, 'Id_Simule' + chr(9));
            for j := 0 to DBModule1.QueryTemp.FieldCount - 1 do
              Write(F, DBModule1.QueryTemp.Fields[j].DisplayName + chr(9));
            writeln(F, '');
          end;
          for k := 0 to DBModule1.QueryTemp.RecordCount - 1 do
          begin
            Write(F, DBModule1.RequeteSimuleIdSimule.Value + chr(9));
            for j := 0 to DBModule1.QueryTemp.FieldCount - 1 do
              Write(F, DBModule1.QueryTemp.Fields[j].AsString + chr(9));
            writeln(F, '');
            DBModule1.QueryTemp.Next;
          end;
          DBModule1.QueryTemp.EnableControls;
          CloseFile(F);
        end;

        if XportTable then
        begin
          nom := DBModule1.ListeRequete.FindField('IdRequete').Value;
          if cptSimul = 0 then
            CopyFile(Pchar(CheminBDD + 'DBRequete\Temp.DB'), Pchar(CheminBDD +
              'DBResult\Requetes\' + nom + '.DB'), false);
          DBModule1.TableTemp.close;
          DBModule1.TableTemp.DatabaseName := 'DBResultRequetes';
          DBModule1.TableTemp.TableName := nom;
          DBModule1.BatchMoveTemp.Destination := DBModule1.TableTemp;
          if cptSimul = 0 then
            DBModule1.BatchMoveTemp.Mode := batCopy
          else
            DBModule1.BatchMoveTemp.Mode := batAppend;
          DBModule1.BatchMoveTemp.Execute;
        end;
      end;
    end; // Fin du else (si Id=' Résultats Journaliers -> else)

    inc(i);
    DbModule1.ListeRequete.next;

  end; //fin listeRequete.EOF
   
      
  DBModule1.ListeRequete.Filtered := false;
 
end;

// Execute une requete qui modifie la base

procedure requete(r: string);
begin
  DBModule1.QueryTemp.SQL.Clear;
  DBModule1.QueryTemp.SQL.Add(r);
  DBModule1.QueryTemp.ExecSQL;
end;

procedure BatchResJour();
//Recopie si demandé les resultats dans Resjour
begin
  
  DBModule1.BatchmoveFinRequete.Mode := batAppendUpdate;
  DBModule1.BatchmoveFinRequete.execute;

end;

procedure deriveResjour();
// On derive le flot de ResJour vers ResTemp
begin
  //showmessage('debut deriveresjour');
  DBModule1.Resjour.Active := false;
  DBModule1.DSResJour.Enabled := false;
  DBModule1.ResTemp.Active := false;
  DBModule1.Resjour.DatabaseName := 'DBTemp';
  DBModule1.Resjour.TableName := 'ResTemp.db';
  DBModule1.DSResJour.enabled := true;
  DBModule1.Resjour.Active := true;
  
end;

procedure EnleveDeriveResjour();
// On remet en place le flot de ResJour
begin
  DBModule1.Resjour.Active := false;
  DBModule1.DSResJour.Enabled := false;
  DBModule1.ResTemp.Active := false;
  DBModule1.Resjour.DatabaseName := 'DBResult';
  DBModule1.Resjour.TableName := 'ResJour';
  DBModule1.DSResJour.enabled := true;
  DBModule1.Resjour.Active := True;
end;

//Formation de la requete temporaire

function FormeReqTemp(ReqSel2, ReqWhere2: string): string;
var
  SLselect, SLwhere: TstringList;
  ReqSel, ReqWhere: string;
  nb, i, j: integer;
  nbT, nbT2: char;

begin
  result := 'SELECT ';
  mode := '';

  // On décompose la requête en la découpant en tronçons au niveau des virgules
  reqSel := StringReplace(reqSel2, 'Select', '', []);
  SLselect := TStringList.Create;
  SLselect.Text := StringReplace(reqSel, ',', #13#10, [rfReplaceAll]);

  //Ici, chaque partie de la requête est dans une case de la StringList
  //Mais on ne prends pas les champs num_agreg, année, ... qui seront ajoutés à la table temporaire plus tard
  for i := 0 to (SLselect.Count - 1) do
  begin
    if (pos(' AS ', SLselect[i]) = 0) and (pos('num_agreg ', SLselect[i]) = 0)
      and (pos('annee ', SLselect[i]) = 0) and (pos('T', SLselect[i]) <> 0) and
      (pos('jour', SLselect[i]) = 0) and (pos('numeroSimule ', SLselect[i]) = 0)
      then
    begin
      SLselect[i] := StringReplace(SLselect[i], ' ', '', [rfReplaceAll]);
      if i <> 0 then
        result := result + ', ';

      if (pos('(', SLselect[i]) = 0) then
      begin
        if pos(SLSelect[i] + ',', result) = 0 then
          result := result + SLSelect[i]
        else
          result := copy(result, 0, Length(result) - 2);
      end;

      // Si il y a une parenthèse -> c'est une fonction
      //on ne prend que ce qu'il y a entre ( et )
      if (pos('(', SLselect[i]) > 0) then
      begin
        if pos(StringReplace(copy(SLselect[i], pos('(', SLselect[i]) + 1,
          Length(SLselect[i])), ')', '', [rfReplaceAll]) + ',', result) = 0 then
          result := result + StringReplace(copy(SLselect[i], pos('(',
            SLselect[i]) + 1, Length(SLselect[i])), ')', '', [rfReplaceAll])
        else
          result := copy(result, 0, Length(result) - 2);
      end;

      //mise à jour du TabFrom
      // On recupère les 2 chiffres aprés le T, et on les accole
      if (SLselect[i][1] = 'T') then
      begin
        nbT := SLselect[i][2];
        nbT2 := SLselect[i][3];
      end
      else //Si il y a un opérateur devant, le T est en 6ème position
      begin
        nbT := SLselect[i][6];
        nbT2 := SLselect[i][7];
      end;
      if nbt2 = '.' then
        nb := strtoint(nbt)
      else
        nb := strtoint(nbt + nbt2);
          // ! c'est une concatenation, pas une addition
      tabFrom[nb] := true;
    end; // fin du if pos() multiple

    //Dans le cas d'une agreg calendaire, on a un num_agreg AS pentade ou mois, ou quinzaine...
    //On recupère ce qu'il y a aprés le AS pour avoir le pas
    if (pos('num_agreg AS ', SLselect[i]) <> 0) then
    begin
      mode := StringReplace(copy(SLselect[i], pos(' AS ', SLselect[i]) + 3,
        Length(SLselect[i])), ' ', '', []);
    end;

  end; //Fin du for i:=0 to (SLselect.Count-1)do

  i := 1;
  while tabFrom[i] = false do
    inc(i);

  result := result + ',T' + inttostr(i) + '.jour';

  //mode non calendaire
//  if (mode<>'Pentade') AND (mode<>'Decade') AND (mode<>'Quinzaine') AND (mode<>'Mois') AND (tabFrom[1]=true) then
//  begin
//      result:=result+',T1.NumeroSimule';
//  end;
  if tabFrom[1] = true then
  begin
    result := result + ',T1.NumeroSimule';
    result := result + ' WHERE T1.idSimule="' +
      DBModule1.RequeteSimuleIdSimule.Value + '"';
  end

    // Création de la clause WHERE avec le meme principe
    //Sauf que là, on découpe sur les AND et OR
  else
    result := result + ' WHERE ';
  if (reqWhere2 <> '') and (reqWhere2 <> ' ') then
  begin
    reqWhere := StringReplace(reqWhere2, 'WHERE', '', [rfReplaceAll]);
    reqWhere := StringReplace(reqWhere, 'AND', '§§AND', [rfReplaceAll]);
    reqWhere := StringReplace(reqWhere, 'OR', '§§OR', [rfReplaceAll]);

    SLwhere := TStringList.Create;
    SLwhere.Text := StringReplace(reqWhere, '§§', #13#10, [rfReplaceAll]);
    for i := 0 to (SLwhere.Count - 1) do
    begin
      // if i=0 then result:=result+' WHERE ';

      if (pos('Resjour.IdSimule=', SLwhere[i]) = 0) and (pos('T1.IdSimule=',
        SLwhere[i]) = 0) then
      begin
        if (tabFrom[1] = true) and (i = 0) then
          result := result + ' AND ';
        //if (i<>0) then result:=result+' AND ';
        result := result + SLwhere[i];
        //mise à jour du TabFrom
        for j := 1 to 15 do
        begin
          if pos('T' + InttoStr(j) + '.', SLwhere[i]) > 0 then
            tabFrom[j] := true;
        end;
      end;
    end;
  end;
end;

end.

