# FT_IRC 42 Crusus

on code un serveur irc en c++ lol.

## todo

### sujet

- [ ] prendre deux parametres : `./ircserv <port> <password>`
- [ ] gérer plusieurs clients simultanément sans jamais bloquer
- [ ] s'authentifier
- [x] définir un nickname
- [x] un username
- [x] rejoindre un channel
- [x] envoyer et recevoir des messages privés
- [ ] Tous les messages envoyés par un client dans un channel doivent être transmis à tous les clients ayant rejoint ce channel
- [ ] operators et des utilisateurs basiques
- [ ] implémenter les commandes spécifiques aux operators
- [ ] `ctrl+D` pour envoyer la commande en plusieurs parties

### Commandes

- [ ] INVITE
- [ ] KICK
- [ ] LIST
- [ ] NAMES
- [x] NOTICE
- [ ] PART
- [ ] PING
- [ ] QUIT
- [ ] USER
- [ ] JOIN
- [ ] KILL
- [ ] MODE
- [x] NICK
- [x] OPER
- [x] PASS
- [ ] PRIVMSG
- [ ] TOPIC
- [ ] WHO