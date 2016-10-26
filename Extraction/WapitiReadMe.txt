Une liste de mots comporte un mot par ligne et contient une ligne vide pour montré le passage au texte suivant

Les scripts bash peuvent être appelé en c++ avec: system("filename.sh");

Extraire la liste des caractériques:
1)placer le dossier data (avec le fichier train.txt) dans le dossier Extraction
2)dézipper wapiti dans le dossier actuel

Extraire les caractéristiques de wapiti sans les mots:
1)lancer extract_l.sh avec la commande: ./extract_l.sh <train.txt> <nomfichier sortie> 

Extraire les caractéristiques de wapiti avec les mots:
1)lancer extract.sh avec la commande: ./extract.sh <train.txt> <nomfichier sortie> 

Extraire des listes de deux ou trois mots consécutifs:
1)lancer groupe2.sh et groupe3.sh pour obtenir des listes groupées par deux ou trois mots: ./groupe2.sh <liste de mots ou caractériques> <nomfichier sortie>

Enlever les signes de ponctuation dans une liste de mots (',' '\'' ':' '`' '.'):
1)lancer supprPonctu.sh: ./suppPonctu.sh <entree> <sortie>

Convertir un text en une liste de mots (marche avec train):
1)lancer textToList.sh: ./textToList.sh <entree> <sortie>
ex: ./textToList.sh data/tain.txt trainwap
