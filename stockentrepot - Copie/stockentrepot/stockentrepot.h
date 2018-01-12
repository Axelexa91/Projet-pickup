#pragma once
#include "Structures.h"
#include <string.h>
#include <time.h>
int compterligne(const char * fichierlu);
float pourcentagecommandefait(Commande *demande, ArticleStocks *stockentrepot, int indiceentrepot, int (*quantitémanquante)[6], const char *fichier, const char *comandealias);
int fonction(const char *fichier, const char *comandealias, const char *fichierenvoitalias);


int Capacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite);
int RecCapacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite);

ListArticle * newListArticlePickUP();

SingelArticleDansSlotElement* newArticleDansSlotPickUP(ArticleDansSlot value);

int appendArticleDansSlotPickUP(ListArticle *MyList, ArticleDansSlot value, int Size);

int lire_champ_suivantPickUP(char *ligne, int *idx, char *champ, int taille_champ, char separateur);

int ChargerCommandePickUP(ListArticle *rep, char nom_fichier[]);

StationFile * newStationFile();

SingelStationElement * newSingelStationElement(Station value);

int appendStationElement(StationFile *MyList, Station value, int Size);

int ChargerStation(StationFile *rep, char nom_fichier[]);

SlotDispoFile * newSlotDispoFilePickUP();

SingelSlotDispoElement * newSingelSlotDispoElementPickUP(SlotDispo value);

int appendSlotDispoElementPickUP(SlotDispoFile *MyList, SlotDispo value, int Size);

int ParcoursStation(ListArticle * art, StationFile * sta, SlotDispoFile * disp);

int SauvegarderDisponibilite(SlotDispoFile *rep, char nom_fichier[]);

int RetourDisponibiliteSlot(char FichierCommande[], char FichierStation[] /*Nom du fichier donné lors de l'appel par DataCenter*/, char FichierRetour[]);

ReserveSlotList * newReserveSlotListPickUP();

SingleReserveSlotElement* newReserveSlotElementPickUP(ReserveSlot value);

int appendReserveSlotElementPickUP(ReserveSlotList *MyList, ReserveSlot value, int Size);

int ChargerReserveSlotListPickUP(ReserveSlotList *rep, char nom_fichier[]);

SlotOccuppeList * newSlotOccupeP();

SingleSlotOccuppeElement* newSlotOccupeElement(SlotOccuppe value);

int appendSlotOccupeElement(SlotOccuppeList *MyList, SlotOccuppe value, int Size);

int ChargerSlotOccupeList(SlotOccuppeList *rep, char nom_fichier[]);

int SauvegarderSlotOccupe(SlotOccuppeList *rep, char nom_fichier[]);

int RechercherSlotInStation(StationFile * slotStation, int ID_pickUP, int ID_slot);

int ModificationDisponibilitePickUP(StationFile * slotsStation, ReserveSlotList * slotsAReserver, SlotOccuppeList *Slotsoccupe);

int SauvegarderStation(StationFile *rep, char nom_fichier[]);

int ReservationSlotPickUP(char nomStation[], char slotoccupestation[]);

SingleSlotOccuppeElement * GetElementAtSlotOccupe(SlotOccuppeList *MyList, int i, int Size);

int suppInSlotOccupeList(SlotOccuppeList *MyList, int i, int Size);

int RechercheSlotOccuppeWithTel(SlotOccuppeList *slotoccupes, int Tel, int *CodeARentrer, int * SlotId);

int LiberationSlot(SlotOccuppeList *slotoccupes, StationFile * stations, int CodeARentrer, int CodeRentrer, int indice, int SlotId);

int LiberationDansStation001();

int ResultatCapacite(char CommandeRegion[], char Station1Region[], char Station2Region[], char Dispo1Region[], char Dispo2Region[]);

/********************************************/
/********************************************/

int Capacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite);
int RecCapacite(int P_article, int L_article, int H_article, int P_boite, int L_boite, int H_boite);

ListArticle * newListArticlePickUP();

SingelArticleDansSlotElement* newArticleDansSlotPickUP(ArticleDansSlot value);

int appendArticleDansSlotPickUP(ListArticle *MyList, ArticleDansSlot value, int Size);

int lire_champ_suivantPickUP(char *ligne, int *idx, char *champ, int taille_champ, char separateur);

int ChargerCommandePickUP(ListArticle *rep, char nom_fichier[]);

StationFile * newStationFile();

SingelStationElement * newSingelStationElement(Station value);

int appendStationElement(StationFile *MyList, Station value, int Size);

int ChargerStation(StationFile *rep, char nom_fichier[]);

SlotDispoFile * newSlotDispoFilePickUP();

SingelSlotDispoElement * newSingelSlotDispoElementPickUP(SlotDispo value);

int appendSlotDispoElementPickUP(SlotDispoFile *MyList, SlotDispo value, int Size);

int ParcoursStation(ListArticle * art, StationFile * sta, SlotDispoFile * disp);

int SauvegarderDisponibilite(SlotDispoFile *rep, char nom_fichier[]);

int RetourDisponibiliteSlot(char FichierCommande[], char FichierStation[] /*Nom du fichier donné lors de l'appel par DataCenter*/, char FichierRetour[]);

ReserveSlotList * newReserveSlotListPickUP();

SingleReserveSlotElement* newReserveSlotElementPickUP(ReserveSlot value);

int appendReserveSlotElementPickUP(ReserveSlotList *MyList, ReserveSlot value, int Size);

int ChargerReserveSlotListPickUP(ReserveSlotList *rep, char nom_fichier[]);

SlotOccuppeList * newSlotOccupeP();

SingleSlotOccuppeElement* newSlotOccupeElement(SlotOccuppe value);

int appendSlotOccupeElement(SlotOccuppeList *MyList, SlotOccuppe value, int Size);

int ChargerSlotOccupeList(SlotOccuppeList *rep, char nom_fichier[]);

int SauvegarderSlotOccupe(SlotOccuppeList *rep, char nom_fichier[]);

int RechercherSlotInStation(StationFile * slotStation, int ID_pickUP, int ID_slot);

int ModificationDisponibilitePickUP(StationFile * slotsStation, ReserveSlotList * slotsAReserver, SlotOccuppeList *Slotsoccupe);

int SauvegarderStation(StationFile *rep, char nom_fichier[]);

int ReservationSlotPickUP(char nomStation[], char slotoccupestation[]);

SingleSlotOccuppeElement * GetElementAtSlotOccupe(SlotOccuppeList *MyList, int i, int Size);

int suppInSlotOccupeList(SlotOccuppeList *MyList, int i, int Size);

int RechercheSlotOccuppeWithTel(SlotOccuppeList *slotoccupes, int Tel, int *CodeARentrer, int * SlotId);

int LiberationSlot(SlotOccuppeList *slotoccupes, StationFile * stations, int CodeARentrer, int CodeRentrer, int indice, int SlotId);

int LiberationDansStation001();

int DispatchingPickUP(char EnsembleSlotsDisposParRegion[], char Ventes[], char ALivrer[], char Distances[], char Clients[] /*seul fichiers qui ne dépend pas de la région*/, char Commande[], char ReservationSlots[]);

int ChargerVentes(VentesFile *rep, char nom_fichier[]);

int SauvegarderVentesNonTraitee(VentesFile *rep, char nom_fichier[]);