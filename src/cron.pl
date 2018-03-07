#!/usr/bin/env perl
# ------------------------------------------------------------------
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of version 2 of the GNU General Public
#    License published by the Free Software Foundation.
# ------------------------------------------------------------------
# 2018-03-07 23:17:34.0 +0100 / Gilles Quenot <gilles.quenot@sputnick.fr>

# Partie récupération des documenst envoyés

my $loginURL = "https://subscribe.free.fr/login/login.pl";		# URL de login console Free
my $temporisation = 3;  										# secondes d'attente entre chaque essai de téléchargement des archives

use utf8;
use strict; use warnings;
binmode(STDOUT, ":utf8");
binmode(STDERR, ":utf8");
binmode(STDIN,  ":utf8");
use IO::Handle; *STDOUT->autoflush(); *STDERR->autoflush();
use Env qw/HOME/;
use WWW::Mechanize;

my ($message, $num, $currentMsgId, $currentFichier, $currentFichierNew, @newUrl, @docArr, @arrPresent, %h);
my $dieMessage = $0 . "requiert que le fichier ~/.config/cups2freefax/cups2freefaxrc ou /etc/cups2freefax/cups2freefaxrc soit renseigné avec les logins et password de l'interface Free.";
my $BDD = "$HOME/.config/cups2freefax/docs.db";
my $url = "placeholder";

die("$dieMessage\n") unless -s "$HOME/.config/cups2freefax/cups2freefaxrc" || -s "/etc/cups2freefax/cups2freefaxrc";
my @arr = `cat /etc/cups2freefax/cups2freefaxrc $HOME/.config/cups2freefax/cups2freefaxrc 2>/dev/null`;
my (%c2ff, $a, $b);
for (@arr) {
    if (/=/ and !/(^#|^$)/) {
        chomp;
        ($a, $b) = split /=/;
        $c2ff{$a}=$b;
    }
}

exit(0) if $c2ff{'cups2freefax_faxs_store'} == 0;

die($dieMessage) unless $c2ff{'password'} && $c2ff{'login'};

$c2ff{'cups2freefax_hide_fax_number'} = 'Y' if $c2ff{'cups2freefax_hide_fax_number'} eq 'yes';
$c2ff{'cups2freefax_hide_fax_number'} = 'off' if $c2ff{'cups2freefax_hide_fax_number'} eq 'no';
$c2ff{'cups2freefax_email_confirmation'} = 1 if $c2ff{'cups2freefax_email_confirmation'} eq 'yes';
$c2ff{'cups2freefax_email_confirmation'} = 'off' if $c2ff{'cups2freefax_email_confirmation'} eq 'no';

mkdir("$HOME/.config/cups2freefax/envoyes");

sleep($temporisation*2);
my $m = WWW::Mechanize->new( autocheck => 1 );
$m->agent_alias( 'Linux Mozilla' );
$m->get($loginURL);
$m->submit_form( fields => {
		login => $c2ff{'login'}, pass => $c2ff{'password'}, ok => "Connexion"
	}
);
my $authreply = $m->content( format => 'text' );
die("Authentification erronée\n") if $authreply =~ /Identifiant ou mot de passe incorrect/i;
$m->follow_link( url_regex => qr/menu\.pl.*?telephonie/i );
$m->follow_link( url_regex => qr/phone_fax\.pl/i );

@newUrl = $m->find_all_links( url_regex => qr/phone_fax\.pl\?action=dl\&id=\w+\&idt=\w+\&fichier=.*\&msg=\w+&type=tx/ );

foreach my $urls (@newUrl) {
    $url = $urls->url_abs;

    for (split(/&/, $url)) {
        if ($_ =~ /^fichier=/) {
            ($_, $currentFichier) = split(/=/);
        }
        if ($_ =~ /^msg=/) {
            ($_, $currentMsgId) = split(/=/);
        }
    }
    # On utilise une BDD à plat pour y stocker les couples id/noms
    # afin de ne pas ecraser un fichier deja présent et afin
    # d'etre sur de recupérer le bon fichier au bon moment
    
    # Lecture BDD
    if (-s $BDD) {
        open my $openHandle, "<", $BDD or die "$0: $BDD: [$!]\n";
        @arrPresent = <$openHandle>;
        foreach (@arrPresent) {
            ($a, $b) = split;
            $h{$a} = $b;
        }
        close($openHandle);
    }

    # Ecriture BDD
    if (defined($h{$currentMsgId})) {
        #printout("$currentFichier déjà récupéré");
     }
     else {
        # Si le fichier existe déjà, on incremente
        # le nom de fichier avec .N (comme wget)
        if (-s "$HOME/.config/cups2freefax/envoyes/$currentFichier") {
            @docArr = glob("$HOME/.config/cups2freefax/envoyes/$currentFichier*");
            ($num = $docArr[-1]) =~ s/.*\.//;

            if ($#docArr > 0 && $num =~ /^[0-9]+/) {
                $num =~ s/.*\.//;
                $currentFichierNew = $currentFichier . "." . ++$num;
            }
            else {
                $currentFichierNew = $currentFichier . ".1";
            }
        }
        else {
            $currentFichierNew = $currentFichier;
        }
        $m->get( $url, ':content_file' => "$HOME/.config/cups2freefax/envoyes/$currentFichierNew" )
            or die("Il n'a pas été possible de récupérer $currentFichier dans l'interface web de Free...\n");

        open my $writeHandle, ">>", $BDD or die "$0: $BDD: [$!]\n";
        print $writeHandle "$currentMsgId $currentFichier\n";
        close($writeHandle);

        if (-s "$HOME/.config/cups2freefax/envoyes/$currentFichierNew") {
            # printout("$currentFichier récupéré\n");
        }

        sleep($temporisation);
    }
}
