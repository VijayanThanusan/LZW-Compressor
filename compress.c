#include "header.h"

/*!\file compress.c
 * \brief Fichier gérant la compression LZW
 * \author Thanushan Vijayakulanathan
 * \date 05 Juin 2016
 */


/*!\brief Taille de départ du dictionnaire, (256 valeurs ASCII + 3 code de controle)*/
static int taille_dico = 259;
/*!\brief Nombre de bit de départ à écrire*/
static int nb_bit = 9;

/*!\brief Fonction qui charge le dictionnaire en initialisant ses valeurs, et en les remplissants avec les 256 caractère du code ASCII*/
void init_dico()
{
  int i;
  
  dico = malloc(MAX_ELEMENTS * sizeof(char*));
  assert(dico);
 
  for(i=0; i<256 ;i++)
    {
      dico[i] = malloc(2 * sizeof(char));
      assert(dico[i]);
    }
  
  int j;
  for(i=0; i<256 ;i++)
    {
      for(j=0;j<2;j++)
	dico[i][j] = 0;

      if(i<256)
	{
	  dico[i][0] = i;
	  strlen_dico[i][0] = 1;
	}
    }
}

/*!\brief Fonction qui vide le dictionnaire, et le rerempli avec les 256 caractere du code ASCII*/
void vider_dictionnaire()
{
  int i,j;
      
  for(i=0 ; i<MAX_ELEMENTS ; i++)
    {
      if(dico[i] != NULL)
	{
	  for(j=0; j<strlen_dico[i][0]; j++)
	    dico[i][j] = 0;
	}
    }
  for(i=0 ; i<256 ; i++)
    dico[i][0] = i;
}

/*!\brief Fonction qui vide le dictionnaire et libère la mémoire*/
void detruire_dictionnaire()
{
  int i;
  for(i=0 ; i<MAX_ELEMENTS ; i++)
    if(dico[i] != NULL)
      free(dico[i]);
}

/*!\brief Fonction qui modifie le nombre de bit à écrire en fonction de la longueur du code en bit
 *Si le nombre de bit a écrire augmente au cour du programme, le code de controle 256 est écrit 
 * afin de prévenir le décompresseur de ce changement
 */
void change_number_bit(FILE *out)
{
  if(taille_dico == 512)
    { ecrire_code(256, nb_bit, out)   ;   nb_bit = 10 ;}
  if(taille_dico == 1024)
    { ecrire_code(256, nb_bit, out)   ;   nb_bit = 11 ;}
  if(taille_dico == 2048)
    { ecrire_code(256, nb_bit, out)   ;   nb_bit = 12 ;}
  if(taille_dico == 4096)
    { ecrire_code(256, nb_bit, out)   ;   nb_bit = 13 ;}
}


/*!\brief Fonction qui écrit sur n bit, le code passé en paramètre dans le fichier passé en paramètre.*/
void ecrire_code(int code, int nb_bits, FILE* out)
{
  int i=0;
  static char octect = 0;
  static int position_octect = 0;

  for(i=0 ; i<nb_bits ; i++)
    {
      //Si bit a 1, alors mise a 1
      if((1 & (code >> i)) == 1)
  	octect |=  (1 << position_octect);
      //Sinon mise a 0
      else
        octect &=  ~(1 << position_octect);

      position_octect++;

      if (position_octect == 8)
  	{
  	  fputc(octect, out);
  	  position_octect = 0;
	  octect = 0;
  	}
    }
}


/*!\brief Fonction qui ajoute un caractere passé en parametre au pointeur sur la chaine passé en paramètre*/
void ajouter_a_sequence(char **chaine, int taille_sequence, int car)
{
  char *temp = *chaine;
  static int taille_limite = 100;

  if(taille_sequence >= taille_limite -2)
    {
      taille_limite += 100;
      *chaine = realloc(*chaine, taille_limite * sizeof(*chaine));
      assert(*chaine);
    }

  temp[taille_sequence] = car;
  temp[taille_sequence+1] = '\0';
}


/*!\brief Fonction de comparaison de deux chaine de caractères, utilisé dans la fonction \ref found_in_dico
 * retourne -1 si les chaines sont différentes et 0 si elle sont identiques 
 */
int compare(char *sequence, int taille_sequence, char *dico)
{
  int i;
  
  for(i=0; i<taille_sequence ;i++)
    {
      if(sequence[i] != dico[i])
	return -1;
    }
  return 0;
}
	

/*!\brief Fonction qui cherche la sequence passé en paramètre dans le dictionnaire, 
 * retourne -1 si la séquence n'est pas trouvé, 
 * et retourne l'indice de l'élément si elle est trouvée
 */
int found_in_dico(char *sequence, int taille_sequence)
{
  int i;
  
  for(i=0 ; i<taille_dico ; i++)
    {
      //si la taille des deux chaines est la meme alors on les compares ..
      if(strlen_dico[i][0] == taille_sequence)
	{
	  if(compare(sequence,taille_sequence,dico[i]) == 0)
	    return i;
	}
    }
  return -1;
}


/*!\brief Fonction qui affiche la sortie dans le fichier compréssé, prend en paramètre la 
 * sequence en question et le fichier de sortie.
 * Le code écrit est celui de la séquence passé en paramètre sans le dernier caractère
 */
void trouver_code(char *sequence, FILE *compress, int taille_sequence)
{
  char *out = NULL;
  int code;
  int i;
  
  out = malloc(taille_sequence * sizeof(*out));
  assert(out);
  memset(out,0,taille_sequence);
  
  //on séléctionne le contenue de la sequence sans le dernier caractère et on le met dans la chaine 'out'
  if(taille_sequence == 1)
    out[0] = sequence[0]; 
  else
    {
      for(i=0 ; i<taille_sequence-1 ; i++)
	out[i] = sequence[i];
      
      out[i] = '\0';
    }

  //On récupere le code de la chaine de sortie, car a ce stade la séquence se trouve obligatoirement dans le dictionnaire
  code = found_in_dico(out,taille_sequence-1);
  //On adapte le nombre de bit a écrire en fonction de la longueur du code
  change_number_bit(compress);
  //On écri le code dans le fichier compréssé
  ecrire_code(code, nb_bit, compress);

  /*Si le code de sortie est le dernier code codé sur le max de bit autorisé (13), alors on l'écrit puis on insère le code de
    controle 258 pour prevenir le decompresseur qu'il faut vider le dictionnaire, puis on vide le dictionnaire,
    et on repasse au nombre de bit de départ (9)*/
  if(taille_dico == MAX_ELEMENTS)
    { 
      ecrire_code(258, nb_bit, compress);
      nb_bit = 9;
      taille_dico = 259;
      vider_dictionnaire();
    }

  free(out);
}
  

/*!\brief Fonction qui parcours le fichier de données initial et applique la procédure LZW*/
void compress(FILE *in)
{
  int car;
  int found;
  char *sequence = NULL;
  int taille_sequence = 0;
  
  sequence = malloc(100 * sizeof(*sequence));
  assert(sequence);
  memset(sequence,0,sizeof(*sequence));

  while((car = getc(in)) != EOF)
    {
      ajouter_a_sequence(&sequence,taille_sequence, car);
      taille_sequence++;
      
      //si la sequence est dans le dictionnaire on ne fait rien car le caractere est déja ajouté à la sequence
      if((found = found_in_dico(sequence, taille_sequence)) != -1)
	;

      /*si la sequence n'est pas dans le dictionnaire alors on l'inclue dans le dictionnaire, on vide la sequence, et on la
  	rempli avec le caractere actuel afin qu'il devienne le caractere précédent au prochain tour de boucle*/
      else
  	{
	  dico[taille_dico] = malloc(taille_sequence+1 * sizeof(dico[taille_dico]));
	  memcpy(dico[taille_dico], sequence, taille_sequence);
	  strlen_dico[taille_dico][0] = taille_sequence;
	  taille_dico++;
  	  trouver_code(sequence, stdout, taille_sequence);
  	  memset(sequence,0,sizeof(*sequence));
	  taille_sequence = 1;
  	  sequence[0] = car;
  	}
    }

  /*ecrire la dernière sequence avec le code de fin de données.
    On cherche le code de la chaine entiere (avec le dernier caractere) dans le dictionnaire, et on l'affiche en sortie*/
  int code;
  code = found_in_dico(sequence, taille_sequence);
  ecrire_code(code, nb_bit, stdout);
  ecrire_code(257 , nb_bit, stdout);

  free(sequence);
}
