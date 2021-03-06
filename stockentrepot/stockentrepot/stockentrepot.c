



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

int main() {
	clear_fichier("commande.txt");
	for  (int i = 0;  i < 5;  i++)
	{
		int d = rand_a_b(0, 5);
		int e = rand_a_b(0, 6);
		int f = rand_a_b(1, 20);
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
	return(0);
}