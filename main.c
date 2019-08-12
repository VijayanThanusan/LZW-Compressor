#include "header.h"

/*!\file main.c
 * \brief Fichier ou se trouve la fonction main
 * \author Thanushan Vijayakulanathan
 * \date 05 Juin 2016
 * 
 */

//-----------------------------------------------------
int main(int argc, char **argv)
{
  int i;
  FILE *in = NULL;
  char fonction = 0;
  
  //Charger le dictionnaire 
  init_dico();

  //Si pas d'option de compilation, alors compression sur stdin
  if(argc == 1)
    {
      compress(stdin);
      return EXIT_SUCCESS;
    }

  //verifier l'option de compression ou decompression
  if(argc > 1 && strcmp(argv[1],"-c") == 0)
    fonction = 'c';
  else if(argc > 1 && strcmp(argv[1],"-d") == 0)
    fonction = 'd';
  else
    {
      printf("usage : entrer une option : -c pour compresser, -d pour decompresser\n"); 
      return EXIT_FAILURE;
    }
  
  //verifier qu'un fichier est donné en paramètre
  if(argc <= 2)
    {
      printf("usage : entrez le nom d'un fichier\n");
      return EXIT_FAILURE;
    }

  //si tous les parametres sont la entrez dans la boucle
  for(i=2; i<argc; i++)
    {
      //si compression
      if(fonction == 'c')
	{
          in = fopen(argv[i], "r");
          if(in == NULL)
	    {
	      perror(argv[i]);
	      return EXIT_FAILURE;
	    }
          compress(in);
        }

      //si decompression
      if(fonction == 'd')
	{
	  in = fopen(argv[i], "r");
	  
          if(in == NULL)
	    {
	      perror(argv[i]);
	      return EXIT_FAILURE;
	    }
	  decompress(in);
	}
    }
  
  free(in);
  detruire_dictionnaire();
  return 0;
}
