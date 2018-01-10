#include "Test2.h"
#include <string.h>

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
}

int ChargerCommande(CommandeFile * commande, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Commande);
	char buffer[sizeof(Commande) + 1];
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
				char mot[30];

				if (lire_champ_suivant(buffer, &idx, mot, CLIENT_ID_MAX, SEPARATEUR) == OK)//Problème dans lire_champ_suivant
				{
					idx++;							/* on saute le separateur */
					commande->TableauDesCommandes[num_rec].ArticleID = atoi(mot);
					if (lire_champ_suivant(buffer, &idx, mot, CLIENT_ID_MAX, SEPARATEUR) == OK)
					{
						idx++;
						commande->TableauDesCommandes[num_rec].ClientID= atoi(mot);
						if (lire_champ_suivant(buffer, &idx, mot, COMMANDEFILE_MAX, SEPARATEUR) == OK)
						{
							idx++;
							commande->TableauDesCommandes[num_rec].CodeRetrait = atoi(mot);
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivant(buffer, &idx, mot, CODE_RETRAIT_MAX, SEPARATEUR) == OK)
							{
								commande->TableauDesCommandes[num_rec].Quantity = atoi(mot);
								num_rec++;
							}
						}
					}
				}
			}
		}
		commande->NombreDeCommande = num_rec;
		fclose(fic_rep);
		printf("%d\n", num_rec);
		printf("%d\n\n", commande->NombreDeCommande);
		return(OK);
	}
}

int main() {
	CommandeFile commande;
	void *tmpPtr;
	tmpPtr = (void *)malloc(COMMANDEFILE_MAX * sizeof(Commande));
	if (tmpPtr != NULL) {
		commande.TableauDesCommandes = (Commande *)tmpPtr;
	}
	ChargerCommande(&commande, "commande.txt");
	printf("%d\n%d\n%d\n%d\n", commande.TableauDesCommandes[0].ArticleID, commande.TableauDesCommandes[0].ClientID, commande.TableauDesCommandes[0].CodeRetrait, commande.TableauDesCommandes[0].Quantity);
	printf("%d\n%d\n%d\n%d\n", commande.TableauDesCommandes[1].ArticleID, commande.TableauDesCommandes[1].ClientID, commande.TableauDesCommandes[1].CodeRetrait, commande.TableauDesCommandes[1].Quantity);
	system("pause");
	return(0);
}

