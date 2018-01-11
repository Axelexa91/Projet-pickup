#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Structures.h"
#include "Define.h"

/**********************************************************************/
/*                          Initialisation                            */
/**********************************************************************/

/*Cette fonction est activée à chaque début de fuseaux des 4h ainsi qu'à chaque nouvelle exécution.
Elle permet de créer un tableau dans lequel sera contenu toutes les commandes effectuées durant les 4h.
Elle permet égalemnt d'initialiser le tableau avant de charger le contenu du fichier vente appelé "commande".*/
int init_tableau_commandes(CommandeFile* AllCommandes) {
	void*Ptr = (void*)malloc(COMMANDEFILE_MAX * sizeof(Commande)); //Créer un tableau
	if (Ptr != NULL) {
		AllCommandes->TableauDesCommandes = Ptr;
		AllCommandes->NombreDeCommande = 0;
		return OK;
	}
	else return ERROR;
}

/*Cette fonction est activée à chaque nouvelle Commande entrée par l'utilisateur.
Elle permet de créer un tableau dans lequel sera contenu la commande en question
contenue dans le fichier FichierAchat.*/
int init_Commande(Commande* Com) {
	Com->ArticleID = 0;
	Com->ClientID = 0;
	Com->CodeRetrait = 0;
	Com->Quantity = 0;
	return OK;
}


/*Cette fonction est activée lors de la connexion du client ou après la validation du panier.
Elle permet d'initialiser le panier dans lequel sera disposer ses commandes éventuels.*/
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

	// Création d'une nouvelle commande
	Commande nouvelarticle;

	nouvelarticle.ArticleID = ID_article;
	nouvelarticle.ClientID = ID_client;
	nouvelarticle.Quantity = quantite;
	srand(time(NULL));							// Cette fonction permet de générer un nombre aléatoire
	nouvelarticle.CodeRetrait = rand() % 10000;	// différent à chaque éxécution

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

/* Cette fonction permet de valider la totalité du panier, les commandes seront alors définitives.*/
int validation_panier(CommandeFile* AllCommandes, CommandeFile* Panier) {

	//Ajout du panier au tableau de toutes les commandes des 4h
	for (int indice = 0; indice < Panier->NombreDeCommande; indice++) {
		*(AllCommandes->TableauDesCommandes + AllCommandes->NombreDeCommande + indice) = *(Panier->TableauDesCommandes + indice);
	}

	// Mise à 0 du panier
	AllCommandes->NombreDeCommande += Panier->NombreDeCommande;
	init_panier(Panier);

	return(OK);
}

/*Cette fonction sera activée au bout des fuseaux de 4h elle permettra de mettre à 0 le fichier vente et de sauvegarder
les commandes de ces 4 dernières heures dans celui-ci.*/
int envoi_commandes(CommandeFile *AllCommandes, char nom_fichier[]) {

	FILE *fic_rep;					/* le fichier */

	if ((fopen_s(&fic_rep, nom_fichier, "w")) != 0) {
		// Ouverture du fichier possédant le nom compris dans nom_fichier si il existe sinon création d'un fichier possédant ce nom
		// Mode d'utilisation "w" = write (écrire uniquement)
		// Si la fonction fopen_s renvoie autre chose que 0 cela signifie qu'elle n'a pas fonctionnée
		return(ERROR);
	}
	else { // si la fonction fopen_s a fonctionnée
		for (int i = 0; i < AllCommandes->NombreDeCommande; i++) {
			fprintf_s(fic_rep, "%04d;%04d;%04d;%04d;\n", (AllCommandes->TableauDesCommandes + i)->ClientID, (AllCommandes->TableauDesCommandes + i)->ArticleID, (AllCommandes->TableauDesCommandes + i)->Quantity, (AllCommandes->TableauDesCommandes + i)->CodeRetrait);
			// on affiche dans le fichier chaque élément de toutes les commandes
		}
		fclose(fic_rep); // On ferme le fichier qui a été ouvert
		return(OK);
	}

}

/**********************************************************************/
/*                      Fonctions charger                             */
/**********************************************************************/

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
								num_rec++;		/* element à priori correct, on le comptabilise */
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
		while (!feof(fic_rep) )
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
					New.ClientID= atoi(mot);
					if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
					{
						idx++;							/* on saute le separateur */
						New.ArticleID= atoi(mot);
						if (lire_champ_suivant(buffer, &idx, mot, 5, SEPARATEUR) == OK)
						{
							New.Quantity= atoi(mot);
						
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
	CommandeFile Panier; // Contient toutes les commandes effectués par un client durant son achat
	Commande NewAchat;
	init_panier(&Panier);
	init_tableau_commandes(&AllCommandes);
	init_Commande(&NewAchat);
	charger_fichier_vente(&AllCommandes, "commande");
	NewAchat=charger_fichier_achat(NewAchat,FichierAchat);
	ajout_panier(&Panier, NewAchat.ClientID, NewAchat.ArticleID, NewAchat.Quantity);
	validation_panier(&AllCommandes, &Panier);
	envoi_commandes(&AllCommandes, "commande");
	return 1;
}

int main() {
	int a=Gestion_Achat("achat");
}