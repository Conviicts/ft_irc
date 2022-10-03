# FT_IRC *Internet Relay Chat* 💬
On code un serveur irc en c++ lol.

## INTRODUCTION 🤓
L’objectif est de reproduire le fonctionnement d’un serveur IRC en C++ en utilisant un vrai client IRC.

## Sommaire
1. [Le sujet](https://github.com/Conviicts/irc2#1-le-sujet-)
2. [Qu'est-ce qu'un serveur?](https://github.com/Conviicts/irc2#2-quest-ce-quun-serveur)
3. [Ports & protocoles](https://github.com/Conviicts/irc2#3-ports--protocoles)
4. [Le protocole TCP](https://github.com/Conviicts/irc2#4-le-protocole-tcp-transmission-control-protocol)
5. [Usefull links](https://github.com/Conviicts/irc2#usefull-links-%EF%B8%8F)

## todo

### sujet

- [x] prendre deux parametres : `./ircserv <port> <password>`
- [x] gérer plusieurs clients simultanément sans jamais bloquer
- [x] s'authentifier
- [x] définir un nickname
- [x] un username
- [x] rejoindre un channel
- [x] envoyer et recevoir des messages privés
- [ ] Tous les messages envoyés par un client dans un channel doivent être transmis à tous les clients ayant rejoint ce channel
- [ ] operators et des utilisateurs basiques
- [ ] implémenter les commandes spécifiques aux operators
- [ ] `ctrl+D` pour envoyer la commande en plusieurs parties

### Commandes

|      | commande | arguements | description |
| :--- | :------- | :--------- | :---------- |
| [X]  | NICK     | `<pseudonyme>` | donner un pseudonyme à un utilisateur, ou pour changer le pseudonyme précédent |
| [X]  | PASS     | `<mot de passe>` | définir le mot de passe de connexion |
| [x]  | USER     | `<nom d'utilisateur>` `<hôte>` `<nom de serveur>` `<nom réel>` | spécifier le nom d'utilisateur, le nom d'hôte, le nom de serveur, et le véritable nom d'un nouvel utilisateur |
| [X]  | QUIT     | `[<Message de départ>]` (optionnel) | quitter le serveur, le message par defaut est le pseudonyme |
| [X]  | JOIN     | `<chanel>{,<chanel>}` `[<clé>{,<clé>}]` | rejoindre un chanel, avec un mot de passe ou non |
| [ ]  | OPER     | `<utilisateur>` `<mot de passe>` | utilisé par un utilisateur normal pour obtenir le privilège d'opérateur. |
| [X]  | PRIVMSG  | ` <destinataire>{,<destinataire>}` ` <texte à envoyer >` |  envoyer un message privé entre des utilisateurs. |
| [ ]  | NOTICE   | `<pseudonyme>` `<texte>` | idem que PRIVMSG mais different |
| [ ]  | TOPIC    | `<chanel>` `[<sujet>]` | modifier ou voir le sujet d'un chanel |
| [ ]  | KILL     | `<pseudonyme>` `<commentaire>`| provoquer la fermeture de la connexion client/serveur par le serveur qui gère cette connexion. |
| [ ]  | INVITE   | `<pseudonyme>` `<canal>` | inviter des utilisateurs dans un chanel |
| [ ]  | KICK     | `<canal>` `<utilisateur>` `[<commentaire>]` | retirer par la force un utilisateur d'un canal (PART forcé). |
| [ ]  | LIST     | `[<chanel>{,<chanel>}]` `[<serveur>]` | lister les chanels et leur sujet. Si un chanel est precise seul le statut de ces chanels est affiché. |
| [X]  | NAMES    | `[<chanel>{,<chanel>}]` | obtenir la liste des pseudonymes visibles sur n'importe quel chanel qu'il peut voir, on peut preciser un ou plusieurs chanel |
| [ ]  | PART     | `<chanel>{,< chanel >}` | partir d'un ou plusieurs chanel(s) |
| [X]  | PING     | `<serveur1>` `[<serveur2>]` | tester la présence d'un client actif à l'autre bout de la connexion |
| [ ]  | MODE     | `<chanel>` `{[+|-]|o|p|s|i|t|n|b|v}` `[<limite>]` `[<utilisateur>]` `[<masque de bannissement >]` | permet aussi bien de changer les modes des utilisateurs que ceux des chanels |
| [ ]  | WHO      | `[<nom>]` `[<o>]`| renvoie une liste d'informations qui correspondent au paramètre `<nom>` donné par le client |


### Modes

- [x] `o` operator (user)

- [ ] `o` operator (de channel)
- [ ] `i` sur invitation (channel)
- [ ] `l` limite / max size (channel)
- [ ] `k` cle / mdp pour join le channel (channel)
- [ ] `b` masque de bannissement / users banni pour join un channel (channel)

## 1. Le sujet 🌱

42Cursus -> [Le sujet](/links/subject.pdf)

### Prérequis
* Tu dois gérer **plusieurs clients simultanément** sur ton serveur sans jamais bloquer. 
* Le **forking est interdit**. Toutes les opérations entrées/sorties doivent être non bloquantes. 
* Tu n'a le droit qu’à un seul `poll()` (ou équivalent) pour gérer toutes ces opérations (read, write, mais aussi listen, etc.).
* Tu dois choisir ton **client IRC de référence**. Il doit pouvoir se connecter à ton serveur sans rencontrer d’erreur.
* La communication entre le client et le serveur se fera en `TCP/IP` (v4 ou v6). 
**Fonctionnalités obligatoires :**
* Pouvoir t'authentifier, définir un **nickname**, un **username**, rejoindre un **channel**, envoyer et recevoir des messages privés, avec ton client. 
* Tous les **messages** envoyés par un client dans un channel **doivent être transmis à tous les clients** ayant rejoint ce channel.
* Tu dois avoir des **operators** et des utilisateurs basiques. 
* Tu dois donc **implémenter les commandes** spécifiques aux operators. 

>Comme tu peux utiliser des FD en mode non bloquant, il est possible d’avoir un serveur non bloquant avec read/recv ou write/send tout en n’ayant pas recours à poll() (ou équivalent). Mais cela consommerait des ressources système inutilement. Il est donc interdit d’utiliser read/recv ou write/send avec n’importe quel FD sans utiliser poll() (ou équivalent).

### Les fonctions autorisées :
```
socket, setsockopt, getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, bind, connect, listen, accept, htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa, send, recv, signal, lseek, fstat, fcntl, poll (ou équivalent)
```

## 2. Qu'est-ce qu'un serveur? 

>video pour les nuls : [ckoi un serveur](https://www.youtube.com/watch?v=fstuiXu1QsQ)</br>
>Voir aussi : [Le serveur informatique expliqué en dessin](https://www.youtube.com/watch?v=CIhalbnBgA4)

Imagine un restaurant : un client passe sa commande au serveur, le serveur prend en note la commande, part en cuisine et ramène au client ce qu’il a demandé

Le rôle d’un serveur web est le même que celui du serveur au restaurant.

Un serveur informatique permet de stocker et publier des pages web (html) sur internet / intranet. Le client (généralement le navigateur) fait une demande de page web au serveur qui lui envoie en réponse la page demandée. 

<!-- IMAGE -->
![shema serveur + client](/links/sh1.png)


>Le protocole http permet de communiquer avec le client. 

Le serveur web peut être logiciel ou matériel ou bien une combinaison des deux. au niveau matériel, un serveur web est un ordinateur qui stock les fichiers constituant un site ( pages html css js). au niveau logiciel les composants sont différents selon le type de serveur.

### Serveur static

Envoi les fichiers hébergés tels quel vers le client, il contient un os (ex: windows ou linux) et un serveur (daemon) http (logiciel qui prend en charge les requêtes client-serveur du protocole http ex: apache ou “2is”).

<!-- IMAGE -->
![shema serveur static](/links/sh2.png)

### Serveur dynamique

Met à jour les fichiers hébergés avant de les envoyer au client via le daemon http. inclut en plus d’un os et du daemon http, une base de données (ex: mysql) et un langage de script (ex: php) le rôle de ce dernier est d'interpréter les demandes du client et de les traduire en html.

<!-- IMAGE -->
![shema serveur dynamique](/links/sh3.png)

## 3. Ports & protocoles

>Ports et protocoles : [comprendre l'essentiel en 5 minutes](https://www.youtube.com/watch?v=YSl6bordSh8)

### Comment se passe l'échange de données entre les machines ?

Sur une machine on a plusieurs applications (mail, internet, messagerie, …). 
Avec l’adresse IP on retrouve le serveur qui les héberge mais pas l’application destinataire.
donc on a un numéro de port attribué à chaque application.

<!-- IMAGE -->
![shema communication serveur + client](/links/sh4.png)

### Socket

```
La combinaison adresse ip + numéro de port est appelée socket.
```

<!-- IMAGE -->
![shema adresse socket](/links/sh5.png)

L’adresse IP sert à identifier de façon unique une machine sur le réseau et le port identifie une application à laquelle on veut envoyer le paquet de données.

### Les types de ports

| Ports reconnus / réservés | Ports enregistré    | Ports dynamiques et/ou privés |
| :------------------------ | :------------------ | :---------------------------- |
| de `0` à `1023`           | de `1024` à `49151` | de `49152` à `65535`          |
| En gros, un serveur possède un numéro de port fixe, associé à des services résea spécifiques, configurés par les administrateurs réseau. | Port attribué pour un service spécifique sur demande par une organisation. | Utilisé pour l’allocation dynamique de port éphémère valable lors de la durée d'une connection ou bien pour des service privés ou personnalisés. |

```
Tu entendras plus souvent parler de port reconnus.
```

| `port 80` | `port 21` | `port 25` | `port 53` | `port 63` |
| :-------- | :-------- | :-------- | :-------- | :-------- |
| **HTTP**  | **FTP**   | **SMTP**  | **DNS**   | **DHCP**  |
| c’est par le port 80 qu'un navigateur demande des pages web au serveur | utilisé pour la transmission de fichiers | permet le transfert de mail | fournit un moyen d'associer un nom de domaine a une adresse IP | permet d'attribuer automatiquement une adresse ip et un masque de sous réseau à une machine connectée au réseau |

### Les types de protocoles

>Définition : Pour envoyer des données entre deux machines, 
elles doivent parler le même langage. Ce langage est appelé protocole.

<!-- IMAGE -->
![shema communication protocole](/links/sh6.png)

#### Protocole applicatif

Un protocole applicatif est attribué à chaque port réservé. ce protocole applicatif va définir les règles de communication avec les applications (ex: type de message échangés, syntaxe, règles, …)

#### Protocole de transmission

Les deux principaux protocoles de transmission sont **TCP** (Transmission Control Protocol) et **UDP** (User Datagram Protocol).

| `TCP`                                   | `UDP` |
| :-------------------------------------- | :---- |
| **fiable** de bout en bout              | simple et **rapide** |
| **garantit** la transmission de données | **sans garantie** de contrôle sur la transmission de données |
| utilisé la ou la livraison de paquet est obligatoire (si un paquet est endommagé ou manquant, tcp réclame une copie du paquet) | utilisé lors d'un besoin de réponse rapide ou application à faible latence ou en temps réel comme les flux vidéo pour lesquels la perte de quelques paquets na pas d'impact significatif |
| très utilisé avec `HTTP` `SMTP` et `FTP`     | très utilisé avec `DNS` et `DHCP` |

## 4. Le protocole TCP *(Transmission Control Protocol)*

Imagines un document qui se trouve sur un ordinateur et ce document doit être transmis à un autre ordinateur. Chaque document est organisé sous forme d’une file de données, une file d’octet.
Sur l’ordinateur source (qui contient le document) on trouve un programme qui gère le transfert d’information selon le protocole de contrôle de transfert  (TCP). Sur cet ordinateur (le serveur) les données du document sont coupées en petits paquets (un certains nombre d’octet).

<!-- IMAGE -->
![shema protocole TCP 1](/links/sh7.png)

Les données coupées en petits paquets vont être envoyées séparément les uns des autres sur l’internet. Les paquets voyagent par les meilleurs chemins possibles, ils peuvent suivre des chemins différents selon l’encombrement du réseau, de la panne de telle ou de telle machine.

<!-- IMAGE -->
![shema protocole TCP 2](/links/sh8.png)

Lorsqu'ils arrivent à destination sur l’ordinateur receveur (le client) ils sont réceptionnés par un programme qui connaît lui aussi le protocole TCP et qui est chargé de remettre les données dans le bon ordre. Si tu envoies une image il faut que les pixels soient replacés dans le bon ordre, c’est le rôle du protocole TCP sur l’ordinateur client de réordonner chacun des paquets. Son deuxième rôle est de vérifier que les paquets arrivent tous et qu’aucun ne soit endommagé. si c’est le cas, le client réclame au serveur, par le protocole TCP,  une copie du paquet endommagé ou manquant.

<!-- IMAGE -->
![shema protocole TCP 3](/links/sh9.png)


## USEFULL LINKS ✌️

* video pour les nuls : [ckoi un serveur](https://www.youtube.com/watch?v=fstuiXu1QsQ)
* Voir aussi : [Le serveur informatique expliqué en dessin](https://www.youtube.com/watch?v=CIhalbnBgA4)
* Ports et protocoles : [comprendre l'essentiel en 5 minutes](https://www.youtube.com/watch?v=YSl6bordSh8)
* [video protocole TCP/IP](https://www.youtube.com/watch?v=DI3xmtAKZJA)
* [Comprendre les modèles OSI et TCP/IP](https://www.youtube.com/watch?v=26jazyc7VNk)
* Explications complètes sur [le protocole tcp/ip](https://www.youtube.com/watch?v=_0thnFumSdA)
* la [rfc 1459](http://abcdrfc.free.fr/rfc-vf/rfc1459.html#411)

