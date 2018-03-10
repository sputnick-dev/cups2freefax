#!/bin/bash
# ------------------------------------------------------------------
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of version 2 of the GNU General Public
#    License published by the Free Software Foundation.
# ------------------------------------------------------------------
# 2018-03-10 20:46:28.0 +0100 / Gilles Quenot <gilles.quenot@sputnick.fr>


# Doc, bug reports, wiki : https://github.com/sputnick-dev/cups2freefax

. /etc/profile

CURRENT_PDF="$(readlink -f "${1:?}")"
CURRENT_USER="${2:?}"
MYHOME="/home/${CURRENT_USER}"
REPERTOIREFAX1="$MYHOME/.config/cups2freefax/repertoire_tel_fax"
REPERTOIREFAX2="/etc/cups2freefax/repertoire_tel_fax"

mkdir -p $MYHOME/.config/cups2freefax/log

# On crèe le fichier cups2freefaxrc si il n'est pas déjà présent
[[ ! -s "$MYHOME/.config/cups2freefax/cups2freefaxrc" && ! -s "/etc/cups2freefax/cups2freefaxrc" ]] && install -D -m 600 /var/lib/cups2freefax/cups2freefaxrc "/etc/cups2freefax/cups2freefaxrc"
[[ -f ${MYHOME}/.config/cups2freefax/log ]] && chmod 770 -R ${MYHOME}/.config/cups2freefax/log

# On duplique STDOUT et STDERR pour loguer et afficher en même temps.
touch $MYHOME/.config/cups2freefax/log/cups2freefax.log
chmod 600 $MYHOME/.config/cups2freefax/log/cups2freefax.log
exec &> >(tee $MYHOME/.config/cups2freefax/log/cups2freefax.log)

# https://unix.stackexchange.com/questions/429092
export DISPLAY=$(
    ps -u $(id -u) -o pid= |
      xargs -I{} cat /proc/{}/environ 2>/dev/null |
      tr '\0' '\n' |
      grep -m1 '^DISPLAY='
)

xa=$(compgen -W /tmp/xauth-$(id -u)*)
if [[ -s $xa ]]; then
    XAUTHORITY=$xa
else
    export XAUTHORITY=$(
        ps -u $(id -u) -o pid= |
          xargs -I{} cat /proc/{}/environ 2>/dev/null |
          tr '\0' '\n' |
          grep -m1 '^XAUTHORITY='
    )
fi

if [[ $DISPLAY != :[0-9]* ]]; then
    echo >&2 "Impossible de detected DISPLAY, merci de creer un bug report"
    exit 1
fi

if [[ ! -s $XAUTHORITY ]]; then
    echo >&2 "Impossible de detected XAUTHORITY"
fi

# On laisse quelques traces pour nourrir le log.
date
echo "On traite ${CURRENT_PDF} pour ${CURRENT_USER}..."

# On source les variables d'environnement qui vont
# renseigner cups2freefax sur ses options.
[[ -s /etc/cups2freefax/cups2freefaxrc ]] && . /etc/cups2freefax/cups2freefaxrc
[[ -s $MYHOME/.config/cups2freefax/cups2freefaxrc ]] && . $MYHOME/.config/cups2freefax/cups2freefaxrc

if [[ $login == '%%LOGIN%%' || $passwd == '%%PASSWD%%' ]]; then 	# Si on a a faire aux variables template
	zenity --info --title Infos --text "Les 2 questions suivantes permettent de configurer CUPS2FREEFAX avec les access free que vous utiliseriez manuellement\
	\nen naviguant sur http://subscribe.free.fr/login/login.pl, ils ne vous seront plus demandés.\
    Ils sont stockés dans \"$MYHOME/.config/cups2freefax/cups2freefaxrc\" qui n'est lisible que par \"$CURRENT_USER\"."

	login=$(zenity --entry --title Acces --text "Merci de me donner le login de la console Free.")
	sed -i "s@%%LOGIN%%@${login}@" "$MYHOME/.config/cups2freefax/cups2freefaxrc"

	passwd=$(zenity --entry --title Acces --hide-text --text "Merci de me donner le mot de passe de la console Free.")
	sed -i "s@%%PASSWD%%@${passwd}@" "$MYHOME/.config/cups2freefax/cups2freefaxrc"

    chmod 600 "$MYHOME/.config/cups2freefax/cups2freefaxrc"

fi

# Si le répertoire des numeros de FAXs existe, on propose la liste
if [[ -s $REPERTOIREFAX1 || -s $REPERTOIREFAX2 ]]; then
	NUM=$(
		zenity --list \
			--title="Répertoire télephonique" \
			--text="<b>Sélectionner le correspondant</b>" \
			--column="Tel" --column="Nom" ' ' 'COMPOSER UN NUMERO' $(cat 2>/dev/null ${REPERTOIREFAX1} ${REPERTOIREFAX2} | sort -uk 2) || exit 1
		)
fi

# On fait une requête auprès de l'utilisateur afin de déterminer le numéro du destinataire :
if [[ $NUM == ' ' || ( ! -s $REPERTOIREFAX1 && ! -s $REPERTOIREFAX2 ) ]]; then
	NUM=$(zenity --entry --title cups2freefax --text "Numéro de fax du correspondant ?\n( les espaces seront retirés )" || exit 0)	
	NUM=$(tr -d ' ' <<< $NUM)

	# Si le numéro existe dans la "DB" :
	if grep &>/dev/null "^$NUM " ${REPERTOIREFAX1} ${REPERTOIREFAX2}; then
		zenity --error --title cups2freefax --text "Le numéro est dejà attribué à $(awk 2>/dev/null '$1==cnum {print $2}' cnum=$NUM ${REPERTOIREFAX1} ${REPERTOIREFAX2}).\
			\nOn peux modifier manuellement l'entrée dans le fichier \n$REPERTOIREFAX1" || exit 0
        exit 1
	else
        if [[ $NUM == [0-9]* ]]; then
            # Si il n'existe pas, on propose d'ajouter cette nouvelle entrée dans le répertoire
            if zenity --question --title question --text "Ajouter $NUM au répertoire téléphonique ?"; then
                FAXNAME=$(zenity --entry --title cups2freefax --text "Nom du correspondant ?\n( les espaces seront remplacés par des \"_\" )")
                FAXNAME=$(tr -d ' ' <<< $FAXNAME)
                echo "$NUM $FAXNAME" >> $REPERTOIREFAX1
                chmod 600 $REPERTOIREFAX1
            fi
        else
            exit 1
        fi
	fi
fi

set -x
[[ ${NUM} && "${CURRENT_PDF}" ]] && HOME=$MYHOME /usr/bin/fax4free -d ${NUM} -f "${CURRENT_PDF}"
set +x

chown -R ${CURRENT_USER}: $MYHOME/.config/cups2freefax

if [[ $cups2freefax_faxs_store != 0 && $cups2freefax_faxs_store =~ [0-9]+ ]]; then
	find $MYHOME/.config/cups2freefax/envoyes -type f -mtime +$cups2freefax_faxs_store -exec \rm {} \;
fi
