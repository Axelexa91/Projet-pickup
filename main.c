#pragma once
#include "Structure.h"
#include <stdio.h>
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>



int main(int argc, char **argv) {
	print();
	/*ClientFile ListeClient;
	init_BDDclient(&ListeClient);
	connexion(&ListeClient);
	system("pause");*/
	while (1) {
		printf("loooool");
	}
	return(0);
}

static void print() {
	printf("Je suis lance");
}
int init_BDDclient(ClientFile *FichierClient)
{
	int errno;
	void * tmpPtr;
	FichierClient->NombreDeClient = 0;    //On initialise le nombredeclient d'une base vide à 0
	tmpPtr = (void *)malloc(CLIENTFILE_MAX * sizeof(ListeClient));
	if (tmpPtr != NULL) {
		FichierClient->BDDclient = (ListeClient*)tmpPtr;
		FichierClient->BDDclient->head = NULL;  //Si la base de donnée est vide la tête pointe sur un élément nul
		FichierClient->BDDclient->size = 0;
		FichierClient->BDDclient->tail = NULL;   //Pareil pour la queue
	}
	else return(-1);
	errno = chargerclient(FichierClient, "C:\\Users\\Camille\\testLoulous\\client.txt");  //Il faut charger le fichier client existant dans la liste chainée 
	return(0);
}


/*---Fonction qui permet de découper une ligne d'un fichier texte en fonction d'un séparateur*/
int lire_champ_suivant(char *ligne, int *index, char *champ, int taille_champ, char separateur)
{
	int index2 = 0;  //Curseur au sein du mot que l'on souhaite remplir
	while ((index2 < (taille_champ - 1)) && (ligne[*index] != separateur))    //On récupère toutes les lettres contenues entre les séparateur, avec une limite de taille
	{
		champ[index2] = ligne[*index];  //La lettre est copiée dans le champ souhaité
		index2 += 1;
		*index += 1;
	}
	if ((ligne[*index] == separateur) || (ligne[*index] == '\0'))
	{
		champ[index2] = 0;	/* fin de chaine sur caractere suivant */
		return(OK);
	}
	else return(ERROR);		/* fin de ligne ou séparateur non atteints */
}


/*---Fonction qui permet de charger un fichier texte dans la liste chainée contenant les clients*/
int chargerclient(ClientFile * Fichierclient, char nom_fichier[])
{
	FILE *fic_client;					/* le fichier */
	errno_t err;
	int nombre_client = 0;				/* index sur enregistrements */
	int long_max_rec = sizeof(Client);
	char buffer[sizeof(Client) + 1];
	int index = 0;
	char IDtmp[5];     //Tableau servant de tampon pour récupérer l'ID en tant que chaine de caractères pour le transformer après en int
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
				nouv_element = (ClientSeul *)malloc(sizeof(ClientSeul)); //On alloue au nouvel élément une place suffisante en mémoire pour contenir un enregistrement
				nouv_element->next = NULL;

				if (lire_champ_suivant(buffer, &index, IDtmp, 5, SEPARATEUR) == OK)
				{
					nouv_element->client.ID = atoi(IDtmp);   //On transforme la chaine de caractère en entier
					index++;

					if (lire_champ_suivant(buffer, &index, nouv_element->client.Nom, CLIENT_NOM_MAX, SEPARATEUR) == OK) //On récupère la partie nom après le séparateur qu'on place dans le nouvel élément
					{
						index++;
						if (lire_champ_suivant(buffer, &index, nouv_element->client.Prenom, CLIENT_PRENOM_MAX, SEPARATEUR) == OK)
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
										if (lire_champ_suivant(buffer, &index, nouv_element->client.Numero, CLIENT_TELEPHONE_MAX, SEPARATEUR) == OK)
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
						/* element à priori correct, on le comptabilise */
					}
				}
				if (Fichierclient->BDDclient->size == 0) //Si la liste est vide on fait pointer la tête de liste sur cet élément
				{
					Fichierclient->BDDclient->head = nouv_element;
					//printf("%d", nouv_element->client.ID);
				}
				else
				{
					Fichierclient->BDDclient->tail->next = nouv_element; //Si la liste est non vide on fait pointer l'élément suivant la queue de liste sur cet élément
				}
				Fichierclient->BDDclient->tail = nouv_element; //La queue pointe dans tous les cas sur l'élément ajouté
				Fichierclient->BDDclient->size++;
				Fichierclient->NombreDeClient++;
			}
		}fclose(fic_client);
		return(OK);
	}
}

int connexion(ClientFile *FichierClient)
{
	FILE *fic_connexion = NULL;
	errno_t err;
	char IDtmp2[5];
	int index = 0;
	int ID;
	char motdepasse[5];
	int long_max_rec = sizeof(Client);
	char buffer[sizeof(Client) + 1];
	char *char_nw_line;

	if ((err = fopen_s(&fic_connexion, "C:\\Users\\Camille\\testLoulous\\connect.txt", "r")) != 0)
	{
		return(0);
	}
	else
	{
		while (!feof(fic_connexion))
		{
			if (fgets(buffer, long_max_rec, fic_connexion) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
				{
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */
				}

				index = 0;								/* analyse depuis le debut de la ligne */


				if (lire_champ_suivant(buffer, &index, IDtmp2, 5, SEPARATEUR) == OK)
				{
					ID = atoi(IDtmp2);
					index++;
					if (lire_champ_suivant(buffer, &index, motdepasse, 5, SEPARATEUR) == OK)
					{

					}
				}
			}
		}
		fclose(fic_connexion);
	}

	int i = 0;		    /* position (indice) de début de recherche dans tableau/liste rep */
	int ind_fin;			/* position (indice) de fin de tableau/liste rep */


	int trouve = 0;
	ind_fin = FichierClient->NombreDeClient;
	ClientSeul *clientcherche;
	clientcherche = FichierClient->BDDclient->head;
	if (i <= ind_fin)
	{
		do
		{
			if (ID == clientcherche->client.ID)   //On compare l'ID entré avec tous ceux de la BDD
			{
				trouve = 1;
			}
			else
			{
				i++;
				clientcherche = clientcherche->next; //On pointe sur l'élément suivant pour réaliser la comparaison sur l'élément suivant
			}
		} while (trouve == 0 && i <= ind_fin);
	}
	else
	{
		FILE *fic_reponse;					/* le fichier */
		errno_t err;
		int i = 0;
		int div = 10;
		int nbdig = 0;  //Permet de savoir le nombre de digit de l'ID pour normaliser l'écriture dans le fichier avec les 0
		if ((err = fopen_s(&fic_reponse, "C:\\Users\\Camille\\testLoulous\\reponse.txt", "w")) != 0)
		{
			return(0);
		}
		else
		{
			fprintf(fic_reponse,"%d",0);
			return(0);
		}
	}
	if (trouve == 1) //Si l'ID correspond on verifie le mot de passe correspondant à la ligne de l'ID
	{
		if (strcmp(motdepasse, clientcherche->client.MotDePasse) == 0)
		{
			FILE *fic_reponse;					/* le fichier */
			errno_t err;
			int i = 0;
			int div = 10;
			int nbdig = 0;  //Permet de savoir le nombre de digit de l'ID pour normaliser l'écriture dans le fichier avec les 0
			if ((err = fopen_s(&fic_reponse, "C:\\Users\\Camille\\testLoulous\\reponse.txt", "w")) != 0)
			{
				return(0);
			}
			else
			{
				fprintf(fic_reponse, "%d", 1);
				return(1);
			}
		}
	}
	else
	{
		return(0);
	}
}
