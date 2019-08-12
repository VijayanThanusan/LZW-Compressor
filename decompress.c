#include "header.h"

/*!\file decompress.c
 * \brief Fichier gérant la décompression LZW
 * \author Thanushan Vijayakulanathan
 * \date 05 Juin 2016
 * 
 */


/*!\brief Taille de départ du dictionnaire, (256 valeurs ASCII + 3 code de controle)*/
static int taille_dico = 259;
/*!\brief Nombre de bit de départ à lire*/
static int nb_bit = 9;
/*!\brief tour = 0 : Début de décompression. tour != 0 : En cour de decompression*/
static int tour = 0;
/*!\brief Variable qui définit le nombre d'élement dans la chaine sequence.*/
int taille_sequence = 0;

//-----------------------------------------------------
/*!\brief Fonction qui lit le fichier compréssé et applique la procédure de décompression LZW*/
void decompress(FILE *file)
{
  char *sequence = NULL, *precedent = NULL;
  static int position_bit_char = 0;
  int code = 0, receptacle = 0, i=0, car = 0;

  sequence = malloc(100 * sizeof(char));
  assert(sequence);
  precedent = malloc(100 * sizeof(char));
  assert(precedent);

  car = getc(file);
  
  for(i=0 ; ; i++)
    {      
      //lecture du code bit par bit
      for(i=0; i<nb_bit ; i++)
	{
	  if((1 & (car >> position_bit_char)) == 1)
	    receptacle |= (1 << i);
	  else
	    receptacle &= ~(1 << i);
	  
	  position_bit_char++;

	  if(position_bit_char == 8)
	    {
	      position_bit_char = 0;
	      car = getc(file);
	    }
	}

      code = receptacle;

      /*Fin du flux de données*/
      if(car == EOF || code == 257)
	  break;

      /*Vidage du dictionnaire*/
      if(code == 258)
	{
	  nb_bit = 9;
	  taille_dico = 259;
	  vider_dictionnaire();
	  tour = 0;
	  taille_sequence = 0;
	  memset(sequence,0,sizeof(*sequence));
	}

      /*Augmentation du nombre de bit à lire*/
      else if(code == 256)
	nb_bit++;

      /*Décodage du code*/
      else
	{
	  decode(code, &sequence, &precedent);
	  int j;
	  for(j=0;j<strlen_dico[code][0];j++)
	    fputc(sequence[j], stdout);
	}
    }
  
  free(sequence);
  free(precedent);
}

//-----------------------------------------------------
/*!\brief Fonction qui trouve la séquence du code passé en paramètre, tout en regénérant au fur et à mesure le dictionnaire,
  Passage de pointeur de pointeur afin de pouvoir effectuer un realloc des chaine si besoin est, dans la fonction ajouter_a_sequence()*/
void decode(int code, char** sequence, char **precedent)
{

  //lire le premier caractere du fichier compréssé
  if(tour == 0)
    {
      ajouter_a_sequence(sequence,taille_sequence, code);
      tour++;
      taille_sequence++;
    }
  
  else
    {
      //si code dans le dictionnaire
      if(code < taille_dico)
	{
      	  memcpy(*precedent,*sequence, taille_sequence);
	  memcpy(*sequence, dico[code], strlen_dico[code][0]);
	  ajouter_a_sequence(precedent, taille_sequence, *sequence[0]);
	  taille_sequence++; 
	  dico[taille_dico] = malloc( taille_sequence+1 * sizeof(char));
	  assert(dico[taille_dico]);
	  memcpy(dico[taille_dico], *precedent, taille_sequence);
	  strlen_dico[taille_dico][0] = taille_sequence;
	  taille_sequence = strlen_dico[code][0];
	  taille_dico++;
	}
      //si code pas dans le dictionnaire
      else
	{
	  ajouter_a_sequence(sequence,taille_sequence, *sequence[0]); 
	  taille_sequence++; 
	  dico[taille_dico] = malloc( taille_sequence+1 * sizeof(dico[taille_dico]));
	  memcpy(dico[taille_dico], *sequence, taille_sequence);
	  assert(dico[taille_dico]);
	  strlen_dico[code][0] = taille_sequence ; 
	  taille_dico++;
	  memset(*sequence,0,sizeof(**sequence));
	  memcpy(*sequence, dico[code], taille_sequence);
	}
    }
}
