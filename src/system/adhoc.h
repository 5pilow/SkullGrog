/**********************************************
 * This file is a part of GuitarStar by festi *
 *        http://flash.festi.free.fr/         *
 **********************************************/

#ifndef ADHOC_H
#define ADHOC_H

#include <vector>
#include <pspnet_adhocmatching.h>
#include <pspnet_adhoc.h>
#include <pspnet.h>
#include <psputility_netmodules.h>
#include <pspwlan.h>
#include <pspkernel.h>
#include <pspctrl.h>

#define HELLO_DELAY 3000000
#define PING_DELAY 3000000
#define MESSAGE_DELAY 500000
#define BUFFER_HELLODATA_SIZE 1024

namespace adhoc{
	/**
	 * represente une psp
	 * mac: l'adresse mac de la psp
	 * hellodatasize: la taille des données 'hello'
	 * hellodata: les données 'hello'
	 */
	typedef struct PSPelement{
		unsigned char mac[6];
		int hellodatasize;
		char hellodata[BUFFER_HELLODATA_SIZE];
		void* userData;
		int userDataLen;
	} PSPelement;
	
	/**
	 * les états possible du client
	 */
	enum ClientConnectionState{
		CLIENT_STATE_DISCONNECTED, // le client est déconnecté
		CLIENT_STATE_CONNECTING,   // le client est en train de se connecter
		CLIENT_STATE_CONNECTED     // le client est connecté
	};
	
	/**
	 * les états possible du server
	 */
	enum ServerMode{
		SERVER_MODE_AUTO_ACCEPT,      // le serveur accepte toutes les connections
		SERVER_MODE_AUTO_REFUSE,      // le serveur refuse toutes les connections
		SERVER_MODE_MANUAL_ACCEPATION // le serveur accepte les connections manuellement
	};
	
	/**
	 * callback utilisé dans la bibliothèque
	 */
	typedef void (*AdhocDataCallback)(void* pointerData, PSPelement* psp, int datalen, void *data);
	
	/**
	 * initiallise la bibliothèque (a appeler en 1er)
	
	 * @param productID l'identifiant du jeu (une serie de 9 caractères) qui permetera de filtrer les serveurs
	 * @return 0 en cas de succes sinon un code d'erreur
	 */
	int init(char* productID);
	
	/**
	 * represente un client
	 * un client ne peut etre connecté qu'a un seul serveur a la fois
	 */
	namespace client{
		/**
		 * demarre le module client de la librairie
		 *
		 * @param helloDataLength longeur des données 'hello' envoyé au serveur lors de la 1ere connection
		 * @param helloData données 'hello' envoyé au serveur lors de la 1ere connection
		 *
		 * @return 0 en cas de succes sinon un code d'erreur
		 */
		int start(int helloDataLength, void* helloData);
		
		int startVirtual(int helloDataLength, void* helloData);
		
		/**
		 * stope le module client de la librairie
		
		 * @return 0 en cas de succes sinon un code d'erreur
		 */
		int stop();
		
		/**
		 * retourne la liste des serveur disponibles
		 *
		 * @return la liste des serveurs
		 */
		std::vector<PSPelement*> getServerList();
		
		/**
		 * demmande une connection au serveur psp
		 *
		 * @param psp le serveur sur lequel se connecter
		 * @return true en cas de succes sinon false (si le client est deja en train de se connecter a un autre serveur , si le client est deja connecté a un serveur ou si la demmande de connection échoue)
		 */
		bool requestConnection(PSPelement* psp);
		
		/**
		 * annule la demmande de connection a un serveur
		 *
		 * @return true en cas de succes sinon false (si le client est n'est pas en train de se connecter a un serveur ou si la demmande de d'annulation échoue)
		 */
		bool cancelRequestedConnection();
		
		/**
		 * se déconnecte du serveur
		 *
		 * @return true en cas de succes sinon false (si le client n'est pas connecté a un serveur ou si la déconnection échoue)
		 */
		bool disconnect();
		
		/**
		 * envoi des données au serveur
		 *
		 * @param datalen la longeur des données a envoyer
		 * @param data un pointeur vers le bloc de données à envoyer
		 * @return 0 en cas de succes sinon un code d'erreur
		 */
		int send(int datalen, void *data);
		
		/**
		 * renvoi l'etat du client (déconnecté, en connection ou connecté)
		 *
		 * @return l'etat du serveur
		 */
		ClientConnectionState connectionState();
		
		/**
		 * indique le callback a appeler lors de la récéption de message
		 *
		 * @param callback le callback a utiliser
		 */
		void setcallback(AdhocDataCallback callback, void* pointerData);
		
		/**
		 * renvoi le serveur sur lequel le client est connecté (ou en connection)
		 *
		 * @return la psp sur lequel le client est connecté (ou en connection) sinon 0
		 */
		PSPelement* getTargetPSP();
		
		bool isStarted();
	}
	/**
	 * represente un serveur
	 */
	namespace server{
		/**
		 * demarre le module server de la librairie
		 * 
		 * @param helloDataLength longeur des données 'hello' envoyé aux clients lors de la 1ere connection
		 * @param helloData données 'hello' envoyé aux clients lors de la 1ere connection
		 * 
		 * @return 0 en cas de succes sinon un code d'erreur
		 */
		int start(int helloDataLength, void* helloData);
		
		/**
		 * stope le module serveur de la librairie
		 * 
		 * @return 0 en cas de succes sinon un code d'erreur
		 */
		int stop();
		
		/**
		 * retourne les liste des psp attente de validation de la connection (si le serveur est en acceptation manuelle)
		 * 
		 * @return la liste des psp attente de validation de la connection
		 */
		std::vector<PSPelement*> getClientRequestList();
		
		/**
		 * retourne les liste des psp connecté
		 * 
		 * @return la liste des psp connecté
		 */
		std::vector<PSPelement*> getClientList();
		
		/**
		 * accepte la connection d'une psp (si le serveur est en acceptation manuelle)
		
		 * @param psp le client a accepter
		 * @return vrai en cas de succes sinon faux
		 */
		bool acceptConnection(PSPelement* psp);
		
		/**
		 * refuse la connection d'une psp (si le serveur est en acceptation manuelle)
		 * 
		 * @param psp le client a refuser
		 * @return vrai en cas de succes sinon faux
		 */
		bool rejectConnection(PSPelement* psp);
		
		/**
		 * deconnecte une psp du serveur
		 * 
		 * @param psp le client a deconnecter
		 * @return vrai en cas de succes sinon faux
		 */
		bool disconnect(PSPelement* psp);
		
		/**
		 * envoi des données a un client
		 * 
		 * @param psp le client auquel envoyer les données
		 * @param datalen la longeur des données a envoyer
		 * @param data un pointeur vers le bloc de données à envoyer
		 * @return 0 en cas de succes sinon un code d'erreur
		 */
		int send(PSPelement* psp, int datalen, void *data);
		
		/**
		 * indique le callback a appeler lors de la récéption de message
		 * 
		 * @param callback le callback a utiliser
		 */
		void setcallback(AdhocDataCallback callback, void* pointerData);
		
		/**
		 * regle le mode d'acceptation du server
		 * 
		 * @param mode le mode a utiliser
		 */
		void setAcceptationMode(ServerMode mode);
		
		/**
		 * retourne le mode d'acceptation
		 * 
		 * @return le mode d'acceptation
		 */
		ServerMode getAcceptationMode();
		
		bool isStarted();
		
		void setClientConnectingCallback(AdhocDataCallback callback, void* pointerData);
		void setClientConnectedCallback(AdhocDataCallback callback, void* pointerData);
		void setClientDisconnectCallback(AdhocDataCallback callback, void* pointerData);
	}
}

#endif
