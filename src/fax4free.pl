#!/usr/bin/env perl
# ------------------------------------------------------------------
#    gilles.quenot <AT> sputnick <DOT> fr
#
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of version 2 of the GNU General Public
#    License published by the Free Software Foundation.
# ------------------------------------------------------------------

# 2018-03-11 17:25:05.0 +0100 / Gilles Quenot <gilles.quenot@sputnick.fr>
use strict; use warnings;

my $loginURL = "https://subscribe.free.fr/login/login.pl";		# URL de login console Free
my $temporisation = 30; 										# secondes d'attente entre chaque essai de téléchargement de l'archive

use utf8;
binmode(STDOUT, ":utf8");
binmode(STDERR, ":utf8");
binmode(STDIN,  ":utf8");
use IO::Handle; *STDOUT->autoflush(); *STDERR->autoflush();
use Getopt::Std;
use Env qw/HOME USER DISPLAY/;
use WWW::Mechanize;

my $dieMessage = $0 . "requiert que le fichier ~/.config/cups2freefax/cups2freefaxrc ou /etc/cups2freefax/cups2freefaxrc soit renseigné avec les logins et password de l'interface Free.";
my $message;

# Si on est en interactif, on est donc en CLI.
# Si c'est bien le cas, on affiche sur la sortie standard,
# sinon via zenity.
use constant IS_TERMINAL => -t STDIN;

sub printdie {
	if (IS_TERMINAL) {
		print STDERR "@_\n";
		exit(1);
	}
	else {
        $message = "@_";
		`zenity --error --title fax4free --text "$message"`;
		exit(1);
	}
}

sub printout {
	if (IS_TERMINAL) {
		print "@_\n";
	}
	else {
        $message = "@_";
        `echo "message:$message" | zenity --notification --listen 2>/dev/null`;
	}
}

printdie("$dieMessage\n") unless -s "$HOME/.config/cups2freefax/cups2freefaxrc" || -s "/etc/cups2freefax/cups2freefaxrc";
my @arr = `cat /etc/cups2freefax/cups2freefaxrc $HOME/.config/cups2freefax/cups2freefaxrc 2>/dev/null`;
my (%c2ff, $a, $b);
for (@arr) {
    if (/=/ and !/(^#|^$)/) {
        chomp;
        ($a, $b) = split /=/;
        $c2ff{$a}=$b;
    }
}

printdie($dieMessage) unless $c2ff{'password'} && $c2ff{'login'};

sub Help {
	printdie("Usage : $0 -d <numero destinataire fax> -f <fichier>\n-d <Numero de fax du destinataire>\n-f <document a faxer>") unless @ARGV;
}

my %opts;

getopt('h:d:f:', \%opts);

$c2ff{'cups2freefax_hide_fax_number'} = 'Y' if $c2ff{'cups2freefax_hide_fax_number'} eq 'yes';
$c2ff{'cups2freefax_hide_fax_number'} = 'off' if $c2ff{'cups2freefax_hide_fax_number'} eq 'no';
$c2ff{'cups2freefax_email_confirmation'} = 1 if $c2ff{'cups2freefax_email_confirmation'} eq 'yes';
$c2ff{'cups2freefax_email_confirmation'} = 'off' if $c2ff{'cups2freefax_email_confirmation'} eq 'no';

Help if defined $opts{'h'};

my $Doc2Fax = $opts{'f'} or printdie('Argument obligatoire "-f"');
printdie("Le document $Doc2Fax n'existe pas") unless -e $Doc2Fax;

my $destinataire = $opts{'d'} or printdie('Argument obligatoire "-d"');

my $m = WWW::Mechanize->new( autocheck => 1 );
$m->agent_alias( 'Linux Mozilla' );
$m->get($loginURL);
$m->submit_form( fields => {
		login => $c2ff{'login'}, pass => $c2ff{'password'}, ok => "Connexion"
	}
);
my $authreply = $m->content( format => 'text' );
printdie("Authentification erronée\n") if $authreply =~ /Identifiant ou mot de passe incorrect/i;
$m->follow_link( url_regex => qr/menu\.pl.*?telephonie/i );
$m->follow_link( url_regex => qr/phone_fax\.pl/i );
$m->submit_form( fields => {
		masque 			=> 	$c2ff{'cups2freefax_hide_fax_number'},
		destinataire 	=> 	$destinataire,
		email_ack 		=> 	$c2ff{'cups2freefax_email_confirmation'},
		document 		=> 	$Doc2Fax
	}
) or printdie("Impossible d'uploader $Doc2Fax: [$!]\n");
my $overflowreply = $m->content( format => 'text' );
printdie("Quota depassé : Vous envoyez trop de fax\n") if $overflowreply =~ m/Vous envoyez trop de fax/i ;

unlink $Doc2Fax;
$Doc2Fax =~ s!.*/!!;
printout("Le fax $Doc2Fax a bien été envoyé vers $destinataire.\n");
printout("vous allez recevoir une confirmation via mail.\n") if $c2ff{'cups2freefax_email_confirmation'} eq 1;
system("/var/lib/cups2freefax/cron.pl & &>/dev/null");

