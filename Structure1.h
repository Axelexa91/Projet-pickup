#pragma once
#include <stdio.h>

#include <stdlib.h>

#define OK 1

#define ERROR -1

#define CLIENT_ID_MAX 9999 

#define CLIENT_NOM_MAX 21

#define CLIENT_PRENOM_MAX 21

#define CLIENT_ADRESSE_MAX 51

#define CLIENT_VILLE_MAX 21

#define CLIENT_CodePostal_MAX 6

#define CLIENT_TELEPHONE_MAX 11

#define CLIENT_MotDePasse_MAX 5

#define CLIENTFILE_MAX 1000

#define SEPARATEUR ';'



typedef struct Client {

	int ID;								//4 chiffres pour l'ID

	char Nom[21];						//Nom de 20 caract�re alpha ( +1 pour le \0 de fin de chaine de caract�re )

	char Prenom[21];					//Prenom de 20 caract�re alpha ( +1 pour \0 de fin de chaine de caract�re)

	char Adresse[51];					//localisation ( adresse ) du client, 50 caract�res ( +1 pour \0 de fin de chainede caract�re )

	char Ville[21];						//La ville du client (+1 pour le \0)

	char CodePostal[6];

	char Numero[11];					//num�ro de t�l�phone, 10 chiffres dit caract�re num�rique ( et PAS alphanum�rique ) (+1 pour le \0 de fin de chaine de caract�re )

	char MotDePasse[5];					//Mot de passe de 4 caract�re alphanum�rique (+1 pour le \0� de fin de chaine de caract�re)
} Client;

/*La structure qui regroupe tous les clients (UN FICHIER) */

typedef struct ClientSeul  //D�claration d'un �l�ment individuel de la liste
{
	Client client;
	struct ClientSeul *next;  //On d�finit un pointeur sur l'�l�ment suivant
}ClientSeul;

typedef struct ListeClient //D�claration de la liste en elle-m�me avec une t�te de liste, une queue de liste et la taille de la liste
{
	ClientSeul *head;
	int size;
	ClientSeul *tail;
}ListeClient;



typedef struct ClientFile
{
	int NombreDeClient;
	ListeClient *BDDclient;
} ClientFile;

int init_BDDclient(ClientFile *Fichierclient);

int lire_champ_suivant(char *ligne, int *idx, char *champ, int taille_champ, char separateur);

int chargerclient(ClientFile * Fichierclient, char nom_fichier[]);

int creation_compte(ClientFile *Fichierclient);

int sauvegarder(ClientFile *Fichierclient, char nom_fichier[]);