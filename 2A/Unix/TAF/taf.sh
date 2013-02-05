#!/bin/sh

CACHE_DIRECTORY='./cache/'

# delete all files into 'cache' directory
cleanFiles() {
  echo "  - Clean cache directory ..."
  rm $CACHE_DIRECTORY/*
  rm html.html e_var1 taf.txt
  echo "      ... done !"
  
  # @TODO: report list of deleted files
}

# clean : delete all files in 'cache' directory
cmdInit() {
  echo " - Initialization of TAF script"
  
  if [ ! -d $CACHE_DIRECTORY ] # Check if 'cache' directory exists
  then
    mkdir $CACHE_DIRECTORY
    echo "   - Cache directory created"
  fi
  
  cleanFiles
  
  echo " Initialiation done"
}

# download informations about AIRP (html, txt)
## taf -d FR DE 
cmdDownload() {
  
  site='http://wx.rrwx.com/'
  
  for arg in $*
  do
    if [ $1 = "fr" ] || [ $1 = "de" ] || [ $1 = "it" ] || [ $1 = "be" ] || [ $1 = "po" ] || [ $1 = "es" ] || [ $1 = "as" ] || [ $1 = "ch" ]; then
      echo "Check if $arg datas exists"
      for type in ".htm" "-txt.htm"
      do
        uri="taf-${arg,,}$type"
        echo "  Check if $uri exists"
        
        if [ ! -f $CACHE_DIRECTORY/$uri ] # Check if 'cache' directory exists
        then
          echo "  Downloading $site/$uri"
          curl $site/$uri >> $CACHE_DIRECTORY/$uri
          echo "  $site/$uri -> $CACHE_DIRECTORY/$uri"
        else
          echo "  File $site/$uri is cached"
        fi
        echo ""
      done
    fi
    echo ""
  done
  
  #curl --connect-timeout 100 -O $destFile $uriFile
}

# extract informations about AIRP
cmdExtract() {
  while [ $# -gt 0 ]; do
    country=$1
    query=$2
    
    # Return files
    uri1="$CACHE_DIRECTORY/taf-${country,,}.htm"
    uri2="$CACHE_DIRECTORY/taf-${country,,}-txt.htm"

    # Check if 'cache' directory exists
    if [ ! -f $uri1 ] || [ ! -f $uri2 ]
    then
      echo "$uri1 or $uri2 does not exists."
      echo "  Download them (${country,,})"
    
    else
      echo "  Search into '${country,,}' airport : '$query'"

      sed -n "/$2/s/.*<\/td><td .[^>]*><b>\(.[^<]*\)<\/b>.*$/\1/p" "$uri1" > e_var1
      ICAO=$(<e_var1)
      # :s

      if [[ -n $ICAO ]]; then
        cat $uri2 | sed -r ':a;N;$!ba;s/.*<pre>(.*)<\/pre>/\1/g' | sed -r '/<.+>/d;/^$/d' | sed 's/&nbsp;/ /g' | while read airp datas
        do
          if [[ " $ICAO " = " $airp " ]]
          then
            echo "  airport: $airp"
            echo "    datas: $datas"
            echo ""
            # echo "{$query} | $datas" >> taf.txt
            echo "$airp $datas" >> taf.txt
          fi
        done
      else
        echo "  Error airport '$query' (ICAO=$ICAO) not found"
      fi
      
      echo ""
    fi
    shift 2
  done
  
  #cat $uri2 | while read LINE
  #do
  #  echo "- $LINE"
  #done
    #echo $datas | while IFS=' ' read d
      #do
      #  echo "    - [$d]"
      #done
}

cmdReport() {
  # Analyse and create Html report about AIRP
  
  echo "Generation of your report. Please wait"

  echo -n "" > html.html
  
  echo "<!DOCTYPE html>
<html>
  <head>
    <meta charset=\"utf-8\" />
    <link rel=\"stylesheet\" href=\"http://twitter.github.com/bootstrap/assets/css/bootstrap.css\" />
    <style>
      #global { margin: auto auto; width: 800px; padding-top: 2em; }
      article { border: 3px solid black; margin: 1em auto; }
      article > header { background-color: black; color: white; padding: 3px 10px; }
      article > header * { font-size: 1em; line-height: 1em; }
      article > div h1 { font-size: 1em; font-line: 1em; margin-top: 1em; background-color: #eaeaea; padding-left: 2em; }
      dl dt, dl dd { min-height: 1em; }
    </style>
  </head>
  <body>
    <div id=\"global\">" >> html.html
  
  while read -r airp datas; do
    if [[ -n $airp ]]; then
      
      echo "
      <article>
        <header><h1>$airp</h1></header>
        <div>
          <h1><i class=\"icon-chevron-right\"></i> TAF</h1>
          <dl class=\"dl-horizontal\">" >> html.html

      ## On récupère les informations du raport
      set $datas
      isTempo=0
      text=
      month=`date +%B`

      for param in $*; do
        text=
        # @source http://www.developpez.net/forums/d936846/systemes/linux/applications-shell-environnements-graphiques/shell/expression-reguliere-case-esac/#post5281570
        case "$param" in
          
          $(echo "${param}" | egrep "TEMPO"))
            text="</dl><h1><i class=\"icon-chevron-right\"></i> Temporary</h1><dl class=\"dl-horizontal\">";;

          $(echo "${param}" | egrep "BECMG"))
            text="</dl><h1><i class=\"icon-chevron-right\"></i> Becoming</h1><dl class=\"dl-horizontal\">";;
          
          $(echo "${param}" | egrep "^[0-9]{6}Z$"))
            text="<dt>Emitted</dt><dd>${param:0:2} ${month} @ ${param:2:2}h${param:2:2}m</dd>";;

          $(echo "${param}" | egrep "^[0-9]{4}/[0-9]{4}$"))
            text="<dt>Periode</dt><dd>${param:0:2} ${month} @ ${param:2:2}h00m ... ${param:5:2} ${month} @ ${param:7:2}h00m</dd>";;

          $(echo "${param}" | egrep "^[0-9]{5}KT$"))
            text="<dt>Wind</dt><dd>${param:0:3} @ ${param:3:2} KT</dd>";;

          $(echo "${param}" | egrep "^\-?(BR|SH|RA|DZ|GR|SN)$"))
            text="<dt>Weather</dt><dd>${param}</dd>";;

          $(echo "${param}" | egrep "^PROB[0-9]{2,3}$"))
            text="<dt>Probability </dt><dd>${param:4:2}%</dd>";;

          $(echo "${param}" | egrep "^(BKN|SCT|FEW|OVC)[0-9]{3}(TCU)?$"))
            text="<dt>Clouds</dt><dd>"
            case "${param:0:3}" in
              "BKN") text="${text}Broken";;
              "SCT") text="${text}Scattered";;
              "FEW") text="${text}Few cloud";;
              "OVC") text="${text}OverCast";;
              "OHD") text="${text}OverHead";;
              *) text="?";;
            esac
            let i=${param:5:1}
            let i=i+${param:4:1}*10
            let i=i+${param:3:1}*100
            let i=i*100
            text="$text @ $i ft"

            case "${param:6}" in
              "TCU") text="$text TCU";;
            esac

            text="$text</dd>";;

          $(echo "${param}" | egrep "ADM"))
            text="<dt>Probability </dt><dd>${param:4:2}%</dd>";;

          NSW)
            text="<dt>Weather </dt><dd>No significant weather</dd>";;
          
          *)
            text=
            #echo "param not defined : '$param'"
            #text="<dt>-</dt><dd>'$param'</dd>"
            ;;

        esac
  
        echo "
          $text" >> html.html
      done
      
      echo "
          </dl>
        </div>
      </article>" >> html.html
      
      echo " > $airp : $datas"
    fi
  done < taf.txt;
  
  echo "
    </div>
  </body>
</html>" >> html.html

  echo "Report html.html generated";
}

cmdDEA() {
  # :download :extract :report
  echo -n "" > taf.txt
  cmdDownload "$1"
  cmdExtract "$1" "$2"
  cmdReport "$1"
}

main() {
  echo " Bienvenue dans le programme de TAF"
  echo "  -i                    Initialisation"
  echo "  -d pays               Téléchargement des messages TAF du pays concerné"
  echo "                         s'ils ne sont pas déjà sur l'ordinateur."
  echo "  -e pays aéroport      Extraction du message décrivant un aéroport dans"
  echo "                         un pays donné. Le nom de l aéroport est donné"
  echo "                         en clair."
  echo "  -a                    Analyse du message TAF préalablement extrait"
  echo "  -p pays aéroport      Enchainement des commandes d, e et a."
  echo "  -t [pays aéroport]*   Plan de vol sur plusieurs aéroport."

  echo ""
  
  while [ $# -gt 0 ]; do
    arg=$1
    case "$arg" in
      -i) shift; cmdInit;;
      -d) shift; cmdDownload "$1"; shift;;
      -e) shift; cmdExtract "$1" "$2"; shift 2;;
      -t) shift; # same functon than e
            echo "-t"
            echo -n "" > taf.txt
            cmdExtract "$@";
            shift $#;;
      -a) shift; cmdReport;;
      -p) shift; cmdDEA "$@"; shift $#;;
      *) shift;;
    esac
  done
}

main "$@"


# http://stackoverflow.com/questions/11689868/regex-find-and-replace-over-multi-lines-in-shell