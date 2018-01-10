#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include "Structures.h"

#define OK 0
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
#define	ARTICLE_ID_MAX 9999
#define	ARTICLE_NOM_MAX 21
#define	ARTICLE_DESCRIPTION_MAX 21
#define ARTICLE_TYPE_MAX 21
#define ARTICLEFILE_MAX 1000
#define CODE_RETRAIT_MAX 9999
#define QUANTITY_MAX 9999
#define COMMANDEFILE_MAX 1000
#define ENTREPOT_ID_MAX 9999
#define HAUTEUR_MAX 9999 //en cm
#define LARGEUR_MAX 9999
#define PROFONDEUR_MAX 9999
#define ARTICLESRTOCKSFILE_MAX 1000
#define STATION_ID_MAX 255
#define SLOT_ID_MAX 9999
#define ENTREPOT_NOM_DOSSIER_MAX 21
#define ENTREPOTFILE_MAX 1000

#define DISPONIBLE 1
#define INDISPONIBLE 0

#define SEPARATEUR ';'

int lire_champ_suivant(char *ligne, int *idx, char *champ, int taille_champ, char separateur)
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
		return(OK);
	}
	else return(ERROR);		/* fin de ligne ou séparateur non atteints */

} /* fin lire_champ_suivant() */


int charger(ClientFile * client, char nom_fichier[])
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
		while (!feof(fic_rep) && (client->NombreDeClients < CLIENT_ID_MAX))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;							/* analyse depuis le debut de la ligne */
				char mot[30];
				if (lire_champ_suivant(buffer, &idx, mot, CLIENT_ID_MAX, SEPARATEUR) == OK)
				{
					idx++;							/* on saute le separateur */
					client->TableauDeClient->ID = atoi(mot);
					if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Adresse, CLIENT_ADRESSE_MAX, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].CodePostal, CLIENT_CodePostal_MAX, SEPARATEUR) == OK)
						{
							idx++;
							if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Ville, CLIENT_VILLE_MAX, SEPARATEUR) == OK)
							{
								idx++;
								if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Nom, CLIENT_NOM_MAX, SEPARATEUR) == OK)
								{
									idx++;
									if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Prenom, CLIENT_PRENOM_MAX, SEPARATEUR) == OK)
									{
										idx++;
										if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].NumeroTel, CLIENT_TELEPHONE_MAX, SEPARATEUR) == OK)
										{
											idx++;
											if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].MotDePasse, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK)
												num_rec++;		/* element à priori correct, on le comptabilise */
										}
									}
								}
							}
						}
					}
				}
			}

		}
		client->NombreDeClients = num_rec;
		fclose(fic_rep);
		return(OK);
	}
}

int main() {
	ClientFile client;
	void *tmpPtr;
	tmpPtr = (void *)malloc(CLIENTFILE_MAX * sizeof(Client));
	if (tmpPtr != NULL) {
		client.TableauDeClient= (Client *)tmpPtr;
	}
	charger(&client, "client.txt");
	printf("%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s", client.TableauDeClient[0].ID, client.TableauDeClient[0].Adresse, client.TableauDeClient[0].CodePostal, client.TableauDeClient[0].MotDePasse, client.TableauDeClient[0].Nom, client.TableauDeClient[0].NumeroTel, client.TableauDeClient[0].Prenom, client.TableauDeClient[0].Ville);
	system("pause");
	return(0);
}