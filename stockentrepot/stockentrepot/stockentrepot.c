#include "stockentrepot.h"
#define TAILLE_MAX 128


const char *fichier;
FILE* fichierCommande = NULL;
ArticleStocks stockentrepot[10];
Commande demande[10];
int compterligne(const char * fichierlu) {
	int nombreligne=0;
	char chaine[TAILLE_MAX] = "";
	FILE* fichiertest = NULL;
	fichier = fichierlu;
	fichiertest = fopen(fichier, "r");


	while (fgets(chaine, TAILLE_MAX, fichiertest) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
	{
		nombreligne++;
	}
	fclose(fichiertest); // On ferme le fichier qui a été ouvert
	return(nombreligne);
}
int LireFichier(const char * fichierlu,int taillemaxfichier, ArticleStocks stockentrepot[6])
{

	FILE* fichiertest = NULL;
	fichier = fichierlu;
	fichiertest = fopen(fichier, "r");
	if (fichiertest != NULL)

	{
		for (int i = 0; i < taillemaxfichier; i++)
		{


			fscanf(fichiertest, "%d;%d;%d;%d;%d;%d;", &stockentrepot[i].EntrepotID, &stockentrepot[i].ArticleID, &stockentrepot[i].Quantity, &stockentrepot[i].Hauteur, &stockentrepot[i].Largeur, &stockentrepot[i].Profondeur);

		}
		fclose(fichiertest); // On ferme le fichier qui a été ouvert
		for (int j = 0; j < taillemaxfichier; j++)
		{
			printf("%d %d %d %d %d %d \n", stockentrepot[j].EntrepotID, stockentrepot[j].ArticleID, stockentrepot[j].Quantity, stockentrepot[j].Hauteur, stockentrepot[j].Largeur, stockentrepot[j].Profondeur);


		}
		
	}
	return 0;
}
int main(int argc, char *argv[]){
	int taillefile = compterligne("file.txt");
	int taillestock = compterligne("Stock0.txt");
	//int a = LireFichier("Stock0.txt", taillestock, &stockentrepot[6]);

	fichierCommande = fopen("file.txt", "r");
	if (fichierCommande != NULL)

	{
		for (int i = 0; i <taillefile; i++)
		{


			fscanf(fichierCommande, "%d;%d;%d;%d", &demande[i].ClientID, &demande[i].ArticleID, &demande[i].Quantity, &demande[i].CodeRetrait);

		}
		fclose(fichierCommande); // On ferme le fichierCommande qui a été ouvert
		for (int j = 0; j < taillefile; j++)
		{
			printf("%d %d %d %d  \n", demande[j].ClientID, demande[j].ArticleID, demande[j].Quantity, demande[j].CodeRetrait);


		}
	}

	FILE* fichiertest = NULL;
	fichier = "Stock0.txt";
	fichiertest = fopen(fichier, "r");
	if (fichiertest != NULL)

	{
		for (int i = 0; i < taillestock; i++)
		{


			fscanf(fichiertest, "%d;%d;%d;%d;%d;%d;", &stockentrepot[i].EntrepotID, &stockentrepot[i].ArticleID, &stockentrepot[i].Quantity, &stockentrepot[i].Hauteur, &stockentrepot[i].Largeur, &stockentrepot[i].Profondeur);

		}
		fclose(fichiertest); // On ferme le fichier qui a été ouvert
		for (int j = 0; j < taillestock; j++)
		{
			printf("%d %d %d %d %d %d \n", stockentrepot[j].EntrepotID, stockentrepot[j].ArticleID, stockentrepot[j].Quantity, stockentrepot[j].Hauteur, stockentrepot[j].Largeur, stockentrepot[j].Profondeur);


		}

	}





	int quantitémanquante[6];
	int totalarticlecommande=0;
	int totalarticlemanquant = 0;
	float pourcentagecommanderempli = 0;

	//%de remplisage
	
	for (int indicecommande = 0; indicecommande < taillefile ; indicecommande++)
	{
		totalarticlecommande = demande[indicecommande].Quantity + totalarticlecommande;
		for (int indicestock = 0; indicestock < taillestock; indicestock++)
		{

			if (demande[indicecommande].ArticleID==stockentrepot[indicestock].ArticleID)
			{
				
				if (demande[indicecommande].Quantity>stockentrepot[indicestock].Quantity)
				{

					quantitémanquante[indicecommande] = demande[indicecommande].Quantity - stockentrepot[indicestock].Quantity;
					totalarticlemanquant = quantitémanquante[indicecommande] + totalarticlemanquant;
					stockentrepot[indicestock].Quantity = 0;
					
				}
				else {
					quantitémanquante[indicecommande] =0;
					stockentrepot[indicestock].Quantity = stockentrepot[indicestock].Quantity - demande[indicecommande].Quantity;

				}


			}
		}
	}
	for  (int i = 0; i < taillefile;  i++)
	{
		printf("%d \n", quantitémanquante[i]);
	}
	pourcentagecommanderempli = (1 - ((float)totalarticlemanquant /(float)totalarticlecommande)) * 100;
	printf("%d %d ",totalarticlecommande,totalarticlemanquant);

	printf("%f", pourcentagecommanderempli);



	system("pause");
	return(EXIT_SUCCESS);
	return 0;
}


