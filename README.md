Cups2freefax
============

Description
-----------

Ce projet **open-source** permet aux **Freenautes** sous **Linux**
(MacOsX ?) d’envoyer des **faxs** sans avoir a ouvrir son navigateur sur
le site de Free via au choix : \
 - une imprimante virtuelle **CUPS**.\
 - le menu contextuel de **KDE** ou **Gnome**\
 - en CLI ( ligne de commande ) via le sous projet **fax4free**\
*Pour les 2 premiers de la liste, les interactions avec l’utilisateur de
l’imprimante se passe via des **boites de dialogues graphiques**, voir
captures ci-dessous.*

Captures d’écran
----------------

Exemple graphique avec les boites de dialogues à partir du menu
d’impression de Cups :

![](http://www.sputnick-area.net/ftp/downloads/snapshot1262216241.png)

Menu contextuel sous KDE ( clic droit sur un fichier pdf, txt, odt… ) :

![](http://img63.imageshack.us/img63/9335/fax4free.png)

Menu contextuel sous Gnome ( clic droit sur un fichier pdf, txt, odt… )
:\
*A noter que ce menu n’apparaît qu’après avoir lancé cups2freefax au
moins une fois via cups.*

![](http://omploader.org/vNGFqbQ)

Après le choix du menu contextuel ou de Cups :

![](http://www.sputnick-area.net/ftp/downloads/snapshot1262216245.png)

Envoi effectif du fax :

![](http://pix.toile-libre.org/upload/original/1333509024.png)

Features
--------

*Voici les principales features gérées :*\
 - gestion des faxs en CLI ou GUI ( ligne de commande ou en graphique )
de façon automatique (nul besoin d’ouvrir un navigateur)\
 - les identifiants de login sont conservés dans la configuration dés la
première utilisation (il transite sur le web (de façon cryptée via SSL)
pour l’interface free uniquement)\
 - configuration et répertoire de fax dans /etc/cups2freefax/ qui peut
être outre-passée avec \~/.config/cups2freefax/\*\
 - implémentation de toutes les options de la console fax Free ( cacher
le destinataire, option de confirmation par mail ) et plus : \
 récupération des faxs (via le script et en cron journalier) tels que
reçus par le destinataire avec durée paramétrable de 0 à l’infini\
 - envoi par menu contextuel pour KDE et/ou Gnome (accessible seulement
après au moins une utilisation de cups2freefax)\
 - répertoire téléphonique des numéros de faxs\
 - vérification optionnelle des nouvelles versions de cups2freefax\
 - configuration de l’imprimante virtuelle automatique (quand vous
voulez imprimer, vous aurez une nouvelle imprimante CUPS2FREEFAX)\
 - gestion de l’impression en réseau via export DISPLAY, nécessite de
configurer le serveur X pour cela. (Expérimental)

Ce projet a été testé avec succès sous Linux ( différentes distributions
).

Prérequis packages système
--------------------------

Les pré requis sont **cups**, **zenity**, **gcc** et les module Perl
**WWW::Mechanize** et **Net::SSLeay**.

Pour tout installer sous Debian et dérivés ( ubuntu, knoppix, kanotix,
linspire, mepis, xandros…) en console :\
*on peux aussi utiliser **synaptic** pour les réfractaires à la ligne de
commande*\

    sudo apt-get update; sudo apt-get install cups zenity gcc libwww-mechanize-perl libnet-ssleay-perl

Archlinux :

    pacman -Sy cups zenity gcc perl-www-mechanize perl-net-ssleay

Mageia :

    urpmi cups zenity urpmi perl-HTML-Tree perl-WWW-Mechanize perl-Net-SSLeay perl-LWP-Protocol-https

RedHat, Centos :

    yum install cups zenity gcc perl-HTML-Tree perl-WWW-Mechanize perl-Net-SSLeay perl-LWP-Protocol-https

OpenSuse :

    zypper install cups zenity gcc perl-HTML-Tree perl-WWW-Mechanize perl-Net-SSLeay perl-LWP-Protocol-https

Gentoo :

    emerge net-print/cups gnome-extra/zenity sys-devel/gcc dev-perl/WWW-Mechanize dev-perl/Net-SSLeay

Install
-------

Ouvrir une console en tant que *user simple* ( xterm, konsole,
gnome-terminal… ) dans une session graphique X ( Xorg ).\

    cd /tmp
    wget http://www.sputnick-area.net/scripts/cups2freefax/cups2freefax_current.run
    chmod +x cups2freefax_current.run
    sudo /tmp/cups2freefax_current.run

Si vous n’utilisez pas sudo concernant cette dernière ligne
d’instructions, tapez :\

    su    # taper son mot de passe "en aveugle"
    /tmp/cups2freefax_current.run

Usage avancé
------------

L’installeur gère quelques options en utilisation avancée :\

    # bash /tmp/cups2freefax_201004262125.run -- -h
    Verifying archive integrity... All good.
    Uncompressing  installation de cups2freefax ....................

    Aide des options de cups2freefax :
    ./cups2freefax_xxx.run
    ./cups2freefax_xxx.run -f         installer uniquement le script fax4free utilisable seulement en CLI
    ./cups2freefax_xxx.run -u         désinstallation de cups2freefax et fax4free
    ./cups2freefax_xxx.run -d :N     N est le numéro du DISPLAY, :0 pour l'installation si non renseigné.
    ./cups2freefax_xxx.run -h        la présente aide

Message final du package d’install
----------------------------------

La sortie comportant le tutoriel en utilisation basique ( se laisser
guider… )\

    # ./cups2freefax_201204040324.run 
    Verifying archive integrity... All good.
    Uncompressing CUPS2FREEFAX............
    Le répertoire "/home/sputnick/.cups2freefax" a été déplacé dans "/home/sputnick/.config/cups2freefax" pour suivre les préconisations XDG.
    Voulez vous etre mis au courant des futures versions de cups2freefax ? (N/o) >>> o
    Voulez vous installer une nouvelle action "fax" pour le menu contextuel des navigateurs de fichiers KDE et/ou de Gnome ? (N/o) >>> o
    1) kde
    2) gnome
    3) kde-Et-Gnome
    4) aucun
    #? 3
    Ok sputnick, C'est terminé. Besoin d'un tutoriel ? Voir http://redmine.sputnick-area.net/wiki/cups2freefax

Configuration personnelle ( ce n’est normalement utile que pour faire des corrections )
---------------------------------------------------------------------------------------

On gère cela via le fichier
\*~/.config/cups2freefax/cups2freefaxrc\*\\ et\\ **/etc/cups2freefaxrc**\\ :
\
On\\ peux\\ y\\ paramétrer\\ les\\ mêmes\\ options\\ que\\ dans\\ la\\ console\\ web\\ FAX\\ de\\ http://free.fr,
\
\<pre\>\
\#\\ Fichier\\ d’environnement\\ de\\ cups2freefax\\ sourcé\\ par\\ cups2freefax.bash
\
\#\\ Login\\ et\\ password\\ de\\ l’interface\\ free\
login=xxxxxxxxx\
password=xxxxxxxx
\
\#\\ Masquer\\ le\\ numéro\\ appelant\\ :\\ \
cups2freefax\_hide\_fax\_number=no
\
\#\\ Recevoir\\ un\\ rapport\\ de\\ transmission\\ par\\ e-mail\\ :\\ \
cups2freefax\_email\_confirmation=no
\
\#\\ Durée\\ en\\ jours\\ de\\ rétention\\ de\\ la\\ copie\\ des\\ faxs\\ envoyés.\
\#\\ Pour\\ ne\\ rien\\ récuperer,\\ mettre\\ cups2freefax\_faxs\_store=0\
cups2freefax\_faxs\_store=365
\
\#\\ Support\\ de\\ l’export\\ DISPLAY\\ en\\ réseau\
\#cups2freefax\_export\_display=true\
\</pre\>
\
A\\ noter\\ :\\ le\\ fichier\\ de\\ config\\ du\\ \$HOME\\ outrepasse\\ **/etc/cups2freefaxrc**\\ si\\ des\\ variables\\ y\\ sont\\ renseignées
\
h2.\\ Édition\\ manuelle\\ du\\ répertoire\\ des\\ numéros\\ de\\ fax\\ 
\
Le\\ répertoire\\ peux\\ être\\ mis\\ en\\ commun\\ dans\\ /etc/cups2freefax/repertoire\_tel\_fax\\ ou\\ bien\\ personnel\\ dans~/.config/cups2freefax/repertoire\_tel\_fax\

    $ editor ~/.config/cups2freefax/repertoire_tel_fax</pre>
    Utiliser cette syntaxe :
    <pre>
    0148765442 Pierre-Henry
    0954661277 Alex_3f
    0455669977 FakeLaule

Sources
-------

Pour voir les sources :\

    cd /tmp
    wget http://www.sputnick-area.net/scripts/cups2freefax/cups2freefax_current.run
    chmod +x cups2freefax_current.run
    mkdir /tmp/sources-cups2freefax
    cd !$
    /tmp/cups2freefax_current.run --tar xf
    ls -l

Ou bien téléchargez le **.tar.gz, c’est la même chose.
\
h2. Troubleshooting
\
Si un problème survient, surveiller
\~/.config/cups2freefax/log/cups2freefax.log,
/var/log/cups/cups2freefax\_log et /var/log/cups/error\_log
\
Vous pouvez aussi faire un**bug report**, une**feature request\* ou une
**demande de support** sur :\
https://redmine.sputnick-area.net/projects/cups2freefax/issues/new

Enjoy ;)

++sputnick;
