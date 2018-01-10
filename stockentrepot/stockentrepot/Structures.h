#include <stdio.h>
#include <stdlib.h>


/*--------------------------------------------------------------------*/
/*                          DONNEES STOCKEES                          */
/*--------------------------------------------------------------------*/

/*La structure qui défini un client (UNE LIGNE) */ 
typedef struct Client {	
	int ID;								//4 chiffres pour l'ID
	char Nom[CLIENT_NOM_MAX];						//Nom de 20 caractère alpha ( +1 pour le \0 de fin de chaine de caractère )
	char Prenom[CLIENT_PRENOM_MAX];					//Prenom de 20 caractère alpha ( +1 pour \0 de fin de chaine de caractère)
	char Adresse[CLIENT_ADRESSE_MAX];					//localisation ( adresse ) du client, 50 caractères ( +1 pour \0 de fin de chainede caractère )
	char Ville[CLIENT_VILLE_MAX];						//La ville du client (+1 pour le \0)
	char CodePostal[CLIENT_CodePostal_MAX];					//Code postal du client 5 chiffres + le \0
	char NumeroTel[CLIENT_TELEPHONE_MAX];					//numéro de téléphone, 10 chiffres dit caractère numérique ( et PAS alphanumérique ) (+1 pour le \0 de fin de chaine de caractère )
	char MotDePasse[CLIENT_MotDePasse_MAX];					//Mot de passe de 4 caractère alphanumérique (+1 pour le \0à de fin de chaine de caractère)
} Client;

/*La structure qui regroupe tous les clients (UN FICHIER) */
typedef struct ClientSeul  //Déclaration d'un élément individuel de la liste
{
	Client client;  
	struct ClientSeul *next;  //On définit un pointeur sur l'élément suivant
}ClientSeul;

typedef struct ListeClient //Déclaration de la liste en elle-même avec une tête de liste, une queue de liste et la taille de la liste
{
	ClientSeul *head;
	int size;
	ClientSeul *tail;
}ListeClient;

typedef struct ClientFile
{
	int NombreDeClient;
	ListeClient *BDDclient;
} ClientFile;




/*La structure qui definit un Article (UNE LIGNE) */
typedef struct Article {
	int ID;								//4 chiffres pour l'ID
	char Nom[ARTICLE_NOM_MAX];						//Nom de l'article de 20 caractère ( +1 pour le \0 de fin de chaine de caractère )
	char Description[ARTICLE_DESCRIPTION_MAX];				//Description de l'article de 20 caractère de long ( +1 pour le \0 de fin de chaine de caractère )
	char Type[ARTICLE_TYPE_MAX];						//Type de l'article de 20 caractère de long
} Article;
/*La structure qui regroupe tous les articles défini (UN FICHIER) */
typedef struct ArticleFile {	
	Article *TableauDesArticles;		//Un pointeur sur un tableau d'article (Necessite un malloc et des realloc)
	int NombreArticles;					//Nombre d'article dans ce fichier d'article
} ArticleFile;




/*La structure qui defini une commande (UNE LIGNE) */
typedef struct Commande {
	int ClientID;						//L'ID du client qui passe la commande
	int ArticleID;						//L'ID de l'article que le client a commander
	int Quantity;						//La quantité de c et article que le client à commander
	int CodeRetrait;					//Le code que le client doit taper afin de retirer son colis dans les pickup Station
} Commande;
/*La structure qui regroupe toutes les commandes (UN FICHIER) */
typedef struct CommandeFile {
	Commande *TableauDesCommandes;		//Un pointeur qui pointe sur un tableau de toutes les commandes
	int NombreDeCommande;				//Le nombre de commandes dans ce fichier de commande
} CommandeFile;




/*strucutre qui défini les stocks d'un article dans un entrepot (UNE LIGNE) */
typedef struct ArticleStocks {
	int EntrepotID;					//ID de l'entrepot (USELESS)
	int ArticleID;					//ID de l'article
	int Quantity;					//Quantité en stock de cet article
	int Hauteur;					//Hauteur de l'article	
	int Largeur;					//Largeur de l'article
	int Profondeur;					//Profondeur de l'article
} ArticleStocks;
/*structure qui defini tous les article stockées dans un entrepot (UN FICHIER) */
typedef struct ArticleStocksFile {
	ArticleStocks *TabArticleStock;	//Un pointeur qui pointe sur un tableau qui liste tous les articles en stocks dans CET entrepot
	int NombreArticle;				//Le nombre d'article DIFFERENT en stock
} ArticleStocksFile;

/*structure qui defini un entrepot (le dossier ou il faut aller voir son stock) (UNE LIGNE) */
typedef struct Entrepot {
	int EntrepotID;					//ID de l'entrepot
	char NomDuDossier[ENTREPOT_NOM_DOSSIER_MAX];			//Nom du dossier dans lequel se trouve le dossier stock de cet entrepot
} Entrepot;
/*structure qui contient tous les emplacements des dossier des entrepots (UN FICHIER) */
typedef struct EntrepotFile {
	Entrepot *TabEntrepot;			//Un poiteur qui pointe sur un tableau listant chaquun des entrepots
	int NombreEntrepot;				//Nombre d'entrepot a disposition
} EntrepotFile;





/*structure qui defini une station (UNE LIGNE) */
typedef struct Station {
	int StationID;		//L'ID de la pickup station
	int SlotID;					//L'ID du slot de cette pickup station
	int Hauteur;				//La hauteur de ce slot
	int Largeur;				//La largeur de ce slot
	int Profondeur;				//La profondeur de ce slot
	int Disponible;				//Si slot disponible mettre a 1, sinon 0
} Station;
/*structure qui regroupe toutes nos stations*/
typedef struct StationFile {
	Station *TableauDesStation;	//pointeur qui pointe vers un tableau qui liste les stations a disposition
	int NombreDeStation;		//Le Nombre de station a disposition
} StationFile;





/*structure achat*/
typedef struct Achat {
	int AchatID;				//L'ID de l'achat... ? Pas compris c'etait pendant la pause y avais plus personne pour me repondre... :(
} Achat;





/*structure dispatching*/
typedef struct Dispatching {
	int DispatchingID;			//L'id du dispatching... Pareil :'(
} Dispatching;


/*--------------------------------------------------------------------*/
/*                          DONNEES ENVOYEES                          */
/*--------------------------------------------------------------------*/
















