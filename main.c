#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"

#define OK 1
#define ERROR -1

#define MAX_ID_CLIENT 9999 
#define MAX_NOM_CLIENT 21
#define MAX_PRENOM_CLIENT 21
#define MAX_LOCALISATION_CLIENT 50
#define MAX_MDP 4
#define MAX_CODE_POSTAL 5
#define MAX_NUM_TEL 10
#define MAX_VILLE 15

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
		while (!feof(fic_rep) && (client->NombreDeClients < MAX_ID_CLIENT))
		{
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
				if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].ID, MAX_ID_CLIENT, ';') == OK)
				{
					idx++;							/* on saute le separateur */
					if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Adresse, MAX_LOCALISATION_CLIENT, ';') == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].CodePostal, MAX_CODE_POSTAL, ';') == OK)
						{
							idx++;
							if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Ville, MAX_VILLE, ';') == OK)
							{
								idx++;
								if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Nom, MAX_NOM_CLIENT, ';') == OK)
								{
									idx++;
									if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].Prenom, MAX_PRENOM_CLIENT, ';') == OK)
									{
										idx++;
										if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].NumeroTel, MAX_NUM_TEL, ';') == OK)
										{
											idx++;
											if (lire_champ_suivant(buffer, &idx, client->TableauDeClient[num_rec].MotDePasse, MAX_MDP, ';') == OK)
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