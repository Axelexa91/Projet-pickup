#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GestionPickUp.h"


#define OK 1
#define ERROR 0
#define MAX_IDArticle 4
#define MAX_Dim 3
#define SEPARATEUR ';'
#define MAX_Article 4
#define MAX_ID_PICKUP 3
#define MAX_ID_SLOT 4

int RecCapacite (int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite)
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
	//On trie le dimensions de la boîte pour la comparaison
	
	if ((H_article <= H_boite) && (L_article <= L_boite) && (P_article <= P_boite))
	{
		
		return (1 + RecCapacite(P_article, L_article, H_article, H_boite - H_article, L_article, P_article) /*Dessus*/ + RecCapacite(P_article, L_article, H_article, H_boite , L_boite , P_boite - P_article) /*Fond*/ + RecCapacite(P_article, L_article, H_article, H_boite, L_boite-L_article, P_article) /*Côté*/ );
	}
	else
	{
		return(0);
	}
}

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


/**********  Chargement de la commande **********/

ListArticle * newListArticle()
{
	ListArticle *tmp = malloc(sizeof(ListArticle));
	//On alloue dynamiquement l'espace mémoire pour définir la liste chaînée
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a réussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head à NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'élement de type LinkedList mais aussi tous les éléments qui la constitue avec free! 

SingelArticleDansSlotElement* newArticleDansSlot (ArticleDansSlot value)
{
	SingelArticleDansSlotElement *Elem = malloc(sizeof(SingelArticleDansSlotElement));
	//On alloue dynamiquement la mémoire nécessaire à la création d'un maillon de la chaîne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a réussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la chaîne
		(*Elem).next = NULL;
		//L'élément suivant est initialise à NULL et sera modifier lors de l'insertion de l'élément dans la liste
	}
	return(Elem);
}

int appendArticleDansSlot (ListArticle *MyList, ArticleDansSlot value, int Size)
{
	SingelArticleDansSlotElement *NewElem = newArticleDansSlot(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entités NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe déjà une queue à la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel élément en reliant le dernier élément de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet élément devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'élément et l'élément ajouté est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = MyList->Size + 1;
		return(Size + 1);//La taille est augmenté de 1 (on retourne la taille de la liste 
	}
	return(-1);//Retourne le code erreur -1
}

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

int ChargerCommande(ListArticle *rep, char nom_fichier[])
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
				ArticleDansSlot tmp = {-1, -1, -1, -1, -1};
				char mot[10];
				//On définit un enregistrement dans lequel on va enregistré le nom, le prénom et le numéro de téléphone lu sur la ligne enregistré dans le buffer
				if (lire_champ_suivant(buffer, &idx, mot, MAX_IDArticle+1, SEPARATEUR) == OK)//Problème dans lire_champ_suivant
				{
					idx++;							/* on saute le separateur */
					tmp.IDArticle =atoi(mot);
					if (lire_champ_suivant(buffer, &idx,mot, MAX_Article+1, SEPARATEUR) == OK)
					{
						idx++;
						tmp.quantite = atoi(mot);
						if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK)
						{
							idx++;
							tmp.largeur =atoi(mot);
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK)
							{
								idx++;
								tmp.profondeur = atoi(mot);
								if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK)
									    tmp.hauteur = atoi(mot);
								        num_rec = appendArticleDansSlot(rep, tmp, num_rec);      /* element à priori correct, on le comptabilise */
								//num_rec contient la taille de la liste chaînée
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

/********** Chargement du fichier Station *********/

StationFile * newStationFile()
{
	StationFile *tmp = malloc(sizeof(StationFile));
	//On alloue dynamiquement l'espace mémoire pour définir la liste chaînée
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a réussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).Size = 0;
		//On initialise tail et head à NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'élement de type LinkedList mais aussi tous les éléments qui la constitue avec free! 

SingelStationElement * newSingelStationElement(Station value)
{
	SingelStationElement  *Elem = malloc(sizeof(SingelStationElement));
	//On alloue dynamiquement la mémoire nécessaire à la création d'un maillon de la chaîne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a réussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la chaîne
		(*Elem).next = NULL;
		//L'élément suivant est initialise à NULL et sera modifier lors de l'insertion de l'élément dans la liste
	}
	return(Elem);
}

int appendStationElement(StationFile *MyList, Station value, int Size)
{
	SingelStationElement *NewElem = newSingelStationElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entités NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe déjà une queue à la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel élément en reliant le dernier élément de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet élément devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'élément et l'élément ajouté est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).Size = (*MyList).Size + 1;
		return(Size + 1);//La taille est augmenté de 1 (on retourne la taille de la liste 
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
				Station newSlot = {-1, -1, -1, -1, -1, -1}; //on crée un élément de type contact dans lequel on va enregistrer les chaines de caractères du fichier
				char mot[10];
				if (lire_champ_suivant(buffer, &idx, mot, MAX_ID_PICKUP+1, SEPARATEUR) == OK) //On vérifie que e nom est OK
				{
					idx++;							/* on saute le separateur */
					newSlot.PickupStationID = atoi(mot);
					if (lire_champ_suivant(buffer, &idx, mot, MAX_ID_SLOT+1, SEPARATEUR) == OK) //On vérifie que le prnom est OK
					{
						idx++;
						newSlot.SlotID = atoi(mot);
						if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK) //On vérifie que le prnom est OK
						{
							idx++;
							newSlot.Profondeur = atoi(mot);
							if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK) //On vérifie que le prnom est OK
							{
								idx++;
								newSlot.Largeur = atoi(mot);
								if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK) //On vérifie que le prnom est OK
								{
									idx++;
									newSlot.Hauteur = atoi(mot);
									if (lire_champ_suivant(buffer, &idx, mot, MAX_Dim+1, SEPARATEUR) == OK) //on verifie que le tel est OK
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
		return(OK);
	}


} /* fin charger */







/********** Etablissement liste quantité article dans slots dispos ************/

SlotDispoFile * newSlotDispoFile()
{
	SlotDispoFile *tmp = malloc(sizeof(SlotDispoFile));
	//On alloue dynamiquement l'espace mémoire pour définir la liste chaînée
	if (tmp != NULL)
	{
		//Dans ce cas, l'allocation dynamique a réussi
		(*tmp).head = NULL;
		(*tmp).tail = NULL;
		(*tmp).size = 0;
		//On initialise tail et head à NULL car la liste est vide pour le moment
	}
	return(tmp);
}
//Attention pour supprimer la liste, il ne suffit pas de supprimer l'élement de type LinkedList mais aussi tous les éléments qui la constitue avec free! 

SingelSlotDispoElement * newSingelSlotDispoElement(SlotDispo value)
{
	SingelSlotDispoElement  *Elem = malloc(sizeof(SingelSlotDispoElement));
	//On alloue dynamiquement la mémoire nécessaire à la création d'un maillon de la chaîne (de type SingelLinkedListElem)
	if (Elem != NULL)
	{
		//Dans ce cas, l'allocation dynamique a réussi
		(*Elem).data = value;
		//On met l'enregistrement dans le maillon de la chaîne
		(*Elem).next = NULL;
		//L'élément suivant est initialise à NULL et sera modifier lors de l'insertion de l'élément dans la liste
	}
	return(Elem);
}

int appendSlotDispoElement(SlotDispoFile *MyList, SlotDispo value, int Size)
{
	SingelSlotDispoElement *NewElem = newSingelSlotDispoElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
	if ((NewElem != NULL) && (MyList != NULL)) //On peut manipuler les deux entités NewElem et MyList
	{
		if (((*MyList).tail) != NULL)//Dans ce cas, la liste n'est pas vide et il existe déjà une queue à la liste
		{
			((*MyList).tail)->next = NewElem; //On ajoute le nouvel élément en reliant le dernier élément de la liste au nouveau maillon
			(*MyList).tail = NewElem;//Cet élément devient la queue de la liste
		}
		else
		{
			//Dans ce cas, il n'y a pas encore d'élément et l'élément ajouté est le premier mais aussi le dernier
			(*MyList).tail = NewElem;
			(*MyList).head = NewElem;
		}
		(*MyList).size = (*MyList).size + 1;
		return(Size + 1);//La taille est augmenté de 1 (on retourne la taille de la liste 
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
		for (int j = 0; j < sta->Size; j++)
		{
			if (ptr->data.Disponible == 0) //Le slot est disponible
			{
				SingelArticleDansSlotElement * ptrart = art->head;
				for (int i = 0; i < art->Size; i++)
				{
					cap = Capacite(ptrart->data.profondeur, ptrart->data.largeur, ptrart->data.hauteur, ptr->data.Profondeur, ptr->data.Largeur, ptr->data.Hauteur);
					if (cap != 0)
					{
						SlotDispo newData;
						newData.ID_article = ptrart->data.IDArticle;
						newData.ID_PickUP_Station = ptr->data.PickupStationID;
						newData.ID_slot = ptr->data.SlotID;
						newData.quantite = cap;
						appendSlotDispoElement(disp, newData, disp->size);
					}
					ptrart = ptrart->next;
				}
			}
			ptr = ptr->next;
		}
	}
	return(OK);
}

/********** Sauvegarde du résultat ***********/

int SauvegarderDisponibilite (SlotDispoFile *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingelSlotDispoElement *myPtr = rep->head;
	//On place le premier élément sur la tête de la liste
	int Nb_elts = 1;
	//On peut faire écriture seul car on peut effacer les données présent dans le document s'il existe déjà.
	//Le répertoire est en intégralité dans rep.
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
			//On va vers l'élément suivant. On n'aura pas de problème lié à un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistré tous les éléments. On ne cherche donc pas à afficher l'élément nb_elts (sui est le nb_elts+1 ème élément du répertoire) qui a une adresse NULL car non définit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien passée
		return(OK);
	}
}



/*********** Fonction qui donne la réponse avec fichiers de départs **********/

int RetourDisponibiliteSlot(char FichierCommande[], char FichierStation[] /*Nom du fichier donné lors de l'appel par DataCenter*/, char FichierRetour[])
{
	ListArticle * art = newListArticle();
	StationFile * sta = newStationFile();
	SlotDispoFile * disp = newSlotDispoFile();
	if ((art != NULL) && (sta != NULL) && (disp != NULL))
	{
	
		ChargerCommande(art, FichierCommande);
		
		ChargerStation(sta, FichierStation);
		
		ParcoursStation(art, sta, disp);
		
		SauvegarderDisponibilite(disp, FichierRetour);
		return(EXIT_SUCCESS);
	}
	else return (EXIT_FAILURE);
}






/********** Test *********/

int main()
{
	printf("%d\n", Capacite(5, 5, 5, 15, 15, 15));
	printf("%d\n", Capacite(8, 8, 8, 16, 16, 16));
	printf("%d\n", Capacite(8, 12, 4, 16, 16, 16));
	system("pause");
	RetourDisponibiliteSlot("CommandeTest.txt", "StationTest2.txt", "ResultatTest.txt");
	system("pause");
	return(EXIT_SUCCESS);
}
