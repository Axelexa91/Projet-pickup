



#include <conio.h>
#include <ctype.h>
#include "stockentrepot.h"
#define TAILLE_MAX 128


/******************************************************************************************************************************/
/* Fonction qui permet de d�couper une ligne d'un fichier texte en fonction d'un s�parateur pour la fonction charger commande */
/******************************************************************************************************************************/
int lire_champ_suivant2(char *ligne, int *idx, char *champ, int taille_champ, char separateur)
{
	int idx2 = 0;		//Curseur au sein du mot que l'on souhaite remplir
	while ((idx2 < (taille_champ - 1)) && (ligne[*idx] != separateur) && (ligne[*idx] != '\0'))		//On r�cup�re toutes les lettres contenues entre les s�parateur, avec une limite de taille
	{
		champ[idx2] = ligne[*idx];	//La lettre est copi�e dans le champ souhait�
		idx2 += 1;
		*idx += 1;
	}
	if ((ligne[*idx] == separateur) || (ligne[*idx] == '\0'))
	{
		champ[idx2] = 0;	/* fin de chaine sur caractere suivant */
		return(OK);
	}
	else return(ERROR);		/* fin de ligne ou s�parateur non atteints */

} /* fin lire_champ_suivant() */


  /*****************************************************************************************************************************/
  /*---Fonction qui permet de d�couper une ligne d'un fichier texte en fonction d'un s�parateur pour la fonction charger client*/
  /*****************************************************************************************************************************/
int lire_champ_suivant(char *ligne, int *index, char *champ, int taille_champ, char separateur)
{
	int index2 = 0;  //Curseur au sein du mot que l'on souhaite remplir
	while ((index2 < (taille_champ - 1)) && (ligne[*index] != separateur))    //On r�cup�re toutes les lettres contenues entre les s�parateur, avec une limite de taille
	{
		champ[index2] = ligne[*index];  //La lettre est copi�e dans le champ souhait�
		index2 += 1;
		*index += 1;
	}
	if ((ligne[*index] == separateur) || (ligne[*index] == '\0'))
	{
		champ[index2] = 0;	/* fin de chaine sur caractere suivant */
		return(OK);
	}
	else return(ERROR);		/* fin de ligne ou s�parateur non atteints */
}



/*********************************************************************************************************/
/* Fonction qui permet de charger le fichier texte client dans la liste chain�e qui contient les clients */
/*********************************************************************************************************/
int charger(ClientFile * Fichierclient, char nom_fichier[])
{
	FILE *fic_client;					/* le fichier */
	errno_t err;
	int nombre_client = 0;				/* index sur enregistrements */
	int long_max_rec = sizeof(Client);
	char buffer[sizeof(Client) + 1];
	int index = 0;
	char IDtmp[5];     //Tableau servant de tampon pour r�cup�rer l'ID en tant que chaine de caract�res pour le transformer apr�s en int
	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_client, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_client))
		{
			if (fgets(buffer, long_max_rec, fic_client) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
				{
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */
				}

				index = 0;								/* analyse depuis le debut de la ligne */

				ClientSeul *nouv_element;
				nouv_element = (ClientSeul *)malloc(sizeof(ClientSeul)); //On alloue au nouvel �l�ment une place suffisante en m�moire pour contenir un enregistrement
				nouv_element->next = NULL;

				if (lire_champ_suivant(buffer, &index, IDtmp, 5, SEPARATEUR) == OK)
				{
					nouv_element->client.ID = atoi(IDtmp);   //On transforme la chaine de caract�re en entier
					index++;

					if (lire_champ_suivant(buffer, &index, nouv_element->client.Nom, CLIENT_NOM_MAX, SEPARATEUR) == OK) //On r�cup�re la partie nom apr�s le s�parateur qu'on place dans le nouvel �l�ment
					{
						index++;
						if (lire_champ_suivant(buffer, &index, nouv_element->client.Prenom, CLIENT_PRENOM_MAX, SEPARATEUR) == OK) // On fait la m�me chose pour le pr�nom et les autres �l�ments
						{
							index++;
							if (lire_champ_suivant(buffer, &index, nouv_element->client.Adresse, CLIENT_ADRESSE_MAX, SEPARATEUR) == OK)
							{
								index++;
								if (lire_champ_suivant(buffer, &index, nouv_element->client.Ville, CLIENT_VILLE_MAX, SEPARATEUR) == OK)
								{
									index++;
									if (lire_champ_suivant(buffer, &index, nouv_element->client.CodePostal, CLIENT_CodePostal_MAX, SEPARATEUR) == OK)
									{
										index++;
										if (lire_champ_suivant(buffer, &index, nouv_element->client.NumeroTel, CLIENT_TELEPHONE_MAX, SEPARATEUR) == OK)
										{
											index++;
											if (lire_champ_suivant(buffer, &index, nouv_element->client.MotDePasse, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK)
											{
												nombre_client++;
											}
										}
									}
								}
							}
						}
						/* element � priori correct, on le comptabilise */
					}
				}
				if (Fichierclient->BDDclient->size == 0) //Si la liste est vide on fait pointer la t�te de liste sur cet �l�ment
				{
					Fichierclient->BDDclient->head = nouv_element;
					//printf("%d", nouv_element->client.ID);
				}
				else
				{
					Fichierclient->BDDclient->tail->next = nouv_element; //Si la liste est non vide on fait pointer l'�l�ment suivant la queue de liste sur cet �l�ment
				}
				Fichierclient->BDDclient->tail = nouv_element; //La queue pointe dans tous les cas sur l'�l�ment ajout�
				Fichierclient->BDDclient->size++;
				Fichierclient->NombreDeClient++;
			}
		}fclose(fic_client);
		return(OK);
	}
}


/*******************************************************************************************************/
/* Fonction qui permet de charger le fichier texte commande dans le tableau qui contient les commandes */
/*******************************************************************************************************/
int ChargerCommande(CommandeFile * commande, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = 4 * sizeof(Commande);
	char buffer[4 * sizeof(Commande) + 1];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep) && (commande->NombreDeCommande < COMMANDEFILE_MAX))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
				char mot[30]; // on cr�e un tableau dans lequel on r�cup�re les chaines de caract�re du fichier texte 

				if (lire_champ_suivant2(buffer, &idx, mot, CLIENT_ID_MAX, SEPARATEUR) == OK) // on r�cup�re la partie Client_ID jusqu'au s�parateur puis on la place dans mot
				{
					idx++;
					commande->TableauDesCommandes[num_rec].ClientID = atoi(mot);        //on transforme la chaine de caract�re en entier et on l'enregistre dans le tableau de commande dans la partie ClientID
					if (lire_champ_suivant2(buffer, &idx, mot, ARTICLE_ID_MAX, SEPARATEUR) == OK) //on fait la m�me chose pour toutes les autres parties
					{
						idx++;
						commande->TableauDesCommandes[num_rec].ArticleID = atoi(mot);
						if (lire_champ_suivant2(buffer, &idx, mot, QUANTITY_MAX, SEPARATEUR) == OK)
						{
							idx++;
							commande->TableauDesCommandes[num_rec].Quantity = atoi(mot);
							if (lire_champ_suivant2(buffer, &idx, mot, CODE_RETRAIT_MAX, SEPARATEUR) == OK)
							{
								commande->TableauDesCommandes[num_rec].CodeRetrait = atoi(mot);
								num_rec++;
							}
						}
					}
				}
			}
		}
		commande->NombreDeCommande = num_rec;
		fclose(fic_rep);
		return(OK);
	}
}


/**********************************************************************************************************/
/*Fonction qui intialise le nombre d��l�ments � z�ro, charge le fichier pour la liste chain�e des clients */
/**********************************************************************************************************/
int init_BDDclient(ClientFile *FichierClient)
{
	int errno;
	void * tmpPtr;
	FichierClient->NombreDeClient = 0;    //On initialise le nombredeclient d'une base vide � 0
	tmpPtr = (void *)malloc(CLIENTFILE_MAX * sizeof(ListeClient));
	if (tmpPtr != NULL) {
		FichierClient->BDDclient = (ListeClient*)tmpPtr;
		FichierClient->BDDclient->head = NULL;  //Si la base de donn�e est vide la t�te pointe sur un �l�ment nul
		FichierClient->BDDclient->size = 0;
		FichierClient->BDDclient->tail = NULL;   //Pareil pour la queue
	}
	else return(-1);
	errno = charger(FichierClient, "client.txt");  //Il faut charger le fichier client existant dans la liste chain�e 
	return(0);
}


/******************************************************************************************************/
/*Fonction qui intialise le nombre d��l�ments � z�ro, charge le fichier pour le tableau des commandes */
/******************************************************************************************************/
int init_commande(CommandeFile *commande) {
	void *tmpPtr2;
	commande->NombreDeCommande = 0;   //On initialise le nombre de commande d'une base vide � 0
	tmpPtr2 = (void *)malloc(COMMANDEFILE_MAX * sizeof(Commande));  //on alloue la place dans la m�moire pour le tableau
	if (tmpPtr2 != NULL) {
		commande->TableauDesCommandes = (Commande *)tmpPtr2;
	}
	else return(-1);
	errno = ChargerCommande(commande, "commande.txt"); //On charge le fichier client existant dans le tableau
	return(0);
}


/**********************************************************************************************/
/* Fonction qui permet de rechercher la position d'un client dans la liste chain� des clients */
/**********************************************************************************************/
int recherche(int nbrcommande, int IDClient, ClientSeul *client, ClientFile *listeclient) {
	client = listeclient->BDDclient->head; //On initialise le client sur la t�te de la liste 
	for (int j = 0; j < nbrcommande; j++) {
		if (IDClient == client->client.ID) { //On regarde � quel client correspond la commande
			return(j);		//Une fois trouv� on revoit sa position dans la liste chain�e
		}
		client = client->next; //Sinon on regarde avec le client d'apr�s
	}
	return(ERROR);
}


/************************************************************************/
/* Fonction qui permet de sauvegarder la commande dans le fichier voulu */
/************************************************************************/
int sauvegarder(CommandeFile *commande, char nom_fichier[], int i)
{
	FILE *fic_rep;					/* le fichier */
	int openSucces = fopen_s(&fic_rep, nom_fichier, "a");  // On ouvre le fichier en mode "ajout" donc on �crit � la suite de ce qu'il y a dans le fichier texte
	if (openSucces == 0) {
		fprintf(fic_rep, "%04d", commande->TableauDesCommandes[i].ClientID); fprintf(fic_rep, ";");  //on �crit ID du client et le s�parateur 
		fprintf(fic_rep, "%04d", commande->TableauDesCommandes[i].ArticleID); fprintf(fic_rep, ";");	//on fait la m�me chose pour les autres parties
		fprintf(fic_rep, "%04d", commande->TableauDesCommandes[i].Quantity); fprintf(fic_rep, ";");
		fprintf(fic_rep, "%04d", commande->TableauDesCommandes[i].CodeRetrait); fprintf(fic_rep, ";\n");
	}
	fclose(fic_rep);  // On ferme le fichier 
	return(OK);
}


/************************************************************************/
/* Fonction qui trie les commandes en fonction du d�partement du client */
/************************************************************************/
void trie(CommandeFile *commande, ClientSeul *client, ClientFile *listeclient) {
	int pos, departement;
	char dep[2];
	for (int i = 0; i < commande->NombreDeCommande; i++) {
		client = listeclient->BDDclient->head;   //On initialise le client � la t�te de la liste 
		pos = recherche(commande->NombreDeCommande, commande->TableauDesCommandes[i].ClientID, client, listeclient); // on recherche la position du client dans la liste par rapport � l'ID du client qui a fait la commande
		for (int k = 0; k < pos; k++) {
			client = client->next;	//On met le pointeur client sur le client qui correspond � la commande
		}
		dep[0] = client->client.CodePostal[0], dep[1] = client->client.CodePostal[1]; //On isole le d�partement 
		departement = atoi(dep);  //On le transforme en entier
		switch (departement) { //On trie les commandes selon leurs d�partements
		case 59:
			sauvegarder(commande, "nord.txt", i);
			break;
		case 62:
			sauvegarder(commande, "pas_de_calais.txt", i);
			break;
		}
	}
}






int compterligne(const char * fichierlu) {
	int nombreligne = 0;
	char chaine[TAILLE_MAX] = "";
	FILE* fichiertest = NULL;
	int ok = fopen_s(&fichiertest, fichierlu, "r");


	while (fgets(chaine, TAILLE_MAX, fichiertest) != NULL) // On lit le fichier tant qu'on ne re�oit pas d'erreur (NULL)
	{
		nombreligne++;
	}
	fclose(fichiertest); // On ferme le fichier qui a �t� ouvert
	return(nombreligne);
}
float pourcentagecommandefait(Commande *demande, ArticleStocks *stockentrepot, int indiceentrepot, int(*quantit�manquante)[6], const char *fichier, const char *comandealias) {
	int taillefile = compterligne(comandealias);
	int taillestock = compterligne(fichier);
	int totalarticlecommande = 0;
	int totalarticlemanquant = 0;
	float pourcentagecommanderempli = 0;

	//%de remplisage

	for (int indicecommande = 0; indicecommande < taillefile; indicecommande++)
	{
		totalarticlecommande = demande[indicecommande].Quantity + totalarticlecommande;
		for (int indicestock = indiceentrepot; indicestock <indiceentrepot + 6; indicestock++)
		{


			if (demande[indicecommande].ArticleID == stockentrepot[indicestock].ArticleID)
			{


				if (demande[indicecommande].Quantity > stockentrepot[indicestock].Quantity)
				{


					(*quantit�manquante)[indicecommande] = demande[indicecommande].Quantity - stockentrepot[indicestock].Quantity;
					totalarticlemanquant = (*quantit�manquante)[indicecommande] + totalarticlemanquant;
					stockentrepot[indicestock].Quantity = 0;

				}
				else {

					(*quantit�manquante)[indicecommande] = 0;
					stockentrepot[indicestock].Quantity = stockentrepot[indicestock].Quantity - demande[indicecommande].Quantity;

				}


			}
		}
	}
	pourcentagecommanderempli = (1 - ((float)totalarticlemanquant / (float)totalarticlecommande));
	printf("%d %d %d", totalarticlemanquant, totalarticlecommande, indiceentrepot);

	return(pourcentagecommanderempli);
}
int fonction(const char *fichier, const char *comandealias,const char *fichierenvoitalias) {


	FILE* fichierenvoit = NULL;
	FILE* fichierecrit = NULL;
	ArticleStocks stockentrepot[18];
	ArticleStocks stockentrepot1[18];

	int nombrelignestock = compterligne(fichier);

	FILE* fichiertest = NULL;

	int ok1 = fopen_s(&fichiertest, fichier, "r");

	//d�but oubverture fichier ici............................................................................

	FILE* fichierCommande = NULL;
	Commande demande[TAILLE_MAX];
	Commande demande1[TAILLE_MAX];
	int nombrelignecommande = compterligne(comandealias);
	int ok2 = fopen_s(&fichierCommande, comandealias, "r");
	if (fichierCommande != NULL)

	{
		for (int i = 0; i < nombrelignecommande ; i++)
		{
			fscanf_s(fichierCommande, "%d;%d;%d;%d;", &demande[i].ClientID, &demande[i].ArticleID, &demande[i].Quantity, &demande[i].CodeRetrait);

		}
		fclose(fichierCommande); // On ferme le fichierCommande qui a �t� ouvert
		for (int j = 0; j < nombrelignecommande; j++)
		{
			printf(" c'est moi%d %d %d %d  \n", demande[j].ClientID, demande[j].ArticleID, demande[j].Quantity, demande[j].CodeRetrait);
		}
	}
	//fin ouverture fichier ici .............................................................................................s

	if (fichiertest != NULL)

	{
		for (int i = 0; i < nombrelignestock; i++)
		{


			fscanf_s(fichiertest, "%d;%d;%d;%d;%d;%d;", &stockentrepot[i].EntrepotID, &stockentrepot[i].ArticleID, &stockentrepot[i].Quantity, &stockentrepot[i].Hauteur, &stockentrepot[i].Largeur, &stockentrepot[i].Profondeur);

		}
		fclose(fichiertest); // On ferme le fichier qui a �t� ouvert
		for (int j = 0; j < nombrelignestock; j++)
		{
			printf(" c'est toi %d %d %d %d %d %d \n", stockentrepot[j].EntrepotID, stockentrepot[j].ArticleID, stockentrepot[j].Quantity, stockentrepot[j].Hauteur, stockentrepot[j].Largeur, stockentrepot[j].Profondeur);


		}

	}
	for (int i = 0; i < nombrelignestock; i++)
	{

		stockentrepot1[i].EntrepotID = stockentrepot[i].EntrepotID;
		stockentrepot1[i].ArticleID = stockentrepot[i].ArticleID;
		stockentrepot1[i].Quantity = stockentrepot[i].Quantity;
		stockentrepot1[i].Hauteur = stockentrepot[i].Hauteur;
		stockentrepot1[i].Largeur = stockentrepot[i].Largeur;
		stockentrepot1[i].Profondeur = stockentrepot[i].Profondeur;

	}
	for (int i = 0; i < compterligne(comandealias); i++)
	{

		demande1[i].Quantity = demande[i].Quantity;

	}
	// pourcentiration
	int nombreentrepot = 3;
	float tabpourcentage[6] = { 0 };
	float max = 0;
	int tabgrand[18] = { 0 };
	int iterate = 0;
	int etat = 0;
	float maxt = 0;
	float pourcentageA = 0;
	float pourcentagerestant = 1;
	while (etat == 0) {
		int entrepotactuel = -1;
		int quantit�manquante[6] = { 0 };
		iterate++;
		for (int indice1 = 0; indice1 < nombreentrepot; indice1++)
		{
			pourcentageA = pourcentagecommandefait(demande, stockentrepot, 6 * indice1, &quantit�manquante, fichier, comandealias);
			tabpourcentage[indice1] = pourcentageA;
			if (tabpourcentage[indice1]>max)
			{
				max = tabpourcentage[indice1];
				entrepotactuel = indice1;
			}
			printf("pourcentage %f ", pourcentageA);
			printf("indice1 %d \n", indice1);
		}

		for (int numeroentre = 0; numeroentre < 3; numeroentre++)
		{
			for (int i = numeroentre * 6; i < (numeroentre + 1) * 6; i++)
			{

				stockentrepot[i].EntrepotID = stockentrepot1[i].EntrepotID;
				stockentrepot[i].ArticleID = stockentrepot1[i].ArticleID;
				stockentrepot[i].Quantity = stockentrepot1[i].Quantity;
				stockentrepot[i].Hauteur = stockentrepot1[i].Hauteur;
				stockentrepot[i].Largeur = stockentrepot1[i].Largeur;
				stockentrepot[i].Profondeur = stockentrepot1[i].Profondeur;

			}
		}
		pourcentageA = pourcentagecommandefait(demande, stockentrepot, 6 * entrepotactuel, &quantit�manquante, fichier, comandealias);
		for (int i = 0; i < compterligne(fichier); i++)
		{

			stockentrepot1[i].EntrepotID = stockentrepot[i].EntrepotID;
			stockentrepot1[i].ArticleID = stockentrepot[i].ArticleID;
			stockentrepot1[i].Quantity = stockentrepot[i].Quantity;
			stockentrepot1[i].Hauteur = stockentrepot[i].Hauteur;
			stockentrepot1[i].Largeur = stockentrepot[i].Largeur;
			stockentrepot1[i].Profondeur = stockentrepot[i].Profondeur;

		}

		for (int i = 0; i < nombrelignecommande; i++)
		{
			printf("quantit� manquante %d \n", quantit�manquante[i]);
			demande[i].Quantity = quantit�manquante[i];

		}

		float ajout = max*pourcentagerestant;
		maxt = ajout + maxt;
		pourcentagerestant = 1 - maxt;
		printf("pourcentagerestant %f \n", pourcentagerestant);
		printf("ajout %f \n", ajout);
		printf("max %f \n", max);
		printf("maxt %f \n", maxt);


		if (entrepotactuel == -1)
		{
			etat = 1;
			printf("c'est la merde");
		}
		else if (max * 100 == 100)
		{
			etat = 1;
			printf("on est bon mon chou");
		}
		else if (iterate == 5) {
			etat = 1;
			printf("stop c'est un for");
		}
		max = 0;
	}


	int ok3 = fopen_s(&fichierecrit, "yolo.txt", "w");
	for (int i = 0; i < compterligne(fichier); i++) { //on ecrit chaque ligne jusqu'a avoir atteint le nombre de ligne du fichier
		fprintf(fichierecrit, "%d;%d;%d;%d;%d;%d;\n", stockentrepot[i].EntrepotID, stockentrepot[i].ArticleID, stockentrepot[i].Quantity, stockentrepot[i].Hauteur, stockentrepot[i].Largeur, stockentrepot[i].Profondeur);
	}
	fclose(fichierecrit); //FERMETURE
	
	int ok4 = fopen_s(&fichierenvoit, fichierenvoitalias, "w");
	for (int indicecommande = 0; indicecommande < compterligne(comandealias); indicecommande++)
	{

		for (int indicestock = 0; indicestock < 6; indicestock++)
		{


			if (demande[indicecommande].ArticleID == stockentrepot[indicestock].ArticleID)
			{
				fprintf(fichierenvoit, "%d;%d;%d;%d;%d;\n", stockentrepot[indicestock].ArticleID, demande1[indicecommande].Quantity, stockentrepot[indicestock].Hauteur, stockentrepot[indicestock].Largeur, stockentrepot[indicestock].Profondeur);
			}
		}
	}
	fclose(fichierecrit); //FERMETURE
	system("pause");
	return(EXIT_SUCCESS);
}

void clear_fichier(char nom_fichier[]) {
	FILE *fic_rep; // le fichier 
		int openSucces = fopen_s(&fic_rep, nom_fichier, "w");
	fclose(fic_rep);
}




/**********************************************************************/
/*                          Initialisation                            */
/**********************************************************************/

/*Cette fonction est activ�e � chaque d�but de fuseaux des 4h ainsi qu'� chaque nouvelle ex�cution.
Elle permet de cr�er un tableau dans lequel sera contenu toutes les commandes effectu�es durant les 4h.
Elle permet �galemnt d'initialiser le tableau avant de charger le contenu du fichier vente appel� "commande".*/
int init_tableau_commandes(CommandeFile* AllCommandes) {
	void*Ptr = (void*)malloc(COMMANDEFILE_MAX * sizeof(Commande)); //Cr�er un tableau
	if (Ptr != NULL) {
		AllCommandes->TableauDesCommandes = Ptr;
		AllCommandes->NombreDeCommande = 0;
		return OK;
	}
	else return ERROR;
}

/*Cette fonction est activ�e � chaque nouvelle Commande entr�e par l'utilisateur.
Elle permet de cr�er un tableau dans lequel sera contenu la commande en question
contenue dans le fichier FichierAchat.*/
int init_Commande(Commande* Com) {
	Com->ArticleID = 0;
	Com->ClientID = 0;
	Com->CodeRetrait = 0;
	Com->Quantity = 0;
	return OK;
}


/*Cette fonction est activ�e lors de la connexion du client ou apr�s la validation du panier.
Elle permet d'initialiser le panier dans lequel sera disposer ses commandes �ventuels.*/
int init_panier(CommandeFile* Panier) {
	void* Ptr = (void*)malloc(ARTICLEFILE_MAX * sizeof(Commande));
	if (Ptr != NULL) {
		Panier->TableauDesCommandes = Ptr;
		Panier->NombreDeCommande = 0;
		return OK;
	}
	else return ERROR;
}

/**********************************************************************/
/*                      Fonctions du panier                           */
/**********************************************************************/

/*Cette fonction permet d'ajouter une nouvelle commande au panier du client.*/
int ajout_panier(CommandeFile *Panier, int ID_client, int ID_article, int quantite) {

	// Cr�ation d'une nouvelle commande
	Commande nouvelarticle;

	nouvelarticle.ArticleID = ID_article;
	nouvelarticle.ClientID = ID_client;
	nouvelarticle.Quantity = quantite;
	srand(time(NULL));							// Cette fonction permet de g�n�rer un nombre al�atoire
	nouvelarticle.CodeRetrait = rand() % 10000;	// diff�rent � chaque �x�cution

												// Ajout de la commande au panier
	*(Panier->TableauDesCommandes + Panier->NombreDeCommande) = nouvelarticle;
	(Panier->NombreDeCommande)++;
	return OK;
}

/* Cette fonction permet de supprimer une commande du panier*/
int supprimer_dans_panier(CommandeFile *Panier, int indice) {
	for (int i = indice + 1; i <COMMANDEFILE_MAX; i++) {
		*(Panier->TableauDesCommandes + i - 1) = *(Panier->TableauDesCommandes + i);
	}
	Panier->NombreDeCommande--;
	return OK;
}

/* Cette fonction permet de valider la totalit� du panier, les commandes seront alors d�finitives.*/
int validation_panier(CommandeFile* AllCommandes, CommandeFile* Panier) {

	//Ajout du panier au tableau de toutes les commandes des 4h
	for (int indice = 0; indice < Panier->NombreDeCommande; indice++) {
		*(AllCommandes->TableauDesCommandes + AllCommandes->NombreDeCommande + indice) = *(Panier->TableauDesCommandes + indice);
	}

	// Mise � 0 du panier
	AllCommandes->NombreDeCommande += Panier->NombreDeCommande;
	init_panier(Panier);

	return(OK);
}

/*Cette fonction sera activ�e au bout des fuseaux de 4h elle permettra de mettre � 0 le fichier vente et de sauvegarder
les commandes de ces 4 derni�res heures dans celui-ci.*/
int envoi_commandes(CommandeFile *AllCommandes, char nom_fichier[]) {

	FILE *fic_rep;					/* le fichier */

	if ((fopen_s(&fic_rep, nom_fichier, "w")) != 0) {
		// Ouverture du fichier poss�dant le nom compris dans nom_fichier si il existe sinon cr�ation d'un fichier poss�dant ce nom
		// Mode d'utilisation "w" = write (�crire uniquement)
		// Si la fonction fopen_s renvoie autre chose que 0 cela signifie qu'elle n'a pas fonctionn�e
		return(ERROR);
	}
	else { // si la fonction fopen_s a fonctionn�e
		for (int i = 0; i < AllCommandes->NombreDeCommande; i++) {
			fprintf_s(fic_rep, "%04d;%04d;%04d;%04d;\n", (AllCommandes->TableauDesCommandes + i)->ClientID, (AllCommandes->TableauDesCommandes + i)->ArticleID, (AllCommandes->TableauDesCommandes + i)->Quantity, (AllCommandes->TableauDesCommandes + i)->CodeRetrait);
			// on affiche dans le fichier chaque �l�ment de toutes les commandes
		}
		fclose(fic_rep); // On ferme le fichier qui a �t� ouvert
		return(OK);
	}

}

/**********************************************************************/
/*                      Fonctions charger                             */
/**********************************************************************/



int charger_fichier_vente(CommandeFile* Fichier, char nom_fichier[])
{

	FILE *fic_rep;
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = 4 * (sizeof(Commande));
	char buffer[4 * (sizeof(Commande) + 1)];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) // si il y a une erreur lors de l'ouverture du fichier
														  // ouverture du fichier en mode "r" = read (lire uniquement)
	{
		return(ERROR);
	}
	else
	{
		while (!feof(fic_rep) && (Fichier->NombreDeCommande < COMMANDEFILE_MAX))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				char mot[10];
				idx = 0; /* analyse depuis le debut de la ligne */


				if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					Fichier->TableauDesCommandes[num_rec].ClientID = atoi(mot);
					if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
					{
						idx++;							/* on saute le separateur */
						Fichier->TableauDesCommandes[num_rec].ArticleID = atoi(mot);
						if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
						{
							idx++;							/* on saute le separateur */
							Fichier->TableauDesCommandes[num_rec].Quantity = atoi(mot);
							if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
							{
								Fichier->TableauDesCommandes[num_rec].CodeRetrait = atoi(mot);
								num_rec++;		/* element � priori correct, on le comptabilise */
							}
						}
					}
				}
			}
		}
		Fichier->NombreDeCommande = num_rec;
		fclose(fic_rep);
		return(OK);
	}
}
Commande charger_fichier_achat(Commande New, char nom_fichier[])
{

	FILE *fic_rep;
	errno_t err;
	int long_max_rec = 4 * (sizeof(Commande));
	char buffer[4 * (sizeof(Commande) + 1)];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) // si il y a une erreur lors de l'ouverture du fichier
														  // ouverture du fichier en mode "r" = read (lire uniquement)
	{
		return(New);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				char mot[10];
				idx = 0; /* analyse depuis le debut de la ligne */


				if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					New.ClientID = atoi(mot);
					if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
					{
						idx++;							/* on saute le separateur */
						New.ArticleID = atoi(mot);
						if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
						{
							New.Quantity = atoi(mot);

						}
					}
				}
			}
		}
		fclose(fic_rep);
		return(New);
	}
}

/* fin charger */


/**********************************************************************/
/*                      Fonction principale                           */
/**********************************************************************/

int Gestion_Achat(const char *FichierAchat) {
	CommandeFile AllCommandes; // Contient toutes les commandes des 4h
	CommandeFile Panier; // Contient toutes les commandes effectu�s par un client durant son achat
	Commande NewAchat;
	init_panier(&Panier);
	init_tableau_commandes(&AllCommandes);
	init_Commande(&NewAchat);
	charger_fichier_vente(&AllCommandes, "commande.txt");
	NewAchat = charger_fichier_achat(NewAchat, FichierAchat);
	ajout_panier(&Panier, NewAchat.ClientID, NewAchat.ArticleID, NewAchat.Quantity);
	validation_panier(&AllCommandes, &Panier);
	envoi_commandes(&AllCommandes, "commande.txt");
	return 1;
}
// On suppose a<b

int rand_a_b(int a, int b) {

	return rand() % (b - a) + a;

}
void mysleep(unsigned int tempsDelais)
{
	time_t tempsFin = time(NULL) + (time_t)tempsDelais;
	for (; time(NULL) < tempsFin; );
}

void compact(char *s)
{
	int decallage = 0, indice = 0;
	do
	{
		if ((*(s + indice) >= '9') || (*(s + indice) <= '0'))
		{
			//Dans ce cas il ne s'agit pas d'un chiffre et le d�callage sera augment� de 1 !!!
			decallage++;
		}
		else
		{
			*(s + indice - decallage) = *(s + indice);
		}
		indice++;
	} while ((*(s + indice) != '\0') && (indice < 21));
	if (indice < 21)
		*(s + indice - decallage) = '\0'; //ajout du cract�re de fin de mot
										  //On n'est pas oblig� de remettre les autres �l�ments � 0 CAR '\0' signifie qu'on est arriv� � la fin du mot.
	return;
}

int lire_champ_suivant1(char *ligne, int *idx, char *champ, int taille_champ, char separateur)
{
	int idx2 = 0;
	while ((idx2 < (taille_champ - 1)) && (ligne[*idx] != separateur)
		&& (ligne[*idx] != '\0'))
	{
		champ[idx2] = ligne[*idx];
		idx2 += 1;
		*idx += 1;
	}
	if ((ligne[*idx] == separateur) || (ligne[*idx] == '\0'))
	{
		champ[idx2] = 0;	/* fin de chaine sur caractere suivant */
		return(OK1);
	}
	else return(ERROR);		/* fin de ligne ou s�parateur non atteints */

} /* fin lire_champ_suivant1() */

  /***********************************************************/
  /************ Chargement du Fichier Ventes *****************/
  /***********************************************************/

VentesFile * newVentesFile()
{
	VentesFile *tmp = malloc(sizeof(VentesFile));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}

SingleVentesElement * newSingleVentesElement(Ventes value)
{
	SingleVentesElement  *Elem = malloc(sizeof(SingleVentesElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendVentesElement(VentesFile *MyList, Ventes value, int Size)
{
	SingleVentesElement *NewElem = newSingleVentesElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = (*MyList).Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerVentes(VentesFile *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Ventes)+4*sizeof(char);
	char buffer[sizeof(Ventes) + 4*sizeof(char)+ 1];
	int idx = 0;
	char *char_nw_line;
	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				Ventes newSlot = { -1, -1, -1, -1 }; //on cr�e un �l�ment de type contact dans lequel on va enregistrer les chaines de caract�res du fichier
				char mot[11];
				if (lire_champ_suivant1(buffer, &idx, mot, 10, SEPARATEUR) == OK1) //On v�rifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					newSlot.ClientID = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, 10, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
					{
						idx++;
						newSlot.ArticleID = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, 10, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
						{
							idx++;
							newSlot.Quantity = atoi(mot);
							if (lire_champ_suivant1(buffer, &idx, mot, 10, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
							{
								idx++;
								newSlot.CodeRetrait = atoi(mot);
								num_rec++;
							}
						}
					}
				}
				appendVentesElement(rep, newSlot, num_rec--);
			}
		}
		fclose(fic_rep);
		return(OK1);
	}
} /* fin charger */

int SauvegarderVentesNonTraitee(VentesFile *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingleVentesElement *myPtr = rep->head;
	//On place le premier �l�ment sur la t�te de la liste
	int Nb_elts = 1;
	//On peut faire �criture seul car on peut effacer les donn�es pr�sent dans le document s'il existe d�j�.
	//Le r�pertoire est en int�gralit� dans rep.
	if ((err = fopen_s(&fic_rep, nom_fichier, "w")) != 0)
	{
		return(ERROR);
	}
	else
	{
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d\n", myPtr->data.ClientID, SEPARATEUR, myPtr->data.ArticleID, SEPARATEUR, myPtr->data.Quantity, SEPARATEUR, myPtr->data.CodeRetrait);
			myPtr = myPtr->next;
			//On va vers l'�l�ment suivant. On n'aura pas de probl�me li� � un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistr� tous les �l�ments. On ne cherche donc pas � afficher l'�l�ment nb_elts (sui est le nb_elts+1 �me �l�ment du r�pertoire) qui a une adresse NULL car non d�finit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien pass�e
		return(OK1);
	}
}

/***************************************************************************/
/**********  Chargement fichiers disponibilit� PICKUP Stations *************/
/***************************************************************************/

SlotDispoFile * newSlotDispoFile()
{
	SlotDispoFile *tmp = malloc(sizeof(SlotDispoFile));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}

SingelSlotDispoElement * newSingelSlotDispoElement(SlotDispo value)
{
	SingelSlotDispoElement  *Elem = malloc(sizeof(SingelSlotDispoElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendSlotDispoElement(SlotDispoFile *MyList, SlotDispo value, int Size)
{
	SingelSlotDispoElement *NewElem = newSingelSlotDispoElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).size = (*MyList).size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerDispoSlot(SlotDispoFile *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(SlotDispo);
	char buffer[sizeof(SlotDispo) + 1];
	int idx = 0;
	char *char_nw_line;
	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				SlotDispo newSlot = { -1, -1, -1, -1 }; //on cr�e un �l�ment de type contact dans lequel on va enregistrer les chaines de caract�res du fichier
				char mot[10];
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					newSlot.ID_PickUP_Station = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
					{
						idx++;
						newSlot.ID_article = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
						{
							idx++;
							newSlot.quantite = atoi(mot);
							if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
							{
								idx++;
								newSlot.ID_slot = atoi(mot);
								num_rec++;
							}
						}
					}
				}
				appendSlotDispoElement(rep, newSlot, num_rec--);
			}
		}
		fclose(fic_rep);
		return(OK1);
	}
} /* fin charger */

  /************************************************/
  /**********  Chargement de la commande **********/
  /************************************************/

ListArticle * newListArticle()
{
	ListArticle *tmp = malloc(sizeof(ListArticle));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'�lement de type LinkedList mais aussi tous les �l�ments qui la constitue avec free! 

SingelArticleDansSlotElement* newArticleDansSlot(ArticleDansSlot value)
{
	SingelArticleDansSlotElement *Elem = malloc(sizeof(SingelArticleDansSlotElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendArticleDansSlot(ListArticle *MyList, ArticleDansSlot value, int Size)
{
	SingelArticleDansSlotElement *NewElem = newArticleDansSlot(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerCommande1(ListArticle *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(ArticleDansSlot);
	char buffer[sizeof(ArticleDansSlot) + 1];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				ArticleDansSlot tmp = { -1, -1, -1, -1, -1 };
				char mot[10];
				//On d�finit un enregistrement dans lequel on va enregistr� le nom, le pr�nom et le num�ro de t�l�phone lu sur la ligne enregistr� dans le buffer
				if (lire_champ_suivant1(buffer, &idx, mot, MAX_IDArticle + 1, SEPARATEUR) == OK1)//Probl�me dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.IDArticle = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, MAX_Article + 1, SEPARATEUR) == OK1)
					{
						idx++;
						tmp.quantite = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
						{
							idx++;
							tmp.largeur = atoi(mot);
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivant1(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
							{
								idx++;
								tmp.profondeur = atoi(mot);
								if (lire_champ_suivant1(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
									tmp.hauteur = atoi(mot);
								num_rec = appendArticleDansSlot(rep, tmp, num_rec);      /* element � priori correct, on le comptabilise */
																						 //num_rec contient la taille de la liste cha�n�e
							}
						}
					}
				}
			}
		}
		fclose(fic_rep);
		//ON ferme le fichier car on a lu toute les lignes
		return(EXIT_SUCCESS);
	}
} /* fin charger */

  /**************************************************************************/
  /************* Trie des Ventes par Dim article et quantit� ****************/
  /**************************************************************************/

int set(VentesFile *MyList, int i, Ventes value, int Size)//�quivalent � insert mais sans le d�callage
{
	int j = 0;
	if ((MyList != NULL) && (i < Size) && (i >= 0))
	{
		SingleVentesElement *tmp = (*MyList).head;
		j = 1;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'�l�ment i
		(*tmp).data = value;
		return(Size);
	}
	return(-1);
}

SingleVentesElement * GetElementAt(VentesFile *MyList, int i, int Size)
{
	int j;
	if ((MyList != NULL) && (i + 1 <= Size) && (i + 1 > 0))
	{
		SingleVentesElement *tmp = (*MyList).head;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'�l�ment i
		return(tmp);
	}
	return(NULL);
}

int swap(VentesFile *MyList, int i, int j, int Size)
{
	int tmp_int;
	if (j < i)
	{
		tmp_int = j;
		j = i;
		i = tmp_int;
	}
	if ((MyList != NULL) && (j < Size) && (i >= 0))
	{
		SingleVentesElement *tmp1 = (GetElementAt(MyList, i, Size));
		SingleVentesElement *tmp2 = (GetElementAt(MyList, j, Size));
		Ventes err1, err2;
		if ((tmp1 != NULL) && (tmp2 != NULL))
		{
			err1 = (*tmp1).data;
			err2 = (*tmp2).data;
			set(MyList, i, err2, Size); //On met l'entier � la pos j en i
			set(MyList, j, err1, Size); //On met la valeur qui �tait avant en i � la position j
										//Remarque - On ne change pas les adresses ce qui fait qe la cha�ne est toujours parcouru dans le m�me ordre.
										//On a fait un simple �change des donn�es transport�es dans les maillons i et j

			return(1);
		}
		return(0);
	}
	return(0);
}

int VolumeArticle(ListArticle * ListArticles, int IdArticle)
{
	if (ListArticles == NULL)
		return(ERROR);
	else
	{
		int trouver = ERROR;//condition qui test si on a trouver l'�l�ment
		int nb_elts = 0; //nombre d'�l�ments parcourues
		SingelArticleDansSlotElement * ptr = ListArticles->head;
		while ((trouver == ERROR) && (nb_elts < ListArticles->Size))
		{
			if (ptr->data.IDArticle == IdArticle)
			{
				trouver = OK1;
				return((ptr->data.largeur)*(ptr->data.profondeur)*(ptr->data.hauteur));
			}
			else
			{
				ptr = ptr->next;
			}
			nb_elts++;
		}
		return(ERROR);
	}
}

//On commence par ranger les articles les plus petits pour ne pas r�server de slots trop grang alors qu'on en aurait des plus petits capables d'acceuillir le colis
//On trie du plus petit article et de la quantit� la moins importante � l'article le plus grand command� en plus rande capacit�
int TrieVente(VentesFile *ListVentes, ListArticle * ListArticles)
{
	if ((ListVentes == NULL) || (ListArticles == NULL))
		return(ERROR);
	else
	{
		for (int i = 1; i < ListVentes->Size; i++)
		{
			SingleVentesElement * ptr = ListVentes->head;
			for (int j = 1; j < ListVentes->Size - i; j++)
			{
				//On met le plus grand article au bout...
				if ((VolumeArticle(ListArticles, ptr->data.ArticleID)) >(VolumeArticle(ListArticles, ptr->next->data.ArticleID)))
					swap(ListVentes, j, j + 1, ListVentes->Size);
				else
				{
					if ((VolumeArticle(ListArticles, ptr->data.ArticleID)) == (VolumeArticle(ListArticles, ptr->next->data.ArticleID)))
					{
						if (ptr->data.Quantity > ptr->next->data.Quantity)
						{
							swap(ListVentes, j, j + 1, ListVentes->Size);
						}
					}
				}
			}
			ptr = ptr->next;
		}
		return (OK1);
	}
}

/************************************************/
/********** Chargement fichier Client ***********/
/************************************************/

ListClient * newListClient()
{
	ListClient *tmp = malloc(sizeof(ListClient));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}

SingleClientElement* newClientElement(Client value)
{
	SingleClientElement *Elem = malloc(sizeof(SingleClientElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendClientElement(ListClient *MyList, Client value, int Size)
{
	SingleClientElement *NewElem = newClientElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerClient(ListClient *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Client);
	char buffer[sizeof(Client) + 1];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				Client tmp = { -1,"" ,"" ,"" ,"","","","" };
				char mot[10];
				//On d�finit un enregistrement dans lequel on va enregistr� le nom, le pr�nom et le num�ro de t�l�phone lu sur la ligne enregistr� dans le buffer
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1)//Probl�me dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.ID = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, tmp.Nom, CLIENT_NOM_MAX, SEPARATEUR) == OK1)
					{
						idx++;
						if (lire_champ_suivant1(buffer, &idx, tmp.Prenom, CLIENT_PRENOM_MAX, SEPARATEUR) == OK1)
						{
							idx++;
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivant1(buffer, &idx, tmp.Adresse, CLIENT_ADRESSE_MAX, SEPARATEUR) == OK1)
							{
								idx++;
								if (lire_champ_suivant1(buffer, &idx, tmp.Ville, CLIENT_VILLE_MAX, SEPARATEUR) == OK1)
								{
									idx++;
									if (lire_champ_suivant1(buffer, &idx, tmp.CodePostal, CLIENT_CodePostal_MAX, SEPARATEUR) == OK1)
									{
										idx++;
										if (lire_champ_suivant1(buffer, &idx, tmp.NumeroTel, CLIENT_TELEPHONE_MAX, SEPARATEUR) == OK1)
										{
											idx++;
											if (lire_champ_suivant1(buffer, &idx, tmp.MotDePasse, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK1)
											{
												num_rec = appendClientElement(rep, tmp, num_rec);      /* element � priori correct, on le comptabilise */
											}				 //num_rec contient la taille de la liste cha�n�e
										}
									}
								}
							}
						}
					}
				}
			}
		}
		fclose(fic_rep);
		//ON ferme le fichier car on a lu toute les lignes
		return(EXIT_SUCCESS);
	}
} /* fin charger */

  /******************************************************************************/
  /********** Charger distances des stations par rapports aux clients************/
  /******************************************************************************/

RecupDistanceList * newRecupDistanceList()
{
	RecupDistanceList *tmp = malloc(sizeof(RecupDistanceList));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}

SingleRecupDistanceElement* newRecupDistanceElement(RecupDistance value)
{
	SingleRecupDistanceElement *Elem = malloc(sizeof(SingleRecupDistanceElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendRecupDistanceElement(RecupDistanceList *MyList, RecupDistance value, int Size)
{
	SingleRecupDistanceElement *NewElem = newRecupDistanceElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerRecupDistance(RecupDistanceList *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(RecupDistance) + 2;
	char buffer[sizeof(RecupDistance) + 3];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			RecupDistance tmp = { -1,-1, -1 };
			char mot[10];
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
														//On d�finit un enregistrement dans lequel on va enregistr� le nom, le pr�nom et le num�ro de t�l�phone lu sur la ligne enregistr� dans le buffer
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1)//Probl�me dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.ID_Client = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, 4, SEPARATEUR) == OK1)
					{
						idx++;
						tmp.ID_Station = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK1)
						{
							tmp.Distance = atoi(mot);
						}				 //num_rec contient la taille de la liste cha�n�e
					}
				}
				num_rec = appendRecupDistanceElement(rep, tmp, num_rec);      /* element � priori correct, on le comptabilise */
			}
		}
		fclose(fic_rep);
		//ON ferme le fichier car on a lu toute les lignes
		return(EXIT_SUCCESS);
	}
} /* fin charger */

  /*********************************************/
  /** Sauvegarde slots r�serv�s pour commande **/
  /*********************************************/

ReserveSlotList * newReserveSlotList()
{
	ReserveSlotList *tmp = malloc(sizeof(ReserveSlotList));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}

SingleReserveSlotElement* newReserveSlotElement(ReserveSlot value)
{
	SingleReserveSlotElement *Elem = malloc(sizeof(SingleReserveSlotElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendReserveSlotElement(ReserveSlotList *MyList, ReserveSlot value, int Size)
{
	SingleReserveSlotElement *NewElem = newReserveSlotElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}
// Sauvegarde du r�sultat
int SauvegarderReserveSlot(ReserveSlotList *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingleReserveSlotElement *myPtr = rep->head;
	//On place le premier �l�ment sur la t�te de la liste
	int Nb_elts = 1;
	//On peut faire �criture seul car on peut effacer les donn�es pr�sent dans le document s'il existe d�j�.
	//Le r�pertoire est en int�gralit� dans rep.
	if ((err = fopen_s(&fic_rep, nom_fichier, "w")) != 0)
	{
		return(ERROR);
	}
	else
	{
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d%c%d%c%d\n", myPtr->data.ID_PickUP, SEPARATEUR, myPtr->data.ID_slot, SEPARATEUR, myPtr->data.ID_article, SEPARATEUR, myPtr->data.quantite, SEPARATEUR, myPtr->data.numero_tel, SEPARATEUR, myPtr->data.login);
			myPtr = myPtr->next;
			//On va vers l'�l�ment suivant. On n'aura pas de probl�me li� � un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistr� tous les �l�ments. On ne cherche donc pas � afficher l'�l�ment nb_elts (sui est le nb_elts+1 �me �l�ment du r�pertoire) qui a une adresse NULL car non d�finit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien pass�e
		return(OK1);
	}
}

//Tri des stations Pickup en fontion de leurs distances vis-�-vis du client 
int TriStationList(int stationliste[MAX_STATION_PAR_REGION], int distancesliste[MAX_STATION_PAR_REGION], int Nb_elts)
{
	int tmp;
	for (int i = 0; i < Nb_elts - 1; i++)
	{
		for (int j = 0; j < Nb_elts - 1 - i; j++)
		{
			if (distancesliste[j] > distancesliste[j + 1])
			{
				tmp = distancesliste[j];
				distancesliste[j] = distancesliste[j + 1];
				distancesliste[j + 1] = tmp;
				tmp = stationliste[j];
				stationliste[j] = stationliste[j + 1];
				stationliste[j + 1] = tmp;
			}
		}
	}
	return(OK1);
}

int TriStationPourClient(RecupDistanceList *ListDistance, int ID_client, int stationliste[MAX_STATION_PAR_REGION]) {
	if (ListDistance == NULL) return ERROR;
	else {
		int distanceStation[MAX_STATION_PAR_REGION], nb_elts = 0;
		SingleRecupDistanceElement * ptr = ListDistance->head;
		for (int i = 0; i < ListDistance->Size; i++)
		{
			if (ptr->data.ID_Client == ID_client)
			{
				//On pred toutes les stations pickup dans la r�gion du client et les distances qui le s�pare d'elles
				stationliste[nb_elts] = ptr->data.ID_Station;
				distanceStation[nb_elts] = ptr->data.Distance;
				nb_elts++;
			}
			ptr = ptr->next;
		}
		//Tableau non tri� des id pickup dans la r�gion du client
		TriStationList(stationliste, distanceStation, nb_elts);
		return(nb_elts);
	}
}

//Fonction qui retourne l'identifiant du slot si il peut acceuillir quantit� de fois l'article dont l'identifiant vaut ID_article, ou -1 sinon.
int RechercheSlots(int ID_article, SlotDispoFile *slotdispo, int ID_station, int quantite)
{
	//La liste chain�e des slots disponibles est tri� par quantit� croissant
	SingelSlotDispoElement * ptrslotdispo = slotdispo->head;
	if (slotdispo->size != 0)
	{
		for (int i = 0; i < slotdispo->size; i++)
		{
			if ((ptrslotdispo->data.ID_article == ID_article) && (ptrslotdispo->data.ID_PickUP_Station == ID_station))
			{
				if (quantite <= ptrslotdispo->data.quantite)
				{
					return(ptrslotdispo->data.ID_slot);
				}
			}
			ptrslotdispo = ptrslotdispo->next;
		}
		return(ERROR);
	}
	return(ERROR);
}

SingelSlotDispoElement * GetElementAtSlotDispoFile(SlotDispoFile *MyList, int i, int Size)
{
	int j;
	if ((MyList != NULL) && (i + 1 <= Size) && (i + 1 > 0))
	{
		SingelSlotDispoElement *tmp = (*MyList).head;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'�l�ment i
		return(tmp);
	}
	return(NULL);
}

int suppSlotFromDispo(SlotDispoFile *MyList, int i, int Size)
{
	//Le premier �l�ment est � l'indice 0
	int j = 0;
	if ((MyList != NULL) && (i < Size) && (i >= 0))
	{
		SingelSlotDispoElement *tmp = (*MyList).head, *tmp2;
		j = 1;
		if (i == 0)//On supprime l'�l�ment en d�but de liste (on est s�r qu'il y a au moins un �l�ment)
		{
			(*MyList).head = tmp->next; //S'il n'y avait qu'un seul �l�ment alors la queue et la t�te sont � NULL
			free(tmp);
		}
		if ((i == Size - 1) && (i > 0)) //Dans ce cas, on supprime le dernier �l�ment et on est s�r qu'il y en a au moins deux
		{
			tmp = GetElementAtSlotDispoFile(MyList, i - 1, Size);
			free(tmp->next);
			tmp->next = NULL;
		}
		if ((i > 0) && (i < Size - 1))//Dans ce cas, on supprime un �l�ment au milieu de la liste. Or la liste ne peut pas �tre vide si value > 1 donc il n'y a pas de probl�me
									  //Il y a bien un ant�c�dent et un suivant � la i-�me position.
		{
			tmp = GetElementAtSlotDispoFile(MyList, i - 1, Size);
			//On enregistre dans tmp l'adresse de l'�l�ment i-1
			//L'�l�ment i-1 doit pointer vers l'�l�ment i+1
			tmp2 = (tmp->next->next);
			free((*tmp).next);//On lib�re la m�moire de l'�l�ment � la position i
			tmp->next = tmp2; //On dit que l'�l�ment i-1 a pour nt�c�dent l'�l�ment suivant de l'�l�ment suivant de i-1 (soit l'�l�ment i+1)
		}
		MyList->size = MyList->size - 1;
		return(Size - 1);
	}
	return(ERROR);
}

int SuppInSlotDispoFile(SlotDispoFile * MyList, int Id_PickupStation, int Id_Slot)
{
	if (MyList == NULL)
		return (ERROR);
	else
	{
		SingelSlotDispoElement * ptr = MyList->head;
		for (int i = 0; i < MyList->size; i++)
		{
			if ((ptr->data.ID_PickUP_Station == Id_PickupStation) && (ptr->data.ID_slot == Id_Slot))
			{
				suppSlotFromDispo(MyList, i, MyList->size);
			}
		}
		return(OK1);
	}
}

int rechercheTel(ListClient *clientsliste, int Id_Client)
{
	if (clientsliste == NULL)
		return (ERROR);
	else
	{
		SingleClientElement *ptr = clientsliste->head;
		int trouver = ERROR;
		int nb_elts = 0;
		while ((trouver == ERROR) && (nb_elts < clientsliste->Size))
		{
			if (ptr->data.ID == Id_Client)
			{
				trouver = atoi(ptr->data.NumeroTel);
			}
			ptr = ptr->next;
		}
		return(trouver);
	}
}

//  Trie de la liste SlotDispoFile par capacit� d'acceuil
int TriSlotDispoFile(SlotDispoFile *MyList)
{
	if (MyList == NULL)
	{
		return(ERROR);
	}
	else
	{
		for (int i = 0; i < MyList->size - 1; i++)
		{
			SingelSlotDispoElement * ptr = MyList->head;
			for (int j = 0; j < MyList->size - 1 - i; j++)
			{
				if (ptr->data.quantite > ptr->next->data.quantite)
				{
					SlotDispo tmp = ptr->data;
					ptr->data = ptr->next->data;
					ptr->next->data = tmp;
				}
				ptr = ptr->next;
			}
		}
		return (OK1);
	}
}

int ReservationSlot(ReserveSlotList *rep, VentesFile *ventes, RecupDistanceList *distancesList, ListClient *clientsliste, ListArticle * ListArticles, SlotDispoFile * slotdispos, char nom_fichier_reservation_slots[], char nom_fichier_ALivrer[])
{
	//On trie le fichier ventes
	TrieVente(ventes, ListArticles);
	//On trie les slots dispos 
	TriSlotDispoFile(slotdispos);
	//Ainsi, les premi�res ventes (un article achet� en quantit� finie) sont celles n�cessitant les slots les moins grands : on grde ainsi les plus grans slot � coup s�r pour des commandes plus volumineuses
	VentesFile * VentesNonTraitees = newVentesFile();
	SingleVentesElement * ptrventes = ventes->head;
	int ID_client, ID_station;
	int trouver = ERROR;
	int stationstrie[MAX_STATION_PAR_REGION];
	//On parcours tous le fichiers des ventes pour essayer de les livrer toutes (trouver un slot disponible)

	for (int i = 0; i < ventes->Size; i++)
	{
		int j = 0;
		trouver = ERROR;
		ID_client = ptrventes->data.ClientID;
		int nb_elts = TriStationPourClient(distancesList, ID_client, stationstrie);
		//Le fichiers disponibilit� r�unis l'ensembles des r�ponses envoy�es par les stations de la r�gion
		do
		{
			//On teste en priorit� la station pickup la plus proche du client (on reste toujours dans sa r�gion)
			trouver = RechercheSlots(ptrventes->data.ArticleID, slotdispos, stationstrie[j], ptrventes->data.Quantity);
			ID_station = stationstrie[j];
			j++;
		} while ((trouver == ERROR) && (j < nb_elts));
		if (trouver != ERROR)
		{
			//Dans ce cas, on a trouv� un slot disponible : le premier pouvant acceuillir le colis (on a donc choisi le slot de taille optimale)
			//On supprime le slot qu'on a r�serv� pour ne pas le reprendre
			SuppInSlotDispoFile(slotdispos, ID_station, trouver);
			//Il faut r�server le slot : on l'ajoute au fichier qu'on devra envoyer aux stations pickup de la r�gion
			ReserveSlot nouveau = { -1, -1, -1, -1, -1, -1 };
			nouveau.ID_PickUP = ID_station;
			nouveau.ID_slot = trouver;
			nouveau.ID_article = ptrventes->data.ArticleID;
			nouveau.quantite = ptrventes->data.Quantity;
			nouveau.numero_tel = rechercheTel(clientsliste, ptrventes->data.ClientID);
			nouveau.login = ptrventes->data.CodeRetrait;
			appendReserveSlotElement(rep, nouveau, rep->Size);
		}
		else
		{
			//La commande ne peut �tre livr�e dans la r�gion : on remet la livraison � plus tard
			appendVentesElement(VentesNonTraitees, ptrventes->data, VentesNonTraitees->Size);
		}
		ptrventes = ptrventes->next;
	}
	//On sauvegarde la r�servation des slots
	SauvegarderReserveSlot(rep, nom_fichier_reservation_slots);
	//On sauvegarde dns le fichiers ALivrer toutes les commandes qui n'ont p� �tre livr� � cause d'un manque de place (en attente)
	SauvegarderVentesNonTraitee(VentesNonTraitees, nom_fichier_ALivrer);
	//Remarque - on r��crit dans le m�me fichier (par dessus)
	return(OK1);
}

//On fait le choix de mettre une ligne de commande dans un seul slot !!!

//Fonction qui ouvre un fichier renvoyant vers tous les r�sultats �mis par les stations pickup d'une r�gion :
//On cherge toutes les disponibilit� dans la m�me structure
int OuvertureFichiersSlotsDispos(SlotDispoFile * rep, char nom_fichier_ensemble_slots_dispos_par_region[])
{
	if (rep == NULL) return (ERROR);
	else
	{
		FILE *fic_rep;					/* le fichier */
		errno_t err;
		int num_rec = 0;						/* index sur enregistrements */
		int long_max_rec = sizeof(int) + 51 * sizeof(char);
		char buffer[sizeof(int) + 51 * sizeof(char) + 1];
		int idx = 0;
		char *char_nw_line;
		_set_errno(0);
		if ((err = fopen_s(&fic_rep, nom_fichier_ensemble_slots_dispos_par_region, "r")) != 0)
		{
			return(err);
		}
		else
		{
			while (!feof(fic_rep))
			{
				if (fgets(buffer, long_max_rec, fic_rep) != NULL)
				{
					/* memorisation de l'enregistrement lu dans le tableau */
					buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

					if ((char_nw_line = strchr(buffer, '\n')) != NULL)
						*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

					idx = 0;								/* analyse depuis le debut de la ligne */
															// ajouter code implemention liste
					char mot[51];
					if (lire_champ_suivant1(buffer, &idx, mot, 4, SEPARATEUR) == OK1) //On v�rifie que e nom est OK1
					{
						idx++;							/* on saute le separateur */
						if (lire_champ_suivant1(buffer, &idx, mot, 51, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
						{
							ChargerDispoSlot(rep, mot);
						}
					}
				}
			}
			fclose(fic_rep);
			return(OK1);
		}
	}
}

//Fonction qui d�termine les slots qui vont �tre r�serv� dans une r�gion
int DispatchingPickUP(char EnsembleSlotsDisposParRegion[], char Ventes[], char ALivrer[], char Distances[], char Clients[] /*seul fichiers qui ne d�pend pas de la r�gion*/, char Commande[], char ReservationSlots[])
{
	SlotDispoFile * listslotsdispos = newSlotDispoFile();
	OuvertureFichiersSlotsDispos(listslotsdispos, EnsembleSlotsDisposParRegion);
	ReserveSlotList * listreservationslots = newReserveSlotList();
	VentesFile *ventes = newVentesFile();
	ChargerVentes(ventes, Ventes);
	ChargerVentes(ventes, ALivrer);
	RecupDistanceList * distanceList = newRecupDistanceList();
	ChargerRecupDistance(distanceList, Distances);
	ListClient * clientsliste = newListClient();
	ChargerClient(clientsliste, Clients);
	ListArticle * ListArticles = newListArticle();
	ChargerCommande1(ListArticles, Commande);
	ReservationSlot(listreservationslots, ventes, distanceList, clientsliste, ListArticles, listslotsdispos, ReservationSlots, /*"Ventes.txt" */ALivrer);
	return(EXIT_SUCCESS);
}

/******************************************************/
/******************************************************/
/************* Fonctions Stations PickUP **************/
/******************************************************/
/******************************************************/

int RecCapacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite)
{
	int tmp;
	int res = 0;
	if (P_boite <= L_boite)
	{
		tmp = P_boite;
		P_boite = L_boite;
		L_boite = tmp;
	}
	if (L_boite <= H_boite)
	{
		tmp = L_boite;
		L_boite = H_boite;
		H_boite = tmp;
	}
	if (P_boite <= L_boite)
	{
		tmp = P_boite;
		P_boite = L_boite;
		L_boite = tmp;
	}
	//On trie le dimensions de la bo�te pour la comparaison
	if ((H_article <= H_boite) && (L_article <= L_boite) && (P_article <= P_boite))
	{
		return (1 + RecCapacite(P_article, L_article, H_article, H_boite - H_article, L_article, P_article) /*Dessus*/ + RecCapacite(P_article, L_article, H_article, H_boite, L_boite, P_boite - P_article) /*Fond*/ + RecCapacite(P_article, L_article, H_article, H_boite, L_boite - L_article, P_article) /*C�t�*/);
	}
	else
	{
		return(0);
	}
}

//Fonction donne cpacit� d'acceuil d'un article dans un slot
int Capacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite)
{
	int tmp;
	if (P_article <= L_article)
	{
		tmp = P_article;
		P_article = L_article;
		L_article = tmp;
	}
	if (L_article <= H_article)
	{
		tmp = L_article;
		L_article = H_article;
		H_article = tmp;
	}
	if (P_article <= L_article)
	{
		tmp = P_article;
		P_article = L_article;
		L_article = tmp;
	}
	//On trie une seule fois les dimensions de l'article
	return (RecCapacite(P_article, L_article, H_article, P_boite, L_boite, H_boite));
}

/***************************************************************/
/**********  Chargement de la commande (d'une r�gion) **********/
/***************************************************************/

ListArticle * newListArticlePickUP()
{
	ListArticle *tmp = malloc(sizeof(ListArticle));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'�lement de type LinkedList mais aussi tous les �l�ments qui la constitue avec free! 

SingelArticleDansSlotElement* newArticleDansSlotPickUP(ArticleDansSlot value)
{
	SingelArticleDansSlotElement *Elem = malloc(sizeof(SingelArticleDansSlotElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendArticleDansSlotPickUP(ListArticle *MyList, ArticleDansSlot value, int Size)
{
	SingelArticleDansSlotElement *NewElem = newArticleDansSlotPickUP(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int lire_champ_suivantPickUP(char *ligne, int *idx, char *champ, int taille_champ, char separateur)
{
	int idx2 = 0;
	while ((idx2 < (taille_champ - 1)) && (ligne[*idx] != separateur)
		&& (ligne[*idx] != '\0'))
	{
		champ[idx2] = ligne[*idx];
		idx2 += 1;
		*idx += 1;
	}
	if ((ligne[*idx] == separateur) || (ligne[*idx] == '\0'))
	{
		champ[idx2] = 0;	/* fin de chaine sur caractere suivant */
		return(OK1);
	}
	else return(ERROR);		/* fin de ligne ou s�parateur non atteints */

} /* fin lire_champ_suivant1() */

  //Chargement du fichier envoy� par le dispatching avec l'int�grali� des articles command�s et leurs quantit�s dans une r�gion
int ChargerCommandePickUP(ListArticle *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(ArticleDansSlot);
	char buffer[sizeof(ArticleDansSlot) + 1];
	int idx = 0;

	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				ArticleDansSlot tmp = { -1, -1, -1, -1, -1 };
				char mot[10];
				//On d�finit un enregistrement dans lequel on va enregistr� le nom, le pr�nom et le num�ro de t�l�phone lu sur la ligne enregistr� dans le buffer
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_IDArticle + 1, SEPARATEUR) == OK1)//Probl�me dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.IDArticle = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Article + 1, SEPARATEUR) == OK1)
					{
						idx++;
						tmp.quantite = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
						{
							idx++;
							tmp.largeur = atoi(mot);
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
							{
								idx++;
								tmp.profondeur = atoi(mot);
								if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
									tmp.hauteur = atoi(mot);
								num_rec = appendArticleDansSlotPickUP(rep, tmp, num_rec);      /* element � priori correct, on le comptabilise */
																							   //num_rec contient la taille de la liste cha�n�e
							}
						}
					}
				}
			}
		}
		fclose(fic_rep);
		//ON ferme le fichier car on a lu toute les lignes
		return(EXIT_SUCCESS);
	}
} /* fin charger */

  /**************************************************/
  /********** Chargement du fichier Station *********/
  /**************************************************/

StationFile * newStationFile()
{
	StationFile *tmp = malloc(sizeof(StationFile));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'�lement de type LinkedList mais aussi tous les �l�ments qui la constitue avec free! 

SingelStationElement * newSingelStationElement(Station value)
{
	SingelStationElement  *Elem = malloc(sizeof(SingelStationElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendStationElement(StationFile *MyList, Station value, int Size)
{
	SingelStationElement *NewElem = newSingelStationElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = (*MyList).Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerStation(StationFile *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Station);
	char buffer[sizeof(Station) + 1];
	int idx = 0;


	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				Station newSlot = { -1, -1, -1, -1, -1, -1 }; //on cr�e un �l�ment de type contact dans lequel on va enregistrer les chaines de caract�res du fichier
				char mot[10];
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_PICKUP + 1, SEPARATEUR) == OK1) //On v�rifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					newSlot.PickupStationID = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_SLOT + 1, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
					{
						idx++;
						newSlot.SlotID = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
						{
							idx++;
							newSlot.Profondeur = atoi(mot);
							if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
							{
								idx++;
								newSlot.Largeur = atoi(mot);
								if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
								{
									idx++;
									newSlot.Hauteur = atoi(mot);
									if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //on verifie que le tel est OK1
										newSlot.Disponible = atoi(mot);
									num_rec++;
								}
							}
						}
					}
				}
				appendStationElement(rep, newSlot, num_rec--);
			}

		}
		fclose(fic_rep);
		return(OK1);
	}


} /* fin charger */

  /****************************************************************************************************************/
  /********** Etablissement de la liste des capacit� d'acceuils des article dans les slots disponibles ************/
  /****************************************************************************************************************/

SlotDispoFile * newSlotDispoFilePickUP()
{
	SlotDispoFile *tmp = malloc(sizeof(SlotDispoFile));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'�lement de type LinkedList mais aussi tous les �l�ments qui la constitue avec free! 

SingelSlotDispoElement * newSingelSlotDispoElementPickUP(SlotDispo value)
{
	SingelSlotDispoElement  *Elem = malloc(sizeof(SingelSlotDispoElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendSlotDispoElementPickUP(SlotDispoFile *MyList, SlotDispo value, int Size)
{
	SingelSlotDispoElement *NewElem = newSingelSlotDispoElementPickUP(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).size = (*MyList).size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ParcoursStation(ListArticle * art, StationFile * sta, SlotDispoFile * disp)
{
	if ((art == NULL) || (sta == NULL) || (disp == NULL))
		return(ERROR);
	else
	{
		SingelStationElement * ptr = sta->head;
		int cap;
		//On parcours l'int�gralit� de la station
		for (int j = 0; j < sta->Size; j++)
		{
			if (ptr->data.Disponible == 0) //Le slot est disponible : on calcul donc sa capacit� d'acceuil pour chaque article de la commande
			{
				SingelArticleDansSlotElement * ptrart = art->head;
				for (int i = 0; i < art->Size; i++)
				{
					cap = Capacite(ptrart->data.profondeur, ptrart->data.largeur, ptrart->data.hauteur, ptr->data.Profondeur, ptr->data.Largeur, ptr->data.Hauteur);
					if (cap != 0)
					{
						//Dans ce cas, le slot peut acceuillir au moins une fois l'article -> on l'enregistre comme slot disponible pour acceuillir cet article
						SlotDispo newData;
						newData.ID_article = ptrart->data.IDArticle;
						newData.ID_PickUP_Station = ptr->data.PickupStationID;
						newData.ID_slot = ptr->data.SlotID;
						newData.quantite = cap;
						appendSlotDispoElementPickUP(disp, newData, disp->size);
					}
					ptrart = ptrart->next;
				}
			}
			ptr = ptr->next;
		}
	}
	return(OK1);
}

/********** Sauvegarde du r�sultat ***********/
int SauvegarderDisponibilite(SlotDispoFile *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingelSlotDispoElement *myPtr = rep->head;
	//On place le premier �l�ment sur la t�te de la liste
	int Nb_elts = 1;
	//On peut faire �criture seul car on peut effacer les donn�es pr�sent dans le document s'il existe d�j�.
	//Le r�pertoire est en int�gralit� dans rep.
	if ((err = fopen_s(&fic_rep, nom_fichier, "w")) != 0)
	{
		return(ERROR);
	}
	else
	{
		while (Nb_elts <= (*rep).size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d\n", myPtr->data.ID_PickUP_Station, SEPARATEUR, myPtr->data.ID_article, SEPARATEUR, myPtr->data.quantite, SEPARATEUR, myPtr->data.ID_slot);
			myPtr = myPtr->next;
			//On va vers l'�l�ment suivant. On n'aura pas de probl�me li� � un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistr� tous les �l�ments. On ne cherche donc pas � afficher l'�l�ment nb_elts (sui est le nb_elts+1 �me �l�ment du r�pertoire) qui a une adresse NULL car non d�finit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien pass�e
		return(OK1);
	}
}

/*********** Fonction qui donne la r�ponse avec fichiers de d�parts **********/

int RetourDisponibiliteSlot(char FichierCommande[], char FichierStation[] /*Nom du fichier donn� lors de l'appel par DataCenter*/, char FichierRetour[])
{
	ListArticle * art = newListArticlePickUP();
	StationFile * sta = newStationFile();
	SlotDispoFile * disp = newSlotDispoFilePickUP();
	if ((art != NULL) && (sta != NULL) && (disp != NULL))
	{
		ChargerCommandePickUP(art, FichierCommande);
		ChargerStation(sta, FichierStation);
		ParcoursStation(art, sta, disp);
		SauvegarderDisponibilite(disp, FichierRetour);
		return(EXIT_SUCCESS);
	}
	else return (EXIT_FAILURE);
}

//Fonction qui renvoie toutes les disponibilit�s des stations pickup pour les quelles on appelle la fonction pr�c�dente
int ResultatCapacite(char CommandeRegion[], char Station1Region[], char Station2Region[], char Dispo1Region[], char Dispo2Region[])
{
	//Dans le cas o� il y a plus de stations par r�gion et plus de r�gion, on peut �crire une fonction qui va rechercher le noms des stations dans le fichier global des stations
	//On peut "automatiser" cette fonction
	RetourDisponibiliteSlot(CommandeRegion, Station1Region, Dispo1Region);
	RetourDisponibiliteSlot(CommandeRegion, Station2Region, Dispo2Region);
	return(EXIT_SUCCESS);
}

/*********************************************************************/
/*********************************************************************/
/******************* Modification Disponibilite **********************/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************/
/*************** Chargement du fichier de R�servation ****************/
/*********************************************************************/

ReserveSlotList * newReserveSlotListPickUP()
{
	ReserveSlotList *tmp = malloc(sizeof(ReserveSlotList));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'�lement de type LinkedList mais aussi tous les �l�ments qui la constitue avec free! 

SingleReserveSlotElement* newReserveSlotElementPickUP(ReserveSlot value)
{
	SingleReserveSlotElement *Elem = malloc(sizeof(SingleReserveSlotElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendReserveSlotElementPickUP(ReserveSlotList *MyList, ReserveSlot value, int Size)
{
	SingleReserveSlotElement *NewElem = newReserveSlotElementPickUP(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerReserveSlotListPickUP(ReserveSlotList *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(ReserveSlot);
	char buffer[sizeof(ReserveSlot) + 1];
	int idx = 0;


	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				ReserveSlot ptr = { -1, -1, -1, -1, -1, -1 }; //on cr�e un �l�ment de type contact dans lequel on va enregistrer les chaines de caract�res du fichier
				char mot[10];
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_PICKUP + 1, SEPARATEUR) == OK1) //On v�rifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					ptr.ID_PickUP = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_SLOT + 1, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
					{
						idx++;
						ptr.ID_slot = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
						{
							idx++;
							ptr.ID_article = atoi(mot);
							if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
							{
								idx++;
								ptr.quantite = atoi(mot);
								if (lire_champ_suivantPickUP(buffer, &idx, mot, 20, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
								{
									idx++;
									ptr.numero_tel = atoi(mot);
									if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //on verifie que le tel est OK1
										ptr.login = atoi(mot);
									num_rec++;
								}
							}
						}
					}
				}
				appendReserveSlotElementPickUP(rep, ptr, num_rec--);
			}

		}
		fclose(fic_rep);
		return(OK1);
	}


} /* fin charger */

  /***********************************************************************/
  /******************* Cr�ation fichier slots occup�s ********************/
  /***********************************************************************/

  //Fichier qui content le login et mot de passe des slots occup�s dans une station pickup

SlotOccuppeList * newSlotOccupeP()
{
	SlotOccuppeList *tmp = malloc(sizeof(SlotOccuppeList));
	//On alloue dynamiquement l'espace m�moire pour d�finir la liste cha�n�e
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head � NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'�lement de type LinkedList mais aussi tous les �l�ments qui la constitue avec free! 

SingleSlotOccuppeElement* newSlotOccupeElement(SlotOccuppe value)
{
	SingleSlotOccuppeElement *Elem = malloc(sizeof(SingleSlotOccuppeElement));
	//On alloue dynamiquement la m�moire n�cessaire � la cr�ation d'un maillon de la cha�ne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a r�ussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la cha�ne
		(*Elem).next = NULL;
		//L'�l�ment suivant est initialise � NULL et sera modifier lors de l'insertion de l'�l�ment dans la liste
	}
	return(Elem);
}

int appendSlotOccupeElement(SlotOccuppeList *MyList, SlotOccuppe value, int Size)
{
	SingleSlotOccuppeElement *NewElem = newSlotOccupeElement(value);//On fait un nouveau maillon qu'on ajoutera � la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entit�s NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe d�j� une queue � la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel �l�ment en reliant le dernier �l�ment de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet �l�ment devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'�l�ment et l'�l�ment ajout� est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augment� de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

int ChargerSlotOccupeList(SlotOccuppeList *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(SlotOccuppe);
	char buffer[sizeof(SlotOccuppe) + 1];
	int idx = 0;


	char *char_nw_line;

	_set_errno(0);
	if ((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_rep))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
				SlotOccuppe ptr = { -1, -1, -1, -1 }; //on cr�e un �l�ment de type contact dans lequel on va enregistrer les chaines de caract�res du fichier
				char mot[10];
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_PICKUP + 1, SEPARATEUR) == OK1) //On v�rifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					ptr.ID_PickUP = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_SLOT + 1, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
					{
						idx++;
						ptr.ID_Slot = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, 21, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
						{
							idx++;
							ptr.Tel = atoi(mot);
							if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On v�rifie que le prnom est OK1
							{
								idx++;
								ptr.CodeRetrait = atoi(mot);

								num_rec++;
							}
						}
					}
				}
				appendSlotOccupeElement(rep, ptr, num_rec--);
			}

		}
		fclose(fic_rep);
		return(OK1);
	}


} /* fin charger */

int SauvegarderSlotOccupe(SlotOccuppeList *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingleSlotOccuppeElement *myPtr = rep->head;
	//On place le premier �l�ment sur la t�te de la liste
	int Nb_elts = 1;
	//On peut faire �criture seul car on peut effacer les donn�es pr�sent dans le document s'il existe d�j�.
	//Le r�pertoire est en int�gralit� dans rep.
	if ((err = fopen_s(&fic_rep, nom_fichier, "w")) != 0)
	{
		return(ERROR);
	}
	else
	{
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d\n", myPtr->data.ID_PickUP, SEPARATEUR, myPtr->data.ID_Slot, SEPARATEUR, myPtr->data.Tel, SEPARATEUR, myPtr->data.CodeRetrait);
			myPtr = myPtr->next;
			//On va vers l'�l�ment suivant. On n'aura pas de probl�me li� � un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistr� tous les �l�ments. On ne cherche donc pas � afficher l'�l�ment nb_elts (sui est le nb_elts+1 �me �l�ment du r�pertoire) qui a une adresse NULL car non d�finit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien pass�e
		return(OK1);
	}
}

/*********************************************************************/
/***************** Modification du fichier Station *******************/
/*********************************************************************/

//Fonction qui permet de trouver un slot pr�cis dans le fichier station � partir de son ID de slot (et Id de station)
//On change la disponibilit� du slot ayant cet ID
int RechercherSlotInStation(StationFile * slotStation, int ID_pickUP, int ID_slot)
{
	if (slotStation == NULL)
		return (ERROR);
	else
	{
		SingelStationElement * ptr = slotStation->head;
		for (int i = 0; i < slotStation->Size; i++)
		{
			if ((ptr->data.PickupStationID == ID_pickUP) && (ptr->data.SlotID == ID_slot))
			{
				ptr->data.Disponible = 1;
				//Le slot n'est plus disponible d�sormais -> on la r�serv�
			}
			ptr = ptr->next;
		}
		return(OK1);
	}
}

int ModificationDisponibilitePickUP(StationFile * slotsStation, ReserveSlotList * slotsAReserver, SlotOccuppeList *Slotsoccupe)
{
	if ((slotsAReserver == NULL) || (slotsStation == NULL))
		return(ERROR);
	else
	{
		SingleReserveSlotElement * ptr = slotsAReserver->head;
		for (int i = 0; i < slotsAReserver->Size; i++)
		{
			RechercherSlotInStation(slotsStation, ptr->data.ID_PickUP, ptr->data.ID_slot);
			if (ptr->data.ID_PickUP == slotsStation->head->data.PickupStationID)
			{
				SlotOccuppe nouveau = { -1,-1,-1,-1 };
				nouveau.ID_PickUP = ptr->data.ID_PickUP;
				nouveau.ID_Slot = ptr->data.ID_slot;
				nouveau.Tel = ptr->data.numero_tel;
				nouveau.CodeRetrait = ptr->data.login;
				appendSlotOccupeElement(Slotsoccupe, nouveau, Slotsoccupe->Size);
			}
			ptr = ptr->next;
		}
		return(OK1);
	}
}

int SauvegarderStation(StationFile *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingelStationElement *myPtr = rep->head;
	//On place le premier �l�ment sur la t�te de la liste
	int Nb_elts = 1;
	//On peut faire �criture seul car on peut effacer les donn�es pr�sent dans le document s'il existe d�j�.
	//Le r�pertoire est en int�gralit� dans rep.
	if ((err = fopen_s(&fic_rep, nom_fichier, "w")) != 0)
	{
		return(ERROR);
	}
	else
	{
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d%c%d%c%d\n", myPtr->data.PickupStationID, SEPARATEUR, myPtr->data.SlotID, SEPARATEUR, myPtr->data.Hauteur, SEPARATEUR, myPtr->data.Largeur, SEPARATEUR, myPtr->data.Profondeur, SEPARATEUR, myPtr->data.Disponible);
			myPtr = myPtr->next;
			//On va vers l'�l�ment suivant. On n'aura pas de probl�me li� � un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistr� tous les �l�ments. On ne cherche donc pas � afficher l'�l�ment nb_elts (sui est le nb_elts+1 �me �l�ment du r�pertoire) qui a une adresse NULL car non d�finit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien pass�e
		return(OK1);
	}
}

//On modifie le fichier station � partir du fichier envoy� par le dispatching contenant les slots � r�server dans les pickup stations
//et les logins et mot de passes associ�s permettant � l'utilisateur de r�cup�rer le colis
int ReservationSlotPickUP(char nomStation[], char slotoccupestation[])
{
	ReserveSlotList * listslotAreserve = newReserveSlotListPickUP();
	ChargerReserveSlotListPickUP(listslotAreserve, "ReservationSlotsnord.txt");
	StationFile * stations = newStationFile();
	ChargerStation(stations, nomStation);
	SlotOccuppeList *slotoccupe = newSlotOccupeP();
	ChargerSlotOccupeList(slotoccupe, slotoccupestation);
	ModificationDisponibilitePickUP(stations, listslotAreserve, slotoccupe);
	SauvegarderStation(stations, nomStation);
	SauvegarderSlotOccupe(slotoccupe, slotoccupestation);
	return(OK1);
}

/****************************************************/
/***** Suppression d'un slots des slots occup�s *****/
/****************************************************/

SingleSlotOccuppeElement * GetElementAtSlotOccupe(SlotOccuppeList *MyList, int i, int Size)
{
	int j;
	if ((MyList != NULL) && (i + 1 <= Size) && (i + 1 > 0))
	{
		SingleSlotOccuppeElement *tmp = (*MyList).head;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'�l�ment i
		return(tmp);
	}
	return(NULL);
}

int suppInSlotOccupeList(SlotOccuppeList *MyList, int i, int Size)
{
	int j = 0;
	if ((MyList != NULL) && (i < Size) && (i >= 0))
	{
		SingleSlotOccuppeElement *tmp = (*MyList).head, *tmp2;
		j = 1;
		if (i == 0)//On supprime l'�l�ment en d�but de liste (on est s�r qu'il y a au moins un �l�ment)
		{
			(*MyList).head = tmp->next; //S'il n'y avait qu'un seul �l�ment alors la queue et la t�te sont � NULL
			free(tmp);
		}
		if ((i == Size - 1) && (i > 0)) //Dans ce cas, on supprime le dernier �l�ment et on est s�r qu'il y en a au moins deux
		{
			tmp = GetElementAtSlotOccupe(MyList, i - 1, Size);
			free(tmp->next);
			tmp->next = NULL;
		}
		if ((i > 0) && (i < Size - 1))//Dans ce cas, on supprime un �l�ment au milieu de la liste. Or la liste ne peut pas �tre vide si value > 1 donc il n'y a pas de probl�me
									  //Il y a bien un ant�c�dent et un suivant � la i-�me position.
		{
			tmp = GetElementAtSlotOccupe(MyList, i - 1, Size);
			//On enregistre dans tmp l'adresse de l'�l�ment i-1
			//L'�l�ment i-1 doit pointer vers l'�l�ment i+1
			tmp2 = (tmp->next->next);
			free((*tmp).next);//On lib�re la m�moire de l'�l�ment � la position i
			tmp->next = tmp2; //On dit que l'�l�ment i-1 a pour nt�c�dent l'�l�ment suivant de l'�l�ment suivant de i-1 (soit l'�l�ment i+1)
		}
		MyList->Size = MyList->Size - 1;
		return(Size - 1);
	}
	return(-1);
}

/***************************************************/
/***************************************************/
/******** Retrait de colis (station n�1) ***********/
/***************************************************/
/***************************************************/

//On recherche l'identifiant du slot dans la pickup station dont le login est le n� de t�l�phone saisi par l'utilisateur et on r�cup�re le code de retrait
int RechercheSlotOccuppeWithTel(SlotOccuppeList *slotoccupes, int Tel, int *CodeARentrer, int * SlotId)
{
	if (slotoccupes == NULL)
		return(ERROR);
	else
	{
		SingleSlotOccuppeElement * ptr = slotoccupes->head;
		int nb_elts = 0;
		int trouver = ERROR;
		while ((trouver == ERROR) && (nb_elts < slotoccupes->Size))
		{
			if (ptr->data.Tel == Tel)
			{
				*CodeARentrer = ptr->data.CodeRetrait;
				*SlotId = ptr->data.ID_Slot;
				return(nb_elts);
			}
			ptr = ptr->next;
			nb_elts++;
		}
		return(ERROR);
	}
}

int LiberationSlot(SlotOccuppeList *slotoccupes, StationFile * stations, int CodeARentrer, int CodeRentrer, int indice, int SlotId)
{
	if ((slotoccupes == NULL) || (stations == NULL))
	{
		return (ERROR);
	}
	else
	{
		if (CodeARentrer == CodeRentrer)
		{
			//Dans ce cas, le colis est r�cup�r� par l'utilisateur et on lib�re le slot
			//On supprime le slot du fichier des slots occupp�s de cette station
			suppInSlotOccupeList(slotoccupes, indice, slotoccupes->Size);
			SingelStationElement * ptr = stations->head;
			int trouver = ERROR;
			while (trouver == ERROR) //Si on lance LiberationSlot il y a bien un slot dont le login est le num�ro de telephone rentr� pr�c�dement
			{
				if (ptr->data.SlotID == SlotId)
				{
					trouver = OK1;
					ptr->data.Disponible = 0;
					//On remet � jour la disponibilit� du slot dans le fichier station
				}
				ptr = ptr->next;
			}
			return(OK1);
		}
		return(ERROR);
	}
}

int LiberationDansStation001()
{
	SlotOccuppeList * slotoccupe = newSlotOccupeP();
	ChargerSlotOccupeList(slotoccupe, "SlotOccupe001.txt");
	StationFile * stations = newStationFile();
	ChargerStation(stations, "Station001.txt");
	printf("Entrer votre numero de telephone : \n");
	int tel;
	scanf_s("%d%*c", &tel);
	int CodeRentrer;
	int CodeARentrer = ERROR;
	int SlotID = ERROR;
	printf("Entrer votre Code de Retrait : \n");
	scanf_s("%d%*c", &CodeRentrer);
	int nb_elts = RechercheSlotOccuppeWithTel(slotoccupe, tel, &CodeARentrer, &SlotID);
	LiberationSlot(slotoccupe, stations, CodeARentrer, CodeRentrer, nb_elts, SlotID);
	SauvegarderSlotOccupe(slotoccupe, "SlotOccupe001.txt");
	SauvegarderStation(stations, "Station001.txt");
	return(EXIT_SUCCESS);
}

/*********************************************************************/
/******************** Lancement De La Simulation *********************/
/*********************************************************************/


int main() {
	clear_fichier("commande.txt");
	
	for  (int i = 0;  i < 5;  i++)
	{
		int d = rand_a_b(1, 5);
		int e = rand_a_b(1, 6);
		int f = rand_a_b(1, 4);
		mysleep(1);
		srand(time(NULL));
		FILE *fic_repachats; // le fichier 
		int openSucces = fopen_s(&fic_repachats, "achat.txt", "w");
		fprintf(fic_repachats, "%d;%d;%d;\n", d,e,f);
		fclose(fic_repachats);		
		int c = Gestion_Achat("achat.txt");
		clear_fichier("achat.txt");
	}
	
	clear_fichier("nord.txt");
	clear_fichier("pas_de_calais.txt");
	ClientFile listeclient;
	init_BDDclient(&listeclient);
	ClientSeul *client;
	client = listeclient.BDDclient->head;
	CommandeFile commande;
	init_commande(&commande);
	trie(&commande, client, &listeclient);
	int a = fonction("Stock0.txt", "nord.txt","envoit1.txt");
	int b = fonction( "Stock1.txt", "pas_de_calais.txt","envoit2.txt");
	system("pause");



	
	ResultatCapacite("envoit1.txt", "Station001.txt", "Station002.txt", "SlotsDispos001.txt", "SlotsDispos002.txt");
	system("pause");
	DispatchingPickUP("EnsembleSlotsDisposParRegionnord.txt", "nord.txt", "ALivrernord.txt", "Distances.txt", "client.txt", "envoit1.txt", "ReservationSlotsnord.txt");
	system("pause");
	ReservationSlotPickUP("Station001.txt", "SlotOccupe001.txt");
	system("pause");
	ReservationSlotPickUP("Station002.txt", "SlotOccupe002.txt");
	system("pause");
	printf("Liberation d'un slot\n");
	//On lib�re un slot dans la pickup station n�1 (Pour tester : Chaque Pickup ne poss�dant que son propre programme -> il n'y aura pas de probl�me de choix)
	//LiberationDansStation001();
	//system("pause");
	//On ne lib�re pas le slot car pas encore enti�rement int�gr�...
	return(EXIT_SUCCESS);
}