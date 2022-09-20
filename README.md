# FT_IRC 42 Crusus

on code un serveur irc en c++ lol.

## todo

### sujet

- [x] prendre deux parametres : `./ircserv <port> <password>`
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

- [x] NICK
- [x] JOIN
- [x] USER
- [x] QUIT
- [x] OPER
- [x] PASS
- [x] PRIVMSG
- [x] NOTICE
- [x] TOPIC
- [x] KILL
- [ ] INVITE
- [ ] KICK
- [ ] LIST
- [ ] NAMES
- [ ] PART
- [ ] PING
- [ ] MODE
- [ ] WHO

### Modes

- [x] `o` operator (user)

- [ ] `o` operator (de channel)
- [ ] `i` sur invitation (channel)
- [ ] `l` limite / max size (channel)
- [ ] `k` cle / mdp pour join le channel (channel)
- [ ] `b` masque de bannissement / users banni pour join un channel (channel)