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
float pourcentagecommandefait(Commande *demande, ArticleStocks *stockentrepot,int indiceentrepot) {
	int taillefile = compterligne("file.txt");
	int taillestock = compterligne("Stock0.txt");
	int quantitémanquante[6];
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

					quantitémanquante[indicecommande] = demande[indicecommande].Quantity - stockentrepot[indicestock].Quantity;
					totalarticlemanquant = quantitémanquante[indicecommande] + totalarticlemanquant;
					stockentrepot[indicestock].Quantity = 0;

				}
				else {
					quantitémanquante[indicecommande] = 0;
					stockentrepot[indicestock].Quantity = stockentrepot[indicestock].Quantity - demande[indicecommande].Quantity;

				}


			}
		}
	}
	pourcentagecommanderempli = (1 - ((float)totalarticlemanquant / (float)totalarticlecommande)) * 100;

	return(pourcentagecommanderempli);
}
int main(int argc, char *argv[]) {
	
	const char *fichier;
	FILE* fichierCommande = NULL;
	ArticleStocks stockentrepot[10];
	Commande demande[10];
	int nombrelignecommande = compterligne("file.txt");
	int nombrelignestock = compterligne("Stock0.txt");
	fichierCommande = fopen("file.txt", "r");
	FILE* fichiertest = NULL;
	fichier = "Stock0.txt";
	fichiertest = fopen(fichier, "r");
	if (fichierCommande != NULL)

	{
		for (int i = 0; i < nombrelignecommande; i++)
		{
			fscanf(fichierCommande, "%d;%d;%d;%d", &demande[i].ClientID, &demande[i].ArticleID, &demande[i].Quantity, &demande[i].CodeRetrait);

		}
		fclose(fichierCommande); // On ferme le fichierCommande qui a été ouvert
		for (int j = 0; j < nombrelignecommande; j++)
		{
			printf("%d %d %d %d  \n", demande[j].ClientID, demande[j].ArticleID, demande[j].Quantity, demande[j].CodeRetrait);
		}
	}

	
	if (fichiertest != NULL)

	{
		for (int i = 0; i < nombrelignestock; i++)
		{


			fscanf(fichiertest, "%d;%d;%d;%d;%d;%d;", &stockentrepot[i].EntrepotID, &stockentrepot[i].ArticleID, &stockentrepot[i].Quantity, &stockentrepot[i].Hauteur, &stockentrepot[i].Largeur, &stockentrepot[i].Profondeur);

		}
		fclose(fichiertest); // On ferme le fichier qui a été ouvert
		for (int j = 0; j < nombrelignestock; j++)
		{
			printf("%d %d %d %d %d %d \n", stockentrepot[j].EntrepotID, stockentrepot[j].ArticleID, stockentrepot[j].Quantity, stockentrepot[j].Hauteur, stockentrepot[j].Largeur, stockentrepot[j].Profondeur);


		}

	}
	
	// pourcentiration
	int nombreentrepot = 2;
	for  (int indice1 = 0;indice1 < nombreentrepot; indice1++)
	{
		float pourcentageA = pourcentagecommandefait(demande, stockentrepot,6*indice1 );
		printf("%f \n", pourcentageA);
	}
	
	


	



	system("pause");
	return(EXIT_SUCCESS);
	return 0;
}


