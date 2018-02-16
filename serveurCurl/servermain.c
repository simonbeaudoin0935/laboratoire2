
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pour recuperer les descriptions d'erreur
#include <errno.h>

// Multiprocessing
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

// Sockets UNIX
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

// Signaux UNIX
#include <signal.h>


// Structures et fonctions de communication
#include "communications.h"
// Fonction de téléchargement utilisant cURL
#include "telechargeur.h"
// Structures et fonctions stockant et traitant les requêtes en cours
#include "requete.h"
// Fonctions de la boucle principale
#include "actions.h"

// Chaînes de caractères représentant chaque statut (utile pour l'affichage)
const char* statusDesc[] = {"Inoccupe", "Connexion client etablie", "En cours de telechargement", "Pret a envoyer"};

// Nombre maximal de connexions simultanés
#define MAX_CONNEXIONS 10
// Contient les requetes en cours de traitement
struct requete reqList[MAX_CONNEXIONS];


void gereSignal(int signo) {
    // Fonction affichant des statistiques sur les tâches en cours
    // lorsque SIGUSR2 (et _seulement_ SIGUSR2) est reçu
    // TODO

}



int main(int argc, char* argv[]){
    // Chemin du socket UNIX
    // Linux ne supporte pas un chemin de plus de 108 octets (voir man 7 unix)
    char path[108] = "/tmp/unixsocket";
    if(argc > 1)        // On peut également le passer en paramètre
        strncpy(path, argv[1], sizeof(path));
    unlink(path);       // Au cas ou le fichier liant le socket UNIX existerait deja

    // On initialise la liste des requêtes
    memset(&reqList, 0, sizeof(reqList));

    // TODO
    // Implémentez ici le code permettant d'attacher la fonction "gereSignal" au signal SIGUSR2


    // TODO
    // Création et initialisation du socket (il y a 5 étapes)
    // 1) Créez une struct de type sockaddr_un et initialisez-la à 0.
    //      Puis, désignez le socket comme étant de type AF_UNIX
    //      Finalement, copiez le chemin vers le socket UNIX dans le bon attribut de la structure
    //      Voyez man unix(7) pour plus de détails sur cette structure

    // TODO
    // 2) Créez le socket en utilisant la fonction socket().
    //      Vérifiez si sa création a été effectuée avec succès, sinon quittez le processus en affichant l'erreur

    // TODO
    // 3) Utilisez fcntl() pour mettre le socket en mode non-bloquant
    //      Vérifiez si l'opération a été effectuée avec succès, sinon quittez le processus en affichant l'erreur
    //      Voyez man fcntl pour plus de détails sur le champ à modifier

    // TODO
    // 4) Faites un bind sur le socket
    //      Vérifiez si l'opération a été effectuée avec succès, sinon quittez le processus en affichant l'erreur
    //      Voyez man bind(2) pour plus de détails sur cette opération

    // TODO
    // 5) Mettez le socket en mode écoute (listen), en acceptant un maximum de MAX_CONNEXIONS en attente
    //      Vérifiez si l'opération a été effectuée avec succès, sinon quittez le processus en affichant l'erreur
    //      Voyez man listen pour plus de détails sur cette opération


    // Initialisation du socket UNIX terminée!

    // Boucle principale du programme
    int tacheRealisee;
    while(1){
        // On vérifie si de nouveaux clients attendent pour se connecter
        tacheRealisee = verifierNouvelleConnexion(reqList, MAX_CONNEXIONS, sock);

        // On teste si un client vient de nous envoyer une requête
        // Si oui, on la traite
        tacheRealisee += traiterConnexions(reqList, MAX_CONNEXIONS);

        // On teste si un de nos processus enfants a terminé son téléchargement
        // Dans ce cas, on traite le résultat
        tacheRealisee += traiterTelechargements(reqList, MAX_CONNEXIONS);

        // Si on a des données à envoyer au client, on le fait
        tacheRealisee += envoyerReponses(reqList, MAX_CONNEXIONS);

        // Si on n'a pas attendu dans un select ou effectué une tâche, on ajoute
        // un petit delai ici pour éviter d'utiliser 100% du CPU inutilement
        if(tacheRealisee == 0)
            usleep(SLEEP_TIME);
    }

    return 0;
}
