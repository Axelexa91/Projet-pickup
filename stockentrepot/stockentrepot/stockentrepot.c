#include "stockentrepot.h"
#define TAILLE_MAX 128



int compterligne(const char * fichierlu) {
	int nombreligne=0;
	char chaine[TAILLE_MAX] = "";
	FILE* fichiertest = NULL;
	fichiertest = fopen(fichierlu, "r");


	while (fgets(chaine, TAILLE_MAX, fichiertest) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
	{
		nombreligne++;
	}
	fclose(fichiertest); // On ferme le fichier qui a été ouvert
	return(nombreligne);
}
float pourcentagecommandefait(Commande *demande, ArticleStocks *stockentrepot,int indiceentrepot,int (*quantitémanquante)[6]) {
	int taillefile = compterligne("file.txt");
	int taillestock = compterligne("Stock0.txt");
	int totalarticlecommande = 0;
	int totalarticlemanquant = 0;
	float pourcentagecommanderempli = 0;

	//%de remplisage

	for (int indicecommande = 0; indicecommande < taillefile; indicecommande++)
	{
		totalarticlecommande = demande[indicecommande].Quantity + totalarticlecommande;
		for (int indicestock = indiceentrepot; indicestock <indiceentrepot+6; indicestock++)
		{
			
			
			if (demande[indicecommande].ArticleID == stockentrepot[indicestock].ArticleID)
			{
				

				if (demande[indicecommande].Quantity > stockentrepot[indicestock].Quantity)
				{
					

					(*quantitémanquante)[indicecommande] = demande[indicecommande].Quantity - stockentrepot[indicestock].Quantity;
					totalarticlemanquant = (*quantitémanquante)[indicecommande] + totalarticlemanquant;
					stockentrepot[indicestock].Quantity = 0;

				}
				else {
					
					(*quantitémanquante)[indicecommande] = 0;
					stockentrepot[indicestock].Quantity = stockentrepot[indicestock].Quantity - demande[indicecommande].Quantity;

				}


			}
		}
	}
	pourcentagecommanderempli = (1 - ((float)totalarticlemanquant / (float)totalarticlecommande)) ;
	printf("%d %d %d",totalarticlemanquant,totalarticlecommande,indiceentrepot );

	return(pourcentagecommanderempli);
}
int main(int argc, char *argv[]) {
	
	const char *fichier;
	
	FILE* fichierecrit = NULL;
	ArticleStocks stockentrepot[18];
	ArticleStocks stockentrepot1[18];
	
	int nombrelignestock = compterligne("Stock0.txt");
	
	FILE* fichiertest = NULL;
	fichier = "Stock0.txt";
	fichiertest = fopen(fichier, "r");

	//début oubverture fichier ici............................................................................

	FILE* fichierCommande = NULL;
	Commande demande[10];
	int nombrelignecommande = compterligne("file.txt");
	fichierCommande = fopen("file.txt", "r");
	if (fichierCommande != NULL)

	{
		for (int i = 0; i < nombrelignecommande; i++)
		{
			fscanf(fichierCommande, "%d;%d;%d;%d;", &demande[i].ClientID, &demande[i].ArticleID, &demande[i].Quantity, &demande[i].CodeRetrait);

		}
		fclose(fichierCommande); // On ferme le fichierCommande qui a été ouvert
		for (int j = 0; j < nombrelignecommande; j++)
		{
			//printf("%d %d %d %d  \n", demande[j].ClientID, demande[j].ArticleID, demande[j].Quantity, demande[j].CodeRetrait);
		}
	}
	//fin ouverture fichier ici .............................................................................................s
	
	if (fichiertest != NULL)

	{
		for (int i = 0; i < nombrelignestock; i++)
		{


			fscanf(fichiertest, "%d;%d;%d;%d;%d;%d;", &stockentrepot[i].EntrepotID, &stockentrepot[i].ArticleID, &stockentrepot[i].Quantity, &stockentrepot[i].Hauteur, &stockentrepot[i].Largeur, &stockentrepot[i].Profondeur);

		}
		fclose(fichiertest); // On ferme le fichier qui a été ouvert
		for (int j = 0; j < nombrelignestock; j++)
		{
			//printf(" %d %d %d %d %d %d \n", stockentrepot[j].EntrepotID, stockentrepot[j].ArticleID, stockentrepot[j].Quantity, stockentrepot[j].Hauteur, stockentrepot[j].Largeur, stockentrepot[j].Profondeur);


		}

	}
	for (int i = 0; i < nombrelignestock; i++)
	{

		stockentrepot1[i].EntrepotID = stockentrepot[i].EntrepotID;
		stockentrepot1[i].ArticleID = stockentrepot[i].ArticleID;
		stockentrepot1[i].Quantity = stockentrepot[i].Quantity;
		stockentrepot1[i].Hauteur = stockentrepot[i].Hauteur;
		stockentrepot1[i].Largeur = stockentrepot[i].Largeur;
		stockentrepot1[i].Profondeur = stockentrepot[i].Profondeur;

	}
	// pourcentiration
	int nombreentrepot = 3;
	float tabpourcentage[6] = { 0 };
	float max = 0;
	int tabgrand[18] = { 0 };
	int iterate = 0;
	int etat = 0;
	float maxt = 0;
	float pourcentageA=0;
	float pourcentagerestant = 1;
	while (etat==0){
		int entrepotactuel = -1;
		int quantitémanquante[6] = { 0 };
		iterate++;
		for (int indice1 = 0; indice1 < nombreentrepot; indice1++)
		{
			pourcentageA= pourcentagecommandefait(demande, stockentrepot, 6 * indice1, &quantitémanquante);		
			tabpourcentage[indice1] = pourcentageA;
			if (tabpourcentage[indice1]>max)
			{
				max = tabpourcentage[indice1];
				entrepotactuel = indice1;
			}
			printf("pourcentage %f ", pourcentageA);
			printf("indice1 %d \n", indice1);
		}

		for (int numeroentre = 0; numeroentre < 3; numeroentre++)
		{
			for (int i = numeroentre * 6; i < (numeroentre + 1) * 6; i++)
			{

					stockentrepot[i].EntrepotID = stockentrepot1[i].EntrepotID;
					stockentrepot[i].ArticleID = stockentrepot1[i].ArticleID;
					stockentrepot[i].Quantity = stockentrepot1[i].Quantity;
					stockentrepot[i].Hauteur = stockentrepot1[i].Hauteur;
					stockentrepot[i].Largeur = stockentrepot1[i].Largeur;
					stockentrepot[i].Profondeur = stockentrepot1[i].Profondeur;

			}
		}
		pourcentageA = pourcentagecommandefait(demande, stockentrepot, 6 * entrepotactuel, &quantitémanquante);
		for (int i = 0; i < compterligne("Stock0.txt"); i++)
		{

			stockentrepot1[i].EntrepotID = stockentrepot[i].EntrepotID;
			stockentrepot1[i].ArticleID = stockentrepot[i].ArticleID;
			stockentrepot1[i].Quantity = stockentrepot[i].Quantity;
			stockentrepot1[i].Hauteur = stockentrepot[i].Hauteur;
			stockentrepot1[i].Largeur = stockentrepot[i].Largeur;
			stockentrepot1[i].Profondeur = stockentrepot[i].Profondeur;

		}
		
			for (int i = 0; i < nombrelignecommande; i++)
		{
			printf("quantité manquante %d \n", quantitémanquante[i]);
			demande[i].Quantity=quantitémanquante[i];

		}
		
		float ajout = max*pourcentagerestant;
		maxt = ajout+ maxt;
		pourcentagerestant = 1 - maxt;
		printf("pourcentagerestant %f \n",pourcentagerestant);
		printf("ajout %f \n", ajout);
		printf("max %f \n", max);
		printf("maxt %f \n", maxt);
		
		
		if (entrepotactuel==-1)
		{
			etat = 1;
			printf("c'est la merde");
		}
		else if(max*100==100)
		{
			etat = 1;
			printf("on est bon mon chou");
		}
		else if (iterate == 5) { etat = 1;
		printf("stop c'est un for");
		}
		max = 0;
	}

	
	fichierecrit = fopen("yolo.txt","w");
	for (int i = 0; i < compterligne("Stock0.txt"); i++) { //on ecrit chaque ligne jusqu'a avoir atteint le nombre de ligne du fichier
		fprintf(fichierecrit, "%d;%d;%d;%d;%d;%d;\n", stockentrepot[i].EntrepotID, stockentrepot[i].ArticleID, stockentrepot[i].Quantity, stockentrepot[i].Hauteur, stockentrepot[i].Largeur, stockentrepot[i].Profondeur);
	}
	fclose(fichierecrit); //FERMETURE

	system("pause");
	return(EXIT_SUCCESS);
	return 0;
}


