Cups2freefax
============

Description
-----------

Ce projet **open-source** permet aux **Freenautes** sous **Linux**
(MacOsX ?) d’envoyer des **faxs** sans avoir a ouvrir son navigateur sur
le site de Free via au choix : 
 - une imprimante virtuelle **CUPS**.
 - le menu contextuel de **KDE** ou **Gnome**
 - en CLI ( ligne de commande ) via le sous projet **fax4free**. Pour les 2 premiers de la liste, les interactions avec l’utilisateur de
l’imprimante se passe via des **boites de dialogues graphiques**, voir
captures ci-dessous.*

Captures d’écran
----------------

Exemple graphique avec les boites de dialogues à partir du menu
d’impression de Cups :

![](http://www.sputnick.fr/ftp/downloads/snapshot1262216241.png)

Menu contextuel sous KDE ( clic droit sur un fichier pdf, txt, odt… ) :

![](http://img63.imageshack.us/img63/9335/fax4free.png)

Menu contextuel sous Gnome ( clic droit sur un fichier pdf, txt, odt… )
:
*A noter que ce menu n’apparaît qu’après avoir lancé cups2freefax au
moins une fois via cups.*

![](http://omploader.org/vNGFqbQ)

Après le choix du menu contextuel ou de Cups :

![](http://www.sputnick.fr/ftp/downloads/snapshot1262216245.png)

Envoi effectif du fax :

![](http://pix.toile-libre.org/upload/original/1333509024.png)

Features
--------

*Voici les principales features gérées :*
 - gestion des faxs en CLI ou GUI ( ligne de commande ou en graphique )
de façon automatique (nul besoin d’ouvrir un navigateur)
 - les identifiants de login sont conservés dans la configuration dés la
première utilisation (il transite sur le web (de façon chiffré via SSL)
pour l’interface free uniquement)
 - configuration et répertoire de fax dans /etc/cups2freefax/ qui peut
être outre-passée avec \~/.config/cups2freefax/\*
 - implémentation de toutes les options de la console fax Free ( cacher
le destinataire, option de confirmation par mail ) et plus : 
 récupération des faxs (via le script et en cron journalier) tels que
reçus par le destinataire avec durée paramétrable de 0 à l’infini
 - envoi par menu contextuel pour KDE et/ou Gnome (accessible seulement
après au moins une utilisation de cups2freefax)
 - répertoire téléphonique des numéros de faxs
 - vérification optionnelle des nouvelles versions de cups2freefax
 - configuration de l’imprimante virtuelle automatique (quand vous
voulez imprimer, vous aurez une nouvelle imprimante CUPS2FREEFAX)
 - gestion de l’impression en réseau via export DISPLAY, nécessite de
configurer le serveur X pour cela. (Expérimental)

Ce projet a été testé avec succès sous Linux ( différentes distributions
).

Prérequis packages système
--------------------------

Les pré requis sont **cups**, **zenity**, **gcc** et les module Perl
**WWW::Mechanize** et **Net::SSLeay**.

Pour tout installer sous Debian et dérivés ( ubuntu, knoppix, kanotix,
linspire, mepis, xandros…) en console : (on peux aussi utiliser **synaptic** pour les réfractaires à la ligne de commande)

    sudo apt-get update; sudo apt-get install cups zenity gcc libwww-mechanize-perl libnet-ssleay-perl

Archlinux :

    pacman -Sy cups zenity gcc perl-www-mechanize perl-net-ssleay

Mageia :

    urpmi cups zenity urpmi perl-HTML-Tree perl-WWW-Mechanize perl-Net-SSLeay perl-LWP-Protocol-https

RedHat, Centos Fedora :

    yum install cups zenity gcc perl-HTML-Tree perl-WWW-Mechanize perl-Net-SSLeay perl-LWP-Protocol-https

OpenSuse :

    zypper install cups zenity gcc perl-HTML-Tree perl-WWW-Mechanize perl-Net-SSLeay perl-LWP-Protocol-https

Gentoo :

    emerge net-print/cups gnome-extra/zenity sys-devel/gcc dev-perl/WWW-Mechanize dev-perl/Net-SSLeay

Install
-------

Ouvrir une console en tant que *user simple* ( xterm, konsole,
gnome-terminal… ) dans une session graphique X ( Xorg ).

    cd /tmp
    git clone https://github.com/sputnick-dev/cups2freefax.git
    cd cups2freefax/src
    sudo ./start

Si vous n’utilisez pas sudo concernant cette dernière ligne
d’instructions, tapez :

    su    # taper son mot de passe "en aveugle"
    ./start

Configuration personnelle
-------------------------

( ce n’est normalement utile que pour faire des corrections )

On gère cela via le fichier

    ~/.config/cups2freefax/cups2freefaxrc

et 

    /etc/cups2freefaxrc

On peux y paramétrer les mêmes options que dans la console web FAX de http://free.fr,

    # Fichier d’environnement de cups2freefax sourcé par cups2freefax.bash

    # Login et password de l’interface free
    login=xxxxxxxxx
    password=xxxxxxxx

    # Masquer le numéro appelant : 
    cups2freefax_hide_fax_number=no

    # Recevoir un rapport de transmission par e-mail : 
    cups2freefax_email_confirmation=no

    # Durée en jours de rétention de la copie des faxs envoyés.
    # Pour ne rien récuperer, mettre cups2freefax_faxs_store=0
    cups2freefax_faxs_store=365

    # Support de l’export DISPLAY en réseau
    #cups2freefax_export_display=true

A noter : le fichier de config du $HOME outrepasse **/etc/cups2freefaxrc** si des variables y sont renseignées

Édition manuelle du répertoire des numéros de fax
-------------------------------------------------

Le répertoire peux être mis en commun dans /etc/cups2freefax/repertoire_tel_fax ou bien personnel dans ~/.config/cups2freefax/repertoire_tel_fax

    $ editor ~/.config/cups2freefax/repertoire_tel_fax

Utiliser cette syntaxe :

    0148765442 Pierre-Henry
    0954661277 Alex_3f
    0455669977 FakeLaule

Usage avancé
---------------

```
./start -h
Aide des options de cups2freefax :
$ sudo ./start -f       installer uniquement le script fax4free utilisable en cli
$ sudo ./start -u       desinstallation de cups2freefax et de fax4free
$ sudo ./start -d :n    n est le numéro du display, :0 pour l'installation si non renseigné.
$ sudo ./start -h       la présente aide
```

Troubleshooting
---------------

Si un problème survient, surveiller

    ~/.config/cups2freefax/log/cups2freefax.log,
    /var/log/cups/cups2freefax_log et /var/log/cups/error_log

Vous pouvez aussi faire un **bug report**, une **feature request** ou une **demande de support** sur [ici](https://github.com/sputnick-dev/cups2freefax/issues)

Enjoy ;)

++sputnick;
