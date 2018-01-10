#include "stockentrepot.h"
#define TAILLE_MAX 128


const char *fichier;
FILE* fichierCommande = NULL;
ArticleStocks primo[6];
Commande victoria[10];
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
int LireFichier(const char * fichierlu,int taillemaxfichier, ArticleStocks primo[6])
{

	FILE* fichiertest = NULL;
	fichier = fichierlu;
	fichiertest = fopen(fichier, "r");
	if (fichiertest != NULL)

	{
		for (int i = 0; i < taillemaxfichier; i++)
		{


			fscanf(fichiertest, "%d;%d;%d;%d;%d;%d;", &primo[i].EntrepotID, &primo[i].ArticleID, &primo[i].Quantity, &primo[i].Hauteur, &primo[i].Largeur, &primo[i].Profondeur);

		}
		fclose(fichiertest); // On ferme le fichier qui a été ouvert
		for (int j = 0; j < taillemaxfichier; j++)
		{
			printf("%d %d %d %d %d %d \n", primo[j].EntrepotID, primo[j].ArticleID, primo[j].Quantity, primo[j].Hauteur, primo[j].Largeur, primo[j].Profondeur);


		}
		
	}
	return 0;
}
int main(int argc, char *argv[]){
	int b = compterligne("file.txt");
	int a = LireFichier("Stock0.txt", b, &primo[6]);

	printf("%d \n", b);
	fichierCommande = fopen("file.txt", "r");
	if (fichierCommande != NULL)

	{
		for (int i = 0; i <b; i++)
		{


			fscanf(fichierCommande, "%d;%d;%d;%d", &victoria[i].ClientID, &victoria[i].ArticleID, &victoria[i].Quantity, &victoria[i].CodeRetrait);

		}
		fclose(fichierCommande); // On ferme le fichierCommande qui a été ouvert
		for (int j = 0; j < b; j++)
		{
			printf("%d %d %d %d  \n", victoria[j].ClientID, victoria[j].ArticleID, victoria[j].Quantity, victoria[j].CodeRetrait);


		}
	}
	
	//%de remplisage
	/*
	for (int k = 0; k < b; k++)
	{
		while (true)
		{

		}
	}*/
















	system("pause");
	return(EXIT_SUCCESS);
	return 0;
}


