/*!\file header.h
 * \brief Fichier regroupant les prototyes de fonctions et les variables globales
 * \author Thanushan Vijayakulanathan
 * \date 05 Juin 2016
 * 
 */

#ifndef VAR
#define VAR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*!\brief Nombre d'éléments maximum pour le dictionnaire (13 bit maximum)*/
#define MAX_ELEMENTS 8192

/*!\brief Tableau à deux dimensions pour le dictionnaire */
char **dico;
/*!\brief Tableau de meme taille que le dictionnaire pour enregistrer la taille de chaque chaine se trouvant dans celui-ci
  Ceci car pour que le caractere NULL soit pris en compte, il faut passer le pointeur ainsi que la taille de la chaine.*/
int strlen_dico[MAX_ELEMENTS][2];

void init_dico();
void ecrire_code(int code, int nb_bits, FILE* out);
void ajouter_a_sequence(char **chaine,int taillle_sequence, int car);
int compare(char *sequence, int taille_sequence, char *dico);
int found_in_dico(char *sequence, int taille_sequence);
void trouver_code(char *sequence, FILE *compress, int taille_sequence);
void compress(FILE *in);
void change_number_bit(FILE *out);
void vider_dictionnaire();
void detruire_dictionnaire();
void copy(char dico[], char *sequence,int taille);
void decompress(FILE *file);
void decode(int code, char **sequence, char **precedent);


/*! \mainpage Projet LZW - Algorithmique - Documentation
 *
 * \section intro_sec Introduction
 *
 * Bienvenue sur la documentation de l'implémetation du compresseur/decompresseur LZW.
 *
 * Cette documentation a été réalisée dans le cadre du cours d'Algorithmique et structure de données 2 tenue par Mr Jean Méhat, 
 * en deuxième année de licence Informatique à l'Université Paris 8.
 * Cette documentation recense l'ensemble des descriptifs des différentes fonctions qui composent ce projet, ainsi qu'un descriptif
 * lancement et du fonctionnement du programme.
 *
 * <BR>
 *
 * \section compil_sec Compilation
 *
 * \subsection executable - Génerer l'exécutable
 * Dans un terminal, accéder au répertoire du programme, et tapper la commande suivante :
 * @code
 * $ make
 * @endcode
 * Si la compilation se déroule normalement, l'exécutable 'lzw' sera généré dans le dossier source.
 *
 * \subsection documentation - Générer la documentation
 * Dans un terminal, accéder au répertoire du programme, et tapper la commande suivante :
 * @code
 * $ make doc
 * @endcode
 *
 * \subsection clean - Effacer les fichiers temporaires
 * Dans un terminal, accéder au répertoire du programme, et tapper la commande suivante :
 * @code
 * $ make clean
 * @endcode
 *
 * <BR><BR>
 * \section compression Compression
 *
 * \subsection comp_stdin - Compression sur stdin
 * Pour compresser des données de l'entrée standard stdin vers un fichier, lancer l'exécutable suivi du nom du fichier de 
 * destination précédé d'un chevron '>'
 * @code
 * $ ./lzw > destination
 * @endcode
 * Taper CTRL+D pour mettre fin à la saisie sur stdin.
 *
 * \subsection comp_file - Compression d'un fichier
 * Pour compresser un fichier, entrer l'option de compilation '-c' suivi du nom du fichier à compresser, ainsi que le fichier de 
 * destination précédé d'un chevrion '>':
 * @code
 * $ ./lzw -c fichier > destination
 * @endcode
 * Si aucun fichier de sortie n'est spécifié pour la compression, les données compréssées seront affichées sur la
 * sortie standard stdout.
 *
 * <BR><BR>
 *
 * \section decompression Décompression
 *
 *\subsection decomp_file - Décompression d'un fichier
 * Pour décompresser un fichier, entrer l'option de compilation '-d' suivi du nom du fichier à decompresser, ainsi que le fichier de 
 * destination précédé d'un chevrion '>'
 * @code
 * $ ./lzw -d fichier > destination
 * @endcode
 * Si aucun fichier de sortie n'est spécifié pour la décompression, les données decompréssées seront affichées sur la
 * sortie standard stdout.
 *
 * <BR><BR>
 *
 * \section fonctions Fonctionalitées
 * - Le dictionnaire a une taille maximale de 8192 entrées, permettant au compresseur d'écrire des codes de 13 bit maximum.
 * - Passé cette limite, le dictionnaire se vide pour recommencer à une taille de départ de 259.
 * - Code de contrôle 256 : sert à prévenir le décompresseur de l'augmentation du nombre de bit à lire.
 * - Code de contrôle 257 : sert à indiquer la fin du flux de données.
 * - Code de contrôle 258 : sert à indiquer au décompresseur qu'il faut vider le dictionnaire.
 *
 * <BR><BR>
 *
 * \section sources Sources
 * - Je me suis aidé d'un article sur Wikipedia pour comprendre la manipulation des bits : https://fr.wikipedia.org/wiki/Manipulation_de_bit.
 * 
 * - J'ai à un moment été bloqué car la compression / décompression des fichiers textes se passait sans problèmes, 
 *   mais cela ne marchait pas pour les fichiers exécutables, ou autres fichiers.
 *   Après plusieurs jours de recherches, j'ai donc décidé de demander de l'aide sur un chat de programmation informatique, afin d'avoir
 *   l'avis d'internautes, sur pourquoi les fichiers textes sont bien compréssé/décompresse et pas les fichiers exécutables.
 *   Un internaute m'a demandé si mon programme prenait en compte le caractère NULL, et c'est ce qui m'a mis sur la voie pour régler le problème.
 *
 * <BR><BR>
 *
 * \section problemes Problèmes éventuels
 * - Le programme n'est pas optimisé pour comprésser uniquement si cela est rentable, ainsi il arrive que certains fichiers avec peu de
 *  de répétitions soient plus gros une fois la compression éféctuée qu'avant la compression.
 * - Lorsque le fichier à par exemple le meme caractere des milliers de fois, et que le pointeur acceuillant les caractères lu est réalloué,
 *  pour augmenter la taille de la séquence, une erreur \b "free(): invalid next size" est déclarée par le compilateur à la fin du programme,  
 *  lors de la decompression. Cela n'empeche pas la génération du fichier décomprésse, mais je n'ai pas réussi à trouver l'origine de l'erreur.
 * - La compression met beaucoup de temps pour les gros fichiers.
 */

#endif
