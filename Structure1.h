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

	char Nom[21];						//Nom de 20 caractère alpha ( +1 pour le \0 de fin de chaine de caractère )

	char Prenom[21];					//Prenom de 20 caractère alpha ( +1 pour \0 de fin de chaine de caractère)

	char Adresse[51];					//localisation ( adresse ) du client, 50 caractères ( +1 pour \0 de fin de chainede caractère )

	char Ville[21];						//La ville du client (+1 pour le \0)

	char CodePostal[6];

	char Numero[11];					//numéro de téléphone, 10 chiffres dit caractère numérique ( et PAS alphanumérique ) (+1 pour le \0 de fin de chaine de caractère )

	char MotDePasse[5];					//Mot de passe de 4 caractère alphanumérique (+1 pour le \0à de fin de chaine de caractère)
} Client;

/*La structure qui regroupe tous les clients (UN FICHIER) */

typedef struct ClientSeul  //Déclaration d'un élément individuel de la liste
{
	Client client;
	struct ClientSeul *next;  //On définit un pointeur sur l'élément suivant
}ClientSeul;

typedef struct ListeClient //Déclaration de la liste en elle-même avec une tête de liste, une queue de liste et la taille de la liste
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