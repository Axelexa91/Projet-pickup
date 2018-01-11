#pragma once
#include "Structure1.h"
#include <stdio.h>
#include <string.h>  /* pour les manipulations de cha�nes de caract�res */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, char **argv) {
	print();
	ClientFile ListeClient;
	init_BDDclient(&ListeClient);
	creation_compte(&ListeClient);
	system("pause");
	return(0);
}

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
	errno = chargerclient(FichierClient, "C:\\Users\\Camille\\testLoulous\\client.txt");  //Il faut charger le fichier client existant dans la liste chain�e 
	return(0);
}




/*---Fonction qui permet de d�couper une ligne d'un fichier texte en fonction d'un s�parateur*/
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



/*---Fonction qui permet de charger un fichier texte dans la liste chain�e contenant les clients*/
int chargerclient(ClientFile * Fichierclient, char nom_fichier[])
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
						/* element � priori correct, on le comptabilise */
					}
				}
				if (Fichierclient->BDDclient->size == 0) //Si la liste est vide on fait pointer la t�te de liste sur cet �l�ment
				{
					Fichierclient->BDDclient->head = nouv_element;
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


/*---Fonction qui permet de sauvegarder dans un fichier texte la liste chain�e s'il y a eu changements---*/
int sauvegarder(ClientFile *Fichierclient, char nom_fichier[])
{
	FILE *fic_client;					/* le fichier */
	errno_t err;
	int i = 0;
	int div = 10;
	int nbdig = 0;  //Permet de savoir le nombre de digit de l'ID pour normaliser l'�criture dans le fichier avec les 0
	if ((err = fopen_s(&fic_client, nom_fichier, "w")) != 0)
	{
		return(err);
	}
	else
	{
		ClientSeul *client; //Permet de se d�placer dans la liste chain�e
		client = Fichierclient->BDDclient->head;
		if (Fichierclient->NombreDeClient < CLIENTFILE_MAX) //On r�alise la sauvegarde si on a pas atteint le nombre maximum de contact dans le r�pertoire
		{
			while (i < Fichierclient->NombreDeClient)
			{

				fprintf(fic_client, "%d", client->client.ID);   //A MODIFIER POUR AVOIR L'ID SUR 4 DIGITS
				fputs(";", fic_client);
				fputs(client->client.Nom, fic_client);
				fputs(";", fic_client);
				fputs(client->client.Prenom, fic_client);
				fputs(";", fic_client);
				fputs(client->client.Adresse, fic_client);
				fputs(";", fic_client);
				fputs(client->client.Ville, fic_client);
				fputs(";", fic_client);
				fputs(client->client.CodePostal, fic_client);
				fputs(";", fic_client);
				fputs(client->client.Numero, fic_client);
				fputs(";", fic_client);
				fputs(client->client.MotDePasse, fic_client);
				fputs(";\n", fic_client);
				i++;
				client = client->next; //Pour passer � l�l�ment suivant
			}
		}
		else
		{
			return(ERROR);
		}
	}
	fclose(fic_client);
	return(OK);
} /* fin sauvegarder */

  /*---Fonction qui permet de cr�er un compte avec les infos d'un fichier et l'ajouter � la BDD client---*/
int creation_compte(ClientFile *Fichierclient)
{

	int ancien_ID = 0;  //Permet de savoir le dernier ID attribu�

	ClientSeul *nouv_client;   //On cr�e un nouvel �l�ment de liste
	nouv_client = (ClientSeul *)malloc(sizeof(ClientSeul)); //La taille de l'�l�ment est d�finie de mani�re dynamique
	nouv_client->next = NULL; //L'�l�ment pointe sur un �l�ment suivant qui est nul

	if (Fichierclient->NombreDeClient != 0)
	{
		ClientSeul *dernier_client;
		dernier_client = Fichierclient->BDDclient->tail;
		ancien_ID = dernier_client->client.ID;
		nouv_client->client.ID = ancien_ID + 1;  //L'ID attribu� est celui du dernier �l�ment incr�ment� de 1
	}
	else
	{
		nouv_client->client.ID = ancien_ID; //Si la base de donn�e est vide le premier ID attribu� est 0
	}


	FILE *fic_creation = NULL;
	errno_t err;
	int index = 0;
	int long_max_rec = sizeof(Client);
	char buffer[sizeof(Client) + 1];
	char *char_nw_line;

	if ((err = fopen_s(&fic_creation, "C:\\Users\\Camille\\testLoulous\\creation.txt", "r")) != 0)
	{
		return(err);
	}
	else
	{
		while (!feof(fic_creation))
		{
			if (fgets(buffer, long_max_rec, fic_creation) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
				{
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */
				}

				index = 0;								/* analyse depuis le debut de la ligne */


				if (lire_champ_suivant(buffer, &index, nouv_client->client.Nom, CLIENT_NOM_MAX, SEPARATEUR) == OK)
				{
					index++;
					if (lire_champ_suivant(buffer, &index, nouv_client->client.Prenom, CLIENT_PRENOM_MAX, SEPARATEUR) == OK)
					{
						index++;
						if (lire_champ_suivant(buffer, &index, nouv_client->client.Adresse, CLIENT_ADRESSE_MAX, SEPARATEUR) == OK)
						{
							index++;
							if (lire_champ_suivant(buffer, &index, nouv_client->client.Ville, CLIENT_VILLE_MAX, SEPARATEUR) == OK)
							{
								index++;
								if (lire_champ_suivant(buffer, &index, nouv_client->client.CodePostal, CLIENT_CodePostal_MAX, SEPARATEUR) == OK)
								{
									index++;
									if (lire_champ_suivant(buffer, &index, nouv_client->client.Numero, CLIENT_TELEPHONE_MAX, SEPARATEUR) == OK)
									{
										index++;
										if (lire_champ_suivant(buffer, &index, nouv_client->client.MotDePasse, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK)
										{

										}
									}
								}
							}
						}
					}
				}
			}
		}
		fclose(fic_creation);
	}

	if (Fichierclient->BDDclient->size == 0)        /*Dans le cas o� la liste est vide, head est donc NULL, on doit donc ins�rer l'�l�ment
													� cet endroit*/
	{
		Fichierclient->BDDclient->head = nouv_client;
	}
	else                              //Sinon c'est � la queue qu'on ajoute l'�l�ment � la liste chain�e
	{
		Fichierclient->BDDclient->tail->next = nouv_client;
	}
	Fichierclient->BDDclient->tail = nouv_client;
	Fichierclient->BDDclient->size++;   //La liste chain�e contient alors un contact de plus
	Fichierclient->NombreDeClient++;  //Le r�pertoire contient un �l�ment de plus

	sauvegarder(Fichierclient, "C:\\Users\\Camille\\testLoulous\\client.txt");
	return(OK);
}

