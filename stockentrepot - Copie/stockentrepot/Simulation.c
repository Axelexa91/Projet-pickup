
void compact(char *s)
{
	int decallage = 0, indice = 0;
	do
	{
		if ((*(s + indice) >= '9') || (*(s + indice) <= '0'))
		{
			//Dans ce cas il ne s'agit pas d'un chiffre et le décallage sera augmenté de 1 !!!
			decallage++;
		}
		else
		{
			*(s + indice - decallage) = *(s + indice);
		}
		indice++;
	} while ((*(s + indice) != '\0') && (indice < 21));
	if (indice < 21)
		*(s + indice - decallage) = '\0'; //ajout du cractère de fin de mot
										  //On n'est pas obligé de remettre les autres éléments à 0 CAR '\0' signifie qu'on est arrivé à la fin du mot.
	return;
}

int lire_champ_suivant1(char *ligne, int *idx, char *champ, int taille_champ, char separateur)
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
		return(OK1);
	}
	else return(ERROR);		/* fin de ligne ou séparateur non atteints */

} /* fin lire_champ_suivant1() */

/***********************************************************/
/************ Chargement du Fichier Ventes *****************/
/***********************************************************/

VentesFile * newVentesFile()
{
	VentesFile *tmp = malloc(sizeof(VentesFile));
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

SingleVentesElement * newSingleVentesElement(Ventes value)
{
	SingleVentesElement  *Elem = malloc(sizeof(SingleVentesElement));
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

int appendVentesElement(VentesFile *MyList, Ventes value, int Size)
{
	SingleVentesElement *NewElem = newSingleVentesElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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

int ChargerVentes(VentesFile *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(Ventes);
	char buffer[sizeof(Ventes) + 1];
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
				Ventes newSlot = { -1, -1, -1, -1 }; //on crée un élément de type contact dans lequel on va enregistrer les chaines de caractères du fichier
				char mot[10];
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					newSlot.ClientID = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
					{
						idx++;
						newSlot.ArticleID = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
						{
							idx++;
							newSlot.Quantity = atoi(mot);
							if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
							{
								idx++;
								newSlot.CodeRetrait = atoi(mot);
								num_rec++;
							}
						}
					}
				}
				appendVentesElement(rep, newSlot, num_rec--);
			}
		}
		fclose(fic_rep);
		return(OK1);
	}
} /* fin charger */

int SauvegarderVentesNonTraitee(VentesFile *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingleVentesElement *myPtr = rep->head;
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
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d\n", myPtr->data.ClientID, SEPARATEUR, myPtr->data.ArticleID, SEPARATEUR, myPtr->data.Quantity, SEPARATEUR, myPtr->data.CodeRetrait);
			myPtr = myPtr->next;
			//On va vers l'élément suivant. On n'aura pas de problème lié à un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistré tous les éléments. On ne cherche donc pas à afficher l'élément nb_elts (sui est le nb_elts+1 ème élément du répertoire) qui a une adresse NULL car non définit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien passée
		return(OK1);
	}
}

/***************************************************************************/
/**********  Chargement fichiers disponibilité PICKUP Stations *************/
/***************************************************************************/

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

int ChargerDispoSlot(SlotDispoFile *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(SlotDispo);
	char buffer[sizeof(SlotDispo) + 1];
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
				SlotDispo newSlot = { -1, -1, -1, -1 }; //on crée un élément de type contact dans lequel on va enregistrer les chaines de caractères du fichier
				char mot[10];
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					newSlot.ID_PickUP_Station = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
					{
						idx++;
						newSlot.ID_article = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
						{
							idx++;
							newSlot.quantite = atoi(mot);
							if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
							{
								idx++;
								newSlot.ID_slot = atoi(mot);
								num_rec++;
							}
						}
					}
				}
				appendSlotDispoElement(rep, newSlot, num_rec--);
			}
		}
		fclose(fic_rep);
		return(OK1);
	}
} /* fin charger */

/************************************************/
/**********  Chargement de la commande **********/
/************************************************/

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

SingelArticleDansSlotElement* newArticleDansSlot(ArticleDansSlot value)
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

int appendArticleDansSlot(ListArticle *MyList, ArticleDansSlot value, int Size)
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

int ChargerCommande1(ListArticle *rep, char nom_fichier[])
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
				ArticleDansSlot tmp = { -1, -1, -1, -1, -1 };
				char mot[10];
				//On définit un enregistrement dans lequel on va enregistré le nom, le prénom et le numéro de téléphone lu sur la ligne enregistré dans le buffer
				if (lire_champ_suivant1(buffer, &idx, mot, MAX_IDArticle + 1, SEPARATEUR) == OK1)//Problème dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.IDArticle = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, MAX_Article + 1, SEPARATEUR) == OK1)
					{
						idx++;
						tmp.quantite = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
						{
							idx++;
							tmp.largeur = atoi(mot);
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivant1(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
							{
								idx++;
								tmp.profondeur = atoi(mot);
								if (lire_champ_suivant1(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
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

/**************************************************************************/
/************* Trie des Ventes par Dim article et quantité ****************/
/**************************************************************************/

int set(VentesFile *MyList, int i, Ventes value, int Size)//équivalent à insert mais sans le décallage
{
	int j = 0;
	if ((MyList != NULL) && (i < Size) && (i >= 0))
	{
		SingleVentesElement *tmp = (*MyList).head;
		j = 1;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'élément i
		(*tmp).data = value;
		return(Size);
	}
	return(-1);
}

SingleVentesElement * GetElementAt(VentesFile *MyList, int i, int Size)
{
	int j;
	if ((MyList != NULL) && (i + 1 <= Size) && (i + 1 > 0))
	{
		SingleVentesElement *tmp = (*MyList).head;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'élément i
		return(tmp);
	}
	return(NULL);
}

int swap(VentesFile *MyList, int i, int j, int Size)
{
	int tmp_int;
	if (j < i)
	{
		tmp_int = j;
		j = i;
		i = tmp_int;
	}
	if ((MyList != NULL) && (j < Size) && (i >= 0))
	{
		SingleVentesElement *tmp1 = (GetElementAt(MyList, i, Size));
		SingleVentesElement *tmp2 = (GetElementAt(MyList, j, Size));
		Ventes err1, err2;
		if ((tmp1 != NULL) && (tmp2 != NULL))
		{
			err1 = (*tmp1).data;
			err2 = (*tmp2).data;
			set(MyList, i, err2, Size); //On met l'entier à la pos j en i
			set(MyList, j, err1, Size); //On met la valeur qui était avant en i à la position j
										//Remarque - On ne change pas les adresses ce qui fait qe la chaîne est toujours parcouru dans le même ordre.
										//On a fait un simple échange des données transportées dans les maillons i et j

			return(1);
		}
		return(0);
	}
	return(0);
}

int VolumeArticle(ListArticle * ListArticles, int IdArticle)
{
	if (ListArticles == NULL)
		return(ERROR);
	else
	{
		int trouver = ERROR;//condition qui test si on a trouver l'élément
		int nb_elts = 0; //nombre d'éléments parcourues
		SingelArticleDansSlotElement * ptr = ListArticles->head;
		while ((trouver == ERROR) && (nb_elts < ListArticles->Size))
		{
			if (ptr->data.IDArticle == IdArticle)
			{
				trouver = OK1;
				return((ptr->data.largeur)*(ptr->data.profondeur)*(ptr->data.hauteur));
			}
			else
			{
				ptr = ptr->next;
			}
			nb_elts++;
		}
		return(ERROR);
	}
}

//On commence par ranger les articles les plus petits pour ne pas réserver de slots trop grang alors qu'on en aurait des plus petits capables d'acceuillir le colis
//On trie du plus petit article et de la quantité la moins importante à l'article le plus grand commandé en plus rande capacité
int TrieVente(VentesFile *ListVentes, ListArticle * ListArticles)
{
	if ((ListVentes == NULL) || (ListArticles == NULL))
		return(ERROR);
	else
	{
		for (int i = 1; i < ListVentes->Size; i++)
		{
			SingleVentesElement * ptr = ListVentes->head;
			for (int j = 1; j < ListVentes->Size - i; j++)
			{
				//On met le plus grand article au bout...
				if ((VolumeArticle(ListArticles, ptr->data.ArticleID)) >(VolumeArticle(ListArticles, ptr->next->data.ArticleID)))
					swap(ListVentes, j, j + 1, ListVentes->Size);
				else
				{
					if ((VolumeArticle(ListArticles, ptr->data.ArticleID)) == (VolumeArticle(ListArticles, ptr->next->data.ArticleID)))
					{
						if (ptr->data.Quantity > ptr->next->data.Quantity)
						{
							swap(ListVentes, j, j + 1, ListVentes->Size);
						}
					}
				}
			}
			ptr = ptr->next;
		}
		return (OK1);
	}
}

/************************************************/
/********** Chargement fichier Client ***********/
/************************************************/

ListClient * newListClient()
{
	ListClient *tmp = malloc(sizeof(ListClient));
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

SingleClientElement* newClientElement(Client value)
{
	SingleClientElement *Elem = malloc(sizeof(SingleClientElement));
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

int appendClientElement(ListClient *MyList, Client value, int Size)
{
	SingleClientElement *NewElem = newClientElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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

int ChargerClient(ListClient *rep, char nom_fichier[])
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
				Client tmp = { -1,"" ,"" ,"" ,"","","","" };
				char mot[10];
				//On définit un enregistrement dans lequel on va enregistré le nom, le prénom et le numéro de téléphone lu sur la ligne enregistré dans le buffer
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1)//Problème dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.ID = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, tmp.Nom, CLIENT_NOM_MAX, SEPARATEUR) == OK1)
					{
						idx++;
						if (lire_champ_suivant1(buffer, &idx, tmp.Prenom, CLIENT_PRENOM_MAX, SEPARATEUR) == OK1)
						{
							idx++;
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivant1(buffer, &idx, tmp.Adresse, CLIENT_ADRESSE_MAX, SEPARATEUR) == OK1)
							{
								idx++;
								if (lire_champ_suivant1(buffer, &idx, tmp.Ville, CLIENT_VILLE_MAX, SEPARATEUR) == OK1)
								{
									idx++;
									if (lire_champ_suivant1(buffer, &idx, tmp.CodePostal, CLIENT_CodePostal_MAX, SEPARATEUR) == OK1)
									{
										idx++;
										if (lire_champ_suivant1(buffer, &idx, tmp.NumeroTel, CLIENT_TELEPHONE_MAX, SEPARATEUR) == OK1)
										{
											idx++;
											if (lire_champ_suivant1(buffer, &idx, tmp.MotDePasse, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK1)
											{
												num_rec = appendClientElement(rep, tmp, num_rec);      /* element à priori correct, on le comptabilise */
											}				 //num_rec contient la taille de la liste chaînée
										}
									}
								}
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

/******************************************************************************/
/********** Charger distances des stations par rapports aux clients************/
/******************************************************************************/

RecupDistanceList * newRecupDistanceList()
{
	RecupDistanceList *tmp = malloc(sizeof(RecupDistanceList));
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

SingleRecupDistanceElement* newRecupDistanceElement(RecupDistance value)
{
	SingleRecupDistanceElement *Elem = malloc(sizeof(SingleRecupDistanceElement));
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

int appendRecupDistanceElement(RecupDistanceList *MyList, RecupDistance value, int Size)
{
	SingleRecupDistanceElement *NewElem = newRecupDistanceElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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

int ChargerRecupDistance(RecupDistanceList *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(RecupDistance) + 2;
	char buffer[sizeof(RecupDistance) + 3];
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
			RecupDistance tmp = { -1,-1, -1 };
			char mot[10];
			if (fgets(buffer, long_max_rec, fic_rep) != NULL)
			{
				/* memorisation de l'enregistrement lu dans le tableau */
				buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

				if ((char_nw_line = strchr(buffer, '\n')) != NULL)
					*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

				idx = 0;								/* analyse depuis le debut de la ligne */
														// ajouter code implemention liste
														//On définit un enregistrement dans lequel on va enregistré le nom, le prénom et le numéro de téléphone lu sur la ligne enregistré dans le buffer
				if (lire_champ_suivant1(buffer, &idx, mot, 5, SEPARATEUR) == OK1)//Problème dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.ID_Client = atoi(mot);
					if (lire_champ_suivant1(buffer, &idx, mot, 4, SEPARATEUR) == OK1)
					{
						idx++;
						tmp.ID_Station = atoi(mot);
						if (lire_champ_suivant1(buffer, &idx, mot, CLIENT_MotDePasse_MAX, SEPARATEUR) == OK1)
						{
							tmp.Distance = atoi(mot);
						}				 //num_rec contient la taille de la liste chaînée
					}
				}
				num_rec = appendRecupDistanceElement(rep, tmp, num_rec);      /* element à priori correct, on le comptabilise */
			}
		}
		fclose(fic_rep);
		//ON ferme le fichier car on a lu toute les lignes
		return(EXIT_SUCCESS);
	}
} /* fin charger */

/*********************************************/
/** Sauvegarde slots réservés pour commande **/
/*********************************************/

ReserveSlotList * newReserveSlotList()
{
	ReserveSlotList *tmp = malloc(sizeof(ReserveSlotList));
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

SingleReserveSlotElement* newReserveSlotElement(ReserveSlot value)
{
	SingleReserveSlotElement *Elem = malloc(sizeof(SingleReserveSlotElement));
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

int appendReserveSlotElement(ReserveSlotList *MyList, ReserveSlot value, int Size)
{
	SingleReserveSlotElement *NewElem = newReserveSlotElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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
// Sauvegarde du résultat
int SauvegarderReserveSlot(ReserveSlotList *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingleReserveSlotElement *myPtr = rep->head;
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
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d%c%d%c%d\n", myPtr->data.ID_PickUP, SEPARATEUR, myPtr->data.ID_slot, SEPARATEUR, myPtr->data.ID_article, SEPARATEUR, myPtr->data.quantite, SEPARATEUR, myPtr->data.numero_tel, SEPARATEUR, myPtr->data.login);
			myPtr = myPtr->next;
			//On va vers l'élément suivant. On n'aura pas de problème lié à un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistré tous les éléments. On ne cherche donc pas à afficher l'élément nb_elts (sui est le nb_elts+1 ème élément du répertoire) qui a une adresse NULL car non définit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien passée
		return(OK1);
	}
}

//Tri des stations Pickup en fontion de leurs distances vis-à-vis du client 
int TriStationList(int stationliste[MAX_STATION_PAR_REGION], int distancesliste[MAX_STATION_PAR_REGION], int Nb_elts)
{
	int tmp;
	for (int i = 0; i < Nb_elts - 1; i++)
	{
		for (int j = 0; j < Nb_elts - 1 - i; j++)
		{
			if (distancesliste[j] > distancesliste[j + 1])
			{
				tmp = distancesliste[j];
				distancesliste[j] = distancesliste[j + 1];
				distancesliste[j + 1] = tmp;
				tmp = stationliste[j];
				stationliste[j] = stationliste[j + 1];
				stationliste[j + 1] = tmp;
			}
		}
	}
	return(OK1);
}

int TriStationPourClient(RecupDistanceList *ListDistance, int ID_client, int stationliste[MAX_STATION_PAR_REGION]) {
	if (ListDistance == NULL) return ERROR;
	else {
		int distanceStation[MAX_STATION_PAR_REGION], nb_elts = 0;
		SingleRecupDistanceElement * ptr = ListDistance->head;
		for (int i = 0; i < ListDistance->Size; i++)
		{
			if (ptr->data.ID_Client == ID_client)
			{
				//On pred toutes les stations pickup dans la région du client et les distances qui le sépare d'elles
				stationliste[nb_elts] = ptr->data.ID_Station;
				distanceStation[nb_elts] = ptr->data.Distance;
				nb_elts++;
			}
			ptr = ptr->next;
		}
		//Tableau non trié des id pickup dans la région du client
		TriStationList(stationliste, distanceStation, nb_elts);
		return(nb_elts);
	}
}

//Fonction qui retourne l'identifiant du slot si il peut acceuillir quantité de fois l'article dont l'identifiant vaut ID_article, ou -1 sinon.
int RechercheSlots(int ID_article, SlotDispoFile *slotdispo, int ID_station, int quantite)
{
	//La liste chainée des slots disponibles est trié par quantité croissant
	SingelSlotDispoElement * ptrslotdispo = slotdispo->head;
	if (slotdispo->size != 0)
	{
		for (int i = 0; i < slotdispo->size; i++)
		{
			if ((ptrslotdispo->data.ID_article == ID_article) && (ptrslotdispo->data.ID_PickUP_Station == ID_station))
			{
				if (quantite <= ptrslotdispo->data.quantite)
				{
					return(ptrslotdispo->data.ID_slot);
				}
			}
			ptrslotdispo = ptrslotdispo->next;
		}
		return(ERROR);
	}
	return(ERROR);
}

SingelSlotDispoElement * GetElementAtSlotDispoFile(SlotDispoFile *MyList, int i, int Size)
{
	int j;
	if ((MyList != NULL) && (i + 1 <= Size) && (i + 1 > 0))
	{
		SingelSlotDispoElement *tmp = (*MyList).head;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'élément i
		return(tmp);
	}
	return(NULL);
}

int suppSlotFromDispo(SlotDispoFile *MyList, int i, int Size)
{
	//Le premier élément est à l'indice 0
	int j = 0;
	if ((MyList != NULL) && (i < Size) && (i >= 0))
	{
		SingelSlotDispoElement *tmp = (*MyList).head, *tmp2;
		j = 1;
		if (i == 0)//On supprime l'élément en début de liste (on est sûr qu'il y a au moins un élément)
		{
			(*MyList).head = tmp->next; //S'il n'y avait qu'un seul élément alors la queue et la tête sont à NULL
			free(tmp);
		}
		if ((i == Size - 1) && (i > 0)) //Dans ce cas, on supprime le dernier élément et on est sûr qu'il y en a au moins deux
		{
			tmp = GetElementAtSlotDispoFile(MyList, i - 1, Size);
			free(tmp->next);
			tmp->next = NULL;
		}
		if ((i > 0) && (i < Size - 1))//Dans ce cas, on supprime un élément au milieu de la liste. Or la liste ne peut pas être vide si value > 1 donc il n'y a pas de problème
									  //Il y a bien un antécédent et un suivant à la i-ème position.
		{
			tmp = GetElementAtSlotDispoFile(MyList, i - 1, Size);
			//On enregistre dans tmp l'adresse de l'élément i-1
			//L'élément i-1 doit pointer vers l'élément i+1
			tmp2 = (tmp->next->next);
			free((*tmp).next);//On libére la mémoire de l'élément à la position i
			tmp->next = tmp2; //On dit que l'élément i-1 a pour ntécédent l'élément suivant de l'élément suivant de i-1 (soit l'élément i+1)
		}
		MyList->size = MyList->size - 1;
		return(Size - 1);
	}
	return(ERROR);
}

int SuppInSlotDispoFile(SlotDispoFile * MyList, int Id_PickupStation, int Id_Slot)
{
	if (MyList == NULL)
		return (ERROR);
	else
	{
		SingelSlotDispoElement * ptr = MyList->head;
		for (int i = 0; i < MyList->size; i++)
		{
			if ((ptr->data.ID_PickUP_Station == Id_PickupStation) && (ptr->data.ID_slot == Id_Slot))
			{
				suppSlotFromDispo(MyList, i, MyList->size);
			}
		}
		return(OK1);
	}
}

int rechercheTel(ListClient *clientsliste, int Id_Client)
{
	if (clientsliste == NULL)
		return (ERROR);
	else
	{
		SingleClientElement *ptr = clientsliste->head;
		int trouver = ERROR;
		int nb_elts = 0;
		while ((trouver == ERROR) && (nb_elts < clientsliste->Size))
		{
			if (ptr->data.ID == Id_Client)
			{
				trouver = atoi(ptr->data.NumeroTel);
			}
			ptr = ptr->next;
		}
		return(trouver);
	}
}

//  Trie de la liste SlotDispoFile par capacité d'acceuil
int TriSlotDispoFile(SlotDispoFile *MyList)
{
	if (MyList == NULL)
	{
		return(ERROR);
	}
	else
	{
		for (int i = 0; i < MyList->size - 1; i++)
		{
			SingelSlotDispoElement * ptr = MyList->head;
			for (int j = 0; j < MyList->size - 1 - i; j++)
			{
				if (ptr->data.quantite > ptr->next->data.quantite)
				{
					SlotDispo tmp = ptr->data;
					ptr->data = ptr->next->data;
					ptr->next->data = tmp;
				}
				ptr = ptr->next;
			}
		}
		return (OK1);
	}
}

int ReservationSlot(ReserveSlotList *rep, VentesFile *ventes, RecupDistanceList *distancesList, ListClient *clientsliste, ListArticle * ListArticles, SlotDispoFile * slotdispos, char nom_fichier_reservation_slots[], char nom_fichier_ALivrer[])
{
	//On trie le fichier ventes
	TrieVente(ventes, ListArticles);
	//On trie les slots dispos 
	TriSlotDispoFile(slotdispos);
	//Ainsi, les premières ventes (un article acheté en quantité finie) sont celles nécessitant les slots les moins grands : on grde ainsi les plus grans slot à coup sûr pour des commandes plus volumineuses
	VentesFile * VentesNonTraitees = newVentesFile();
	SingleVentesElement * ptrventes = ventes->head;
	int ID_client, ID_station;
	int trouver = ERROR;
	int stationstrie[MAX_STATION_PAR_REGION];
	//On parcours tous le fichiers des ventes pour essayer de les livrer toutes (trouver un slot disponible)
	for (int i = 0; i < ventes->Size; i++)
	{
		int j = 0;
		trouver = ERROR;
		ID_client = ptrventes->data.ClientID;
		int nb_elts = TriStationPourClient(distancesList, ID_client, stationstrie);
		//Le fichiers disponibilité réunis l'ensembles des réponses envoyées par les stations de la région
		do
		{
			//On teste en priorité la station pickup la plus proche du client (on reste toujours dans sa région)
			trouver = RechercheSlots(ptrventes->data.ArticleID, slotdispos, stationstrie[j], ptrventes->data.Quantity);
			ID_station = stationstrie[j];
			j++;
		} while ((trouver == ERROR) && (j < nb_elts));
		if (trouver != ERROR)
		{
			//Dans ce cas, on a trouvé un slot disponible : le premier pouvant acceuillir le colis (on a donc choisi le slot de taille optimale)
			//On supprime le slot qu'on a réservé pour ne pas le reprendre
			SuppInSlotDispoFile(slotdispos, ID_station, trouver);
			//Il faut réserver le slot : on l'ajoute au fichier qu'on devra envoyer aux stations pickup de la région
			ReserveSlot nouveau = { -1, -1, -1, -1, -1, -1 };
			nouveau.ID_PickUP = ID_station;
			nouveau.ID_slot = trouver;
			nouveau.ID_article = ptrventes->data.ArticleID;
			nouveau.quantite = ptrventes->data.Quantity;
			nouveau.numero_tel = rechercheTel(clientsliste, ptrventes->data.ClientID);
			nouveau.login = ptrventes->data.CodeRetrait;
			appendReserveSlotElement(rep, nouveau, rep->Size);
		}
		else
		{
			//La commande ne peut être livrée dans la région : on remet la livraison à plus tard
			appendVentesElement(VentesNonTraitees, ptrventes->data, VentesNonTraitees->Size);
		}
		ptrventes = ptrventes->next;
	}
	//On sauvegarde la réservation des slots
	SauvegarderReserveSlot(rep, nom_fichier_reservation_slots);
	//On sauvegarde dns le fichiers ALivrer toutes les commandes qui n'ont pû être livré à cause d'un manque de place (en attente)
	SauvegarderVentesNonTraitee(VentesNonTraitees, nom_fichier_ALivrer);
	//Remarque - on réécrit dans le même fichier (par dessus)
	return(OK1);
}

//On fait le choix de mettre une ligne de commande dans un seul slot !!!

//Fonction qui ouvre un fichier renvoyant vers tous les résultats émis par les stations pickup d'une région :
//On cherge toutes les disponibilité dans la même structure
int OuvertureFichiersSlotsDispos(SlotDispoFile * rep, char nom_fichier_ensemble_slots_dispos_par_region[])
{
	if (rep == NULL) return (ERROR);
	else
	{
		FILE *fic_rep;					/* le fichier */
		errno_t err;
		int num_rec = 0;						/* index sur enregistrements */
		int long_max_rec = sizeof(int) + 51 * sizeof(char);
		char buffer[sizeof(int) + 51 * sizeof(char) + 1];
		int idx = 0;
		char *char_nw_line;
		_set_errno(0);
		if ((err = fopen_s(&fic_rep, nom_fichier_ensemble_slots_dispos_par_region, "r")) != 0)
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
					char mot[51];
					if (lire_champ_suivant1(buffer, &idx, mot, 4, SEPARATEUR) == OK1) //On vérifie que e nom est OK1
					{
						idx++;							/* on saute le separateur */
						if (lire_champ_suivant1(buffer, &idx, mot, 51, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
						{
							ChargerDispoSlot(rep, mot);
						}
					}
				}
			}
			fclose(fic_rep);
			return(OK1);
		}
	}
}

//Fonction qui détermine les slots qui vont être réservé dans une région
int DispatchingPickUP(char EnsembleSlotsDisposParRegion[], char Ventes[], char ALivrer[], char Distances[], char Clients[] /*seul fichiers qui ne dépend pas de la région*/, char Commande[], char ReservationSlots [])
{
	SlotDispoFile * listslotsdispos = newSlotDispoFile();
	OuvertureFichiersSlotsDispos(listslotsdispos, EnsembleSlotsDisposParRegion);
	ReserveSlotList * listreservationslots = newReserveSlotList();
	VentesFile *ventes = newVentesFile();
	ChargerVentes(ventes, Ventes);
	ChargerVentes(ventes, ALivrer);
	RecupDistanceList * distanceList = newRecupDistanceList();
	ChargerRecupDistance(distanceList, Distances);
	ListClient * clientsliste = newListClient();
	ChargerClient(clientsliste, Clients);
	ListArticle * ListArticles = newListArticle();
	ChargerCommande1(ListArticles, Commande);
	ReservationSlot(listreservationslots, ventes, distanceList, clientsliste, ListArticles, listslotsdispos, ReservationSlots, /*"Ventes.txt" */ALivrer);
	return(EXIT_SUCCESS);
}

/******************************************************/
/******************************************************/
/************* Fonctions Stations PickUP **************/
/******************************************************/
/******************************************************/

int RecCapacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite)
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
		return (1 + RecCapacite(P_article, L_article, H_article, H_boite - H_article, L_article, P_article) /*Dessus*/ + RecCapacite(P_article, L_article, H_article, H_boite, L_boite, P_boite - P_article) /*Fond*/ + RecCapacite(P_article, L_article, H_article, H_boite, L_boite - L_article, P_article) /*Côté*/);
	}
	else
	{
		return(0);
	}
}

//Fonction donne cpacité d'acceuil d'un article dans un slot
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

/***************************************************************/
/**********  Chargement de la commande (d'une région) **********/
/***************************************************************/

ListArticle * newListArticlePickUP()
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

SingelArticleDansSlotElement* newArticleDansSlotPickUP(ArticleDansSlot value)
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

int appendArticleDansSlotPickUP(ListArticle *MyList, ArticleDansSlot value, int Size)
{
	SingelArticleDansSlotElement *NewElem = newArticleDansSlotPickUP(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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

int lire_champ_suivantPickUP(char *ligne, int *idx, char *champ, int taille_champ, char separateur)
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
		return(OK1);
	}
	else return(ERROR);		/* fin de ligne ou séparateur non atteints */

} /* fin lire_champ_suivant1() */

//Chargement du fichier envoyé par le dispatching avec l'intégralié des articles commandés et leurs quantités dans une région
int ChargerCommandePickUP(ListArticle *rep, char nom_fichier[])
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
				ArticleDansSlot tmp = { -1, -1, -1, -1, -1 };
				char mot[10];
				//On définit un enregistrement dans lequel on va enregistré le nom, le prénom et le numéro de téléphone lu sur la ligne enregistré dans le buffer
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_IDArticle + 1, SEPARATEUR) == OK1)//Problème dans lire_champ_suivant1
				{
					idx++;							/* on saute le separateur */
					tmp.IDArticle = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Article + 1, SEPARATEUR) == OK1)
					{
						idx++;
						tmp.quantite = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
						{
							idx++;
							tmp.largeur = atoi(mot);
							//Et on l'enregistre dans le repertoire (variable)
							if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
							{
								idx++;
								tmp.profondeur = atoi(mot);
								if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1)
									tmp.hauteur = atoi(mot);
								num_rec = appendArticleDansSlotPickUP(rep, tmp, num_rec);      /* element à priori correct, on le comptabilise */
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

/**************************************************/
/********** Chargement du fichier Station *********/
/**************************************************/

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
				Station newSlot = { -1, -1, -1, -1, -1, -1 }; //on crée un élément de type contact dans lequel on va enregistrer les chaines de caractères du fichier
				char mot[10];
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_PICKUP + 1, SEPARATEUR) == OK1) //On vérifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					newSlot.PickupStationID = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_SLOT + 1, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
					{
						idx++;
						newSlot.SlotID = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
						{
							idx++;
							newSlot.Profondeur = atoi(mot);
							if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
							{
								idx++;
								newSlot.Largeur = atoi(mot);
								if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
								{
									idx++;
									newSlot.Hauteur = atoi(mot);
									if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_Dim + 1, SEPARATEUR) == OK1) //on verifie que le tel est OK1
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
		return(OK1);
	}


} /* fin charger */

/****************************************************************************************************************/
/********** Etablissement de la liste des capacité d'acceuils des article dans les slots disponibles ************/
/****************************************************************************************************************/

SlotDispoFile * newSlotDispoFilePickUP()
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

SingelSlotDispoElement * newSingelSlotDispoElementPickUP(SlotDispo value)
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

int appendSlotDispoElementPickUP(SlotDispoFile *MyList, SlotDispo value, int Size)
{
	SingelSlotDispoElement *NewElem = newSingelSlotDispoElementPickUP(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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
		//On parcours l'intégralité de la station
		for (int j = 0; j < sta->Size; j++)
		{
			if (ptr->data.Disponible == 0) //Le slot est disponible : on calcul donc sa capacité d'acceuil pour chaque article de la commande
			{
				SingelArticleDansSlotElement * ptrart = art->head;
				for (int i = 0; i < art->Size; i++)
				{
					cap = Capacite(ptrart->data.profondeur, ptrart->data.largeur, ptrart->data.hauteur, ptr->data.Profondeur, ptr->data.Largeur, ptr->data.Hauteur);
					if (cap != 0)
					{
						//Dans ce cas, le slot peut acceuillir au moins une fois l'article -> on l'enregistre comme slot disponible pour acceuillir cet article
						SlotDispo newData;
						newData.ID_article = ptrart->data.IDArticle;
						newData.ID_PickUP_Station = ptr->data.PickupStationID;
						newData.ID_slot = ptr->data.SlotID;
						newData.quantite = cap;
						appendSlotDispoElementPickUP(disp, newData, disp->size);
					}
					ptrart = ptrart->next;
				}
			}
			ptr = ptr->next;
		}
	}
	return(OK1);
}

/********** Sauvegarde du résultat ***********/
int SauvegarderDisponibilite(SlotDispoFile *rep, char nom_fichier[])
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
		return(OK1);
	}
}

/*********** Fonction qui donne la réponse avec fichiers de départs **********/

int RetourDisponibiliteSlot(char FichierCommande[], char FichierStation[] /*Nom du fichier donné lors de l'appel par DataCenter*/, char FichierRetour[])
{
	ListArticle * art = newListArticlePickUP();
	StationFile * sta = newStationFile();
	SlotDispoFile * disp = newSlotDispoFilePickUP();
	if ((art != NULL) && (sta != NULL) && (disp != NULL))
	{
		ChargerCommandePickUP(art, FichierCommande);
		ChargerStation(sta, FichierStation);
		ParcoursStation(art, sta, disp);
		SauvegarderDisponibilite(disp, FichierRetour);
		return(EXIT_SUCCESS);
	}
	else return (EXIT_FAILURE);
}

//Fonction qui renvoie toutes les disponibilités des stations pickup pour les quelles on appelle la fonction précédente
int ResultatCapacite(char CommandeRegion[], char Station1Region[], char Station2Region[], char Dispo1Region[], char Dispo2Region[])
{
	//Dans le cas où il y a plus de stations par région et plus de région, on peut écrire une fonction qui va rechercher le noms des stations dans le fichier global des stations
	//On peut "automatiser" cette fonction
	RetourDisponibiliteSlot(CommandeRegion, Station1Region, Dispo1Region);
	RetourDisponibiliteSlot(CommandeRegion, Station2Region, Dispo2Region);
	return(EXIT_SUCCESS);
}

/*********************************************************************/
/*********************************************************************/
/******************* Modification Disponibilite **********************/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************/
/*************** Chargement du fichier de Réservation ****************/
/*********************************************************************/

ReserveSlotList * newReserveSlotListPickUP()
{
	ReserveSlotList *tmp = malloc(sizeof(ReserveSlotList));
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

SingleReserveSlotElement* newReserveSlotElementPickUP(ReserveSlot value)
{
	SingleReserveSlotElement *Elem = malloc(sizeof(SingleReserveSlotElement));
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

int appendReserveSlotElementPickUP(ReserveSlotList *MyList, ReserveSlot value, int Size)
{
	SingleReserveSlotElement *NewElem = newReserveSlotElementPickUP(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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

int ChargerReserveSlotListPickUP(ReserveSlotList *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(ReserveSlot);
	char buffer[sizeof(ReserveSlot) + 1];
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
				ReserveSlot ptr = { -1, -1, -1, -1, -1, -1 }; //on crée un élément de type contact dans lequel on va enregistrer les chaines de caractères du fichier
				char mot[10];
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_PICKUP + 1, SEPARATEUR) == OK1) //On vérifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					ptr.ID_PickUP = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_SLOT + 1, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
					{
						idx++;
						ptr.ID_slot = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
						{
							idx++;
							ptr.ID_article = atoi(mot);
							if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
							{
								idx++;
								ptr.quantite = atoi(mot);
								if (lire_champ_suivantPickUP(buffer, &idx, mot, 20, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
								{
									idx++;
									ptr.numero_tel = atoi(mot);
									if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //on verifie que le tel est OK1
										ptr.login = atoi(mot);
									num_rec++;
								}
							}
						}
					}
				}
				appendReserveSlotElementPickUP(rep, ptr, num_rec--);
			}

		}
		fclose(fic_rep);
		return(OK1);
	}


} /* fin charger */

/***********************************************************************/
/******************* Création fichier slots occupés ********************/
/***********************************************************************/

//Fichier qui content le login et mot de passe des slots occupés dans une station pickup

SlotOccuppeList * newSlotOccupeP()
{
	SlotOccuppeList *tmp = malloc(sizeof(SlotOccuppeList));
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

SingleSlotOccuppeElement* newSlotOccupeElement(SlotOccuppe value)
{
	SingleSlotOccuppeElement *Elem = malloc(sizeof(SingleSlotOccuppeElement));
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

int appendSlotOccupeElement(SlotOccuppeList *MyList, SlotOccuppe value, int Size)
{
	SingleSlotOccuppeElement *NewElem = newSlotOccupeElement(value);//On fait un nouveau maillon qu'on ajoutera à la fin de la liste
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

int ChargerSlotOccupeList(SlotOccuppeList *rep, char nom_fichier[])
{
	FILE *fic_rep;					/* le fichier */
	errno_t err;
	int num_rec = 0;						/* index sur enregistrements */
	int long_max_rec = sizeof(SlotOccuppe);
	char buffer[sizeof(SlotOccuppe) + 1];
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
				SlotOccuppe ptr = { -1, -1, -1, -1 }; //on crée un élément de type contact dans lequel on va enregistrer les chaines de caractères du fichier
				char mot[10];
				if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_PICKUP + 1, SEPARATEUR) == OK1) //On vérifie que e nom est OK1
				{
					idx++;							/* on saute le separateur */
					ptr.ID_PickUP = atoi(mot);
					if (lire_champ_suivantPickUP(buffer, &idx, mot, MAX_ID_SLOT + 1, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
					{
						idx++;
						ptr.ID_Slot = atoi(mot);
						if (lire_champ_suivantPickUP(buffer, &idx, mot, 21, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
						{
							idx++;
							ptr.Tel = atoi(mot);
							if (lire_champ_suivantPickUP(buffer, &idx, mot, 5, SEPARATEUR) == OK1) //On vérifie que le prnom est OK1
							{
								idx++;
								ptr.CodeRetrait = atoi(mot);

								num_rec++;
							}
						}
					}
				}
				appendSlotOccupeElement(rep, ptr, num_rec--);
			}

		}
		fclose(fic_rep);
		return(OK1);
	}


} /* fin charger */

int SauvegarderSlotOccupe(SlotOccuppeList *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingleSlotOccuppeElement *myPtr = rep->head;
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
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d\n", myPtr->data.ID_PickUP, SEPARATEUR, myPtr->data.ID_Slot, SEPARATEUR, myPtr->data.Tel, SEPARATEUR, myPtr->data.CodeRetrait);
			myPtr = myPtr->next;
			//On va vers l'élément suivant. On n'aura pas de problème lié à un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistré tous les éléments. On ne cherche donc pas à afficher l'élément nb_elts (sui est le nb_elts+1 ème élément du répertoire) qui a une adresse NULL car non définit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien passée
		return(OK1);
	}
}

/*********************************************************************/
/***************** Modification du fichier Station *******************/
/*********************************************************************/

//Fonction qui permet de trouver un slot précis dans le fichier station à partir de son ID de slot (et Id de station)
//On change la disponibilité du slot ayant cet ID
int RechercherSlotInStation(StationFile * slotStation, int ID_pickUP, int ID_slot)
{
	if (slotStation == NULL)
		return (ERROR);
	else
	{
		SingelStationElement * ptr = slotStation->head;
		for (int i = 0; i < slotStation->Size; i++)
		{
			if ((ptr->data.PickupStationID == ID_pickUP) && (ptr->data.SlotID == ID_slot))
			{
				ptr->data.Disponible = 1;
				//Le slot n'est plus disponible désormais -> on la réservé
			}
			ptr = ptr->next;
		}
		return(OK1);
	}
}

int ModificationDisponibilitePickUP(StationFile * slotsStation, ReserveSlotList * slotsAReserver, SlotOccuppeList *Slotsoccupe)
{
	if ((slotsAReserver == NULL) || (slotsStation == NULL))
		return(ERROR);
	else
	{
		SingleReserveSlotElement * ptr = slotsAReserver->head;
		for (int i = 0; i < slotsAReserver->Size; i++)
		{
			RechercherSlotInStation(slotsStation, ptr->data.ID_PickUP, ptr->data.ID_slot);
			if (ptr->data.ID_PickUP == slotsStation->head->data.PickupStationID)
			{
				SlotOccuppe nouveau = { -1,-1,-1,-1 };
				nouveau.ID_PickUP = ptr->data.ID_PickUP;
				nouveau.ID_Slot = ptr->data.ID_slot;
				nouveau.Tel = ptr->data.numero_tel;
				nouveau.CodeRetrait = ptr->data.login;
				appendSlotOccupeElement(Slotsoccupe, nouveau, Slotsoccupe->Size);
			}
			ptr = ptr->next;
		}
		return(OK1);
	}
}

int SauvegarderStation(StationFile *rep, char nom_fichier[])
{
	FILE *fic_rep = NULL;					/* le fichier */
	errno_t err;
	SingelStationElement *myPtr = rep->head;
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
		while (Nb_elts <= (*rep).Size)
			//Tant qu'il y a des slots disponibles pouvant acceuillir des articles, on les enregistre
		{
			fprintf(fic_rep, "%d%c%d%c%d%c%d%c%d%c%d\n", myPtr->data.PickupStationID, SEPARATEUR, myPtr->data.SlotID, SEPARATEUR, myPtr->data.Hauteur, SEPARATEUR, myPtr->data.Largeur, SEPARATEUR, myPtr->data.Profondeur, SEPARATEUR, myPtr->data.Disponible);
			myPtr = myPtr->next;
			//On va vers l'élément suivant. On n'aura pas de problème lié à un pointeur NULL car on ne rentre pas dans la boucle quand on a enregistré tous les éléments. On ne cherche donc pas à afficher l'élément nb_elts (sui est le nb_elts+1 ème élément du répertoire) qui a une adresse NULL car non définit.
			Nb_elts++;
		}
		fclose(fic_rep);
		//On ferme le fichier et on dit que la sauvegarde c'est bien passée
		return(OK1);
	}
}

//On modifie le fichier station à partir du fichier envoyé par le dispatching contenant les slots à réserver dans les pickup stations
//et les logins et mot de passes associés permettant à l'utilisateur de récupérer le colis
int ReservationSlotPickUP(char nomStation[], char slotoccupestation[])
{
	ReserveSlotList * listslotAreserve = newReserveSlotListPickUP();
	ChargerReserveSlotListPickUP(listslotAreserve, "ReservationSlots.txt");
	StationFile * stations = newStationFile();
	ChargerStation(stations, nomStation);
	SlotOccuppeList *slotoccupe = newSlotOccupeP();
	ChargerSlotOccupeList(slotoccupe, slotoccupestation);
	ModificationDisponibilitePickUP(stations, listslotAreserve, slotoccupe);
	SauvegarderStation(stations, nomStation);
	SauvegarderSlotOccupe(slotoccupe, slotoccupestation);
	return(OK1);
}

/****************************************************/
/***** Suppression d'un slots des slots occupés *****/
/****************************************************/

SingleSlotOccuppeElement * GetElementAtSlotOccupe(SlotOccuppeList *MyList, int i, int Size)
{
	int j;
	if ((MyList != NULL) && (i + 1 <= Size) && (i + 1 > 0))
	{
		SingleSlotOccuppeElement *tmp = (*MyList).head;
		for (j = 1; j < i + 1; j++)
		{
			tmp = (*tmp).next;
		}//On enregistre dans tmp l'adresse de l'élément i
		return(tmp);
	}
	return(NULL);
}

int suppInSlotOccupeList(SlotOccuppeList *MyList, int i, int Size)
{
	int j = 0;
	if ((MyList != NULL) && (i < Size) && (i >= 0))
	{
		SingleSlotOccuppeElement *tmp = (*MyList).head, *tmp2;
		j = 1;
		if (i == 0)//On supprime l'élément en début de liste (on est sûr qu'il y a au moins un élément)
		{
			(*MyList).head = tmp->next; //S'il n'y avait qu'un seul élément alors la queue et la tête sont à NULL
			free(tmp);
		}
		if ((i == Size - 1) && (i > 0)) //Dans ce cas, on supprime le dernier élément et on est sûr qu'il y en a au moins deux
		{
			tmp = GetElementAtSlotOccupe(MyList, i - 1, Size);
			free(tmp->next);
			tmp->next = NULL;
		}
		if ((i > 0) && (i < Size - 1))//Dans ce cas, on supprime un élément au milieu de la liste. Or la liste ne peut pas être vide si value > 1 donc il n'y a pas de problème
									  //Il y a bien un antécédent et un suivant à la i-ème position.
		{
			tmp = GetElementAtSlotOccupe(MyList, i - 1, Size);
			//On enregistre dans tmp l'adresse de l'élément i-1
			//L'élément i-1 doit pointer vers l'élément i+1
			tmp2 = (tmp->next->next);
			free((*tmp).next);//On libére la mémoire de l'élément à la position i
			tmp->next = tmp2; //On dit que l'élément i-1 a pour ntécédent l'élément suivant de l'élément suivant de i-1 (soit l'élément i+1)
		}
		MyList->Size = MyList->Size - 1;
		return(Size - 1);
	}
	return(-1);
}

/***************************************************/
/***************************************************/
/******** Retrait de colis (station n°1) ***********/
/***************************************************/
/***************************************************/

//On recherche l'identifiant du slot dans la pickup station dont le login est le n° de téléphone saisi par l'utilisateur et on récupére le code de retrait
int RechercheSlotOccuppeWithTel(SlotOccuppeList *slotoccupes, int Tel, int *CodeARentrer, int * SlotId)
{
	if (slotoccupes == NULL)
		return(ERROR);
	else
	{
		SingleSlotOccuppeElement * ptr = slotoccupes->head;
		int nb_elts = 0;
		int trouver = ERROR;
		while ((trouver == ERROR) && (nb_elts < slotoccupes->Size))
		{
			if (ptr->data.Tel == Tel)
			{
				*CodeARentrer = ptr->data.CodeRetrait;
				*SlotId = ptr->data.ID_Slot;
				return(nb_elts);
			}
			ptr = ptr->next;
			nb_elts++;
		}
		return(ERROR);
	}
}

int LiberationSlot(SlotOccuppeList *slotoccupes, StationFile * stations, int CodeARentrer, int CodeRentrer, int indice, int SlotId)
{
	if ((slotoccupes == NULL) || (stations == NULL))
	{
		return (ERROR);
	}
	else
	{
		if (CodeARentrer == CodeRentrer)
		{
			//Dans ce cas, le colis est récupéré par l'utilisateur et on libère le slot
			//On supprime le slot du fichier des slots occuppés de cette station
			suppInSlotOccupeList(slotoccupes, indice, slotoccupes->Size);
			SingelStationElement * ptr = stations->head;
			int trouver = ERROR;
			while (trouver == ERROR) //Si on lance LiberationSlot il y a bien un slot dont le login est le numéro de telephone rentré précédement
			{
				if (ptr->data.SlotID == SlotId)
				{
					trouver = OK1;
					ptr->data.Disponible = 0;
					//On remet à jour la disponibilité du slot dans le fichier station
				}
				ptr = ptr->next;
			}
			return(OK1);
		}
		return(ERROR);
	}
}

int LiberationDansStation001()
{
	SlotOccuppeList * slotoccupe = newSlotOccupeP();
	ChargerSlotOccupeList(slotoccupe, "SlotOccupe001.txt");
	StationFile * stations = newStationFile();
	ChargerStation(stations, "Station001.txt");
	printf("Entrer votre numero de telephone : \n");
	int tel;
	scanf_s("%d%*c", &tel);
	int CodeRentrer;
	int CodeARentrer = ERROR;
	int SlotID = ERROR;
	printf("Entrer votre Code de Retrait : \n");
	scanf_s("%d%*c", &CodeRentrer);
	int nb_elts = RechercheSlotOccuppeWithTel(slotoccupe, tel, &CodeARentrer, &SlotID);
	LiberationSlot(slotoccupe, stations, CodeARentrer, CodeRentrer, nb_elts, SlotID);
	SauvegarderSlotOccupe(slotoccupe, "SlotOccupe001.txt");
	SauvegarderStation(stations, "Station001.txt");
	return(EXIT_SUCCESS);
}

/*********************************************************************/
/******************** Lancement De La Simulation *********************/
/*********************************************************************/
