#include <stdio.h>
#include <stdlib.h>

#define OK1 1
#define OK 0
#define ERROR -1

#define CLIENT_ID_MAX 9999 
#define CLIENT_NOM_MAX 21
#define CLIENT_PRENOM_MAX 21
#define CLIENT_ADRESSE_MAX 51

#define CLIENT_CodePostal_MAX 6
#define CLIENT_TELEPHONE_MAX 11
#define CLIENT_MotDePasse_MAX 5
#define CLIENTFILE_MAX 1000
#define	ARTICLE_ID_MAX 9999
#define	ARTICLE_NOM_MAX 21
#define	ARTICLE_DESCRIPTION_MAX 21
#define ARTICLE_TYPE_MAX 21
#define ARTICLEFILE_MAX 1000
#define CODE_RETRAIT_MAX 9999
#define QUANTITY_MAX 9999
#define COMMANDEFILE_MAX 1000
#define ENTREPOT_ID_MAX 9999
#define HAUTEUR_MAX 9999 //en cm
#define LARGEUR_MAX 9999
#define PROFONDEUR_MAX 9999
#define ARTICLESRTOCKSFILE_MAX 1000
#define STATION_ID_MAX 255
#define SLOT_ID_MAX 9999
#define ENTREPOT_NOM_DOSSIER_MAX 21
#define ENTREPOTFILE_MAX 1000

#define DISPONIBLE 1
#define INDISPONIBLE 0

#define SEPARATEUR ';'





#define MAX_ID_CLIENT 4


#define CLIENT_VILLE_MAX 51





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

typedef struct EnvoiCommande { //Envoie commande -> Reception commande
	int ArticleID;							//Commandé
	int Quantity;							//necessaire
	char Adresse[CLIENT_ADRESSE_MAX];		//lieu de livraison
	char Ville[CLIENT_VILLE_MAX];			// "" 
	char CodePostal[CLIENT_CodePostal_MAX];	// ""
	char NumeroTel[CLIENT_TELEPHONE_MAX];	// contact
} EnvoiCommande;


typedef struct DemandeArticle { //Dispaching -> recherche dans entrepot
	int ArticleID;	//Demandé
	int Quantity;	//necessaire
} DemandeArticle;


typedef struct ReponseArticle { //recherche dans entrepot -> dispaching
	int EntrepotID; //Sollicité
	int ArticleID;	//Demandé
	int Quantity;	//Disponible
} ReponseArticle;



typedef struct ReservationArticle { //Dispaching -> Recherche dans entrepot
	int ArticleID;	//réservé
	int Quantity;	//réservé	
} ReservationArticle;


typedef struct DemandeSlot {//Dispaching -> gestion station pickup
	int ArticleID;	//A mettre a disposition
	int Quantity;	//A mettre a disposition
	int Hauteur;	//nécessaire du slot
	int Largeur;	//nécessaire du slot
	int profondeur;	//nécessaire du slot
} DemandeSlot;


typedef struct ReponseSlot { //Gestion station pickup -> Dispaching
	int SationID;	//sollicité
	int ArticleID;	//à héberger
	int Quantity;	//capable d'acceuillir
	int SlotID;		//disponible pour cette commande
} ReponseSlot;

/*
typedef struct ReservationSlot { //dispaching -> gestion de la station pickup
	int SlotID;				//A reserver
	int ArticleID;			//A heberger
	int Quantity;			//A heberger
	char NumeroTelClient[CLIENT_TELEPHONE_MAX];	//Login
	char CodeAccesClient[CLIENT_MotDePasse_MAX];	//Password
} ReservationSlot;*/


typedef struct ValidationSlot {//Gestion station pickup -> Dispaching
	int SlotID;		//réservé
	int Disponible; //0 = plus de disponibilité , 1 = Réservé
} ValidationSlot;


typedef struct FinCommande { //Dispaching -> terminal
	char AdressePickup[CLIENT_ADRESSE_MAX];		//
	char VillePickup[CLIENT_VILLE_MAX];		//
	char CodePostalPickup[CLIENT_CodePostal_MAX];	//
	int SlotID;				//
	char CodeAccesSlot[CLIENT_MotDePasse_MAX];		//
} FinCommande;


#include <stdlib.h>
#include <stdio.h>

#define OK1 1

#define SEPARATEUR ';'
#define MAX_IDArticle 4
#define MAX_Dim 3
#define SEPARATEUR ';'
#define MAX_Article 4
#define MAX_ID_PICKUP 3
#define MAX_ID_SLOT 4
#define MAX_STATION_PAR_REGION 20

typedef struct Ventes {
	int ClientID;						//L'ID du client qui passe la commande
	int ArticleID;						//L'ID de l'article que le client a commander
	int Quantity;						//La quantité de c et article que le client à commander
	int CodeRetrait;					//Le code que le client doit taper afin de retirer son colis dans les pickup Station
} Ventes;

/*La structure qui regroupe toutes les commandes sous forme de liste chaînée(UN FICHIER) */

typedef struct SingleVentesElement
{
	Ventes data;
	struct SingleVentesElement * next;
} SingleVentesElement;

typedef struct VentesFile {
	SingleVentesElement * head;
	SingleVentesElement * tail;
	int Size;				//Le nombre de commandes dans ce fichier de commande
} VentesFile;

typedef struct SlotDispo
{
	int ID_PickUP_Station;
	int ID_article;
	int quantite;
	int ID_slot;
} SlotDispo;

typedef struct SingelSlotDispoElement
{
	SlotDispo data;
	struct SingelSlotDispoElement * next;
} SingelSlotDispoElement;

typedef struct SlotDispoFile
{
	SingelSlotDispoElement * head;
	SingelSlotDispoElement * tail;
	int size;
} SlotDispoFile;

typedef struct ArticleDansSlot {
	int IDArticle;
	int quantite;
	int largeur;
	int profondeur;
	int hauteur;
} ArticleDansSlot;

typedef struct SingelArticleDansSlotElement
{
	ArticleDansSlot data;
	struct SingelArticleDansSlotElement * next;
} SingelArticleDansSlotElement;

typedef struct ListArticle
{
	SingelArticleDansSlotElement * head;
	SingelArticleDansSlotElement * tail;
	int Size;
} ListArticle;
//Liste chaînée des articles à mettre dans la station

/****** Client ******/




/*La structure qui regroupe tous les clients (UN FICHIER) */

typedef struct SingleClientElement  //Déclaration d'un élément individuel de la liste
{
	Client data;
	struct SingleClientElement *next;  //On définit un pointeur sur l'élément suivant
}SingleClientElement;

typedef struct ListClient //Déclaration de la liste en elle-même avec une tête de liste, une queue de liste et la taille de la liste
{
	SingleClientElement *head;
	int Size;
	SingleClientElement *tail;
}ListClient;




/***Structure qui va nous donner la station la plus proche***/
typedef struct RecupDistance {
	int ID_Client;
	int ID_Station;
	int Distance;
}RecupDistance;

typedef struct SingleRecupDistanceElement {
	RecupDistance data;
	struct SingleRecupDistanceElement *next;
}SingleRecupDistanceElement;

typedef struct RecupDistanceList {
	SingleRecupDistanceElement *head;
	SingleRecupDistanceElement *tail;
	int Size;
}RecupDistanceList;


/*Structure qui va nous permettre de renvoyer les slots que l'on réserve avec les articles et la quantité qu'on met dedans*/

typedef struct ReserveSlot {
	int ID_PickUP;
	int ID_slot;
	int ID_article;
	int quantite;
	int numero_tel;
	int login;
}ReserveSlot;

typedef struct SingleReserveSlotElement {
	ReserveSlot data;
	struct SingleReserveSlotElement *next;
}SingleReserveSlotElement;

typedef struct ReserveSlotList {
	SingleReserveSlotElement *head;
	SingleReserveSlotElement *tail;
	int Size;
}ReserveSlotList;

//Un seul fichier pour toutes les réservation dans une région région

/*********************************************************/
/*********************************************************/

typedef struct Station {
	int PickupStationID;		//L'ID de la pickup station
	int SlotID;					//L'ID du slot de cette pickup station
	int Hauteur;				//La hauteur de ce slot
	int Largeur;				//La largeur de ce slot
	int Profondeur;				//La profondeur de ce slot
	int Disponible;				//Si slot disponible mettre a 1, sinon 0
} Station;

typedef struct SingelStationElement
{
	Station data;
	struct SingelStationElement * next;
} SingelStationElement;

typedef struct StationFile
{
	SingelStationElement * head;
	SingelStationElement * tail;
	int Size;
} StationFile;

typedef struct SlotOccuppe
{
	int ID_PickUP;
	int ID_Slot;
	int Tel;
	int CodeRetrait;
} SlotOccuppe;

typedef struct SingleSlotOccuppeElement
{
	SlotOccuppe data;
	struct SingleSlotOccuppeElement *next;
}SingleSlotOccuppeElement;

typedef struct SlotOccuppeList {
	SingleSlotOccuppeElement *head;
	SingleSlotOccuppeElement *tail;
	int Size;
}SlotOccuppeList;














