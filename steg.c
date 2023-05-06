#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
  Stores the RGB values of a pixel
*/
typedef struct Pixel{
  int red;
  int green;
  int blue;
} Pixel;

/*
  Linked list to store the comments
*/
typedef struct Comments{
  char * text;
  struct Comments *next;
}Comments;

/*
  Stores the contents of a PPM file
*/
typedef struct PPM{
  /* TODO: Question 1 */
  char format[2];
  Comments *comments;
  int width;
  int height;
  int max;
  Pixel *pixels;
} PPM;

/*
  Function to copy linked list
*/
Comments *copyLL(Comments *oldlist){
  Comments *newlist = NULL;                                    //initializing pointers 
  Comments *prev = NULL;
  Comments *p = malloc(sizeof(Comments));
  p->text = oldlist->text;                                     //copy text from original linked list
  p->next = NULL;
  if (newlist == NULL){                                        //if linked list is empty initialize head
      newlist = p;
      prev = p;
  } 
  else{
      while (oldlist != NULL){                                 //else continue ocpying comments using new linked list pointer
          prev->next = p;
          prev = p;
          oldlist = oldlist->next;
      }
  }
  return newlist;
}

/*
  Function to copy PPM 
*/
PPM * copyPPM(PPM* oldppm){
  PPM *newppm = malloc(sizeof(PPM));                                  //initialize pointer
  
  strcpy(newppm->format,oldppm->format);                              //copy format
  newppm->comments = copyLL(oldppm->comments);                        //copy comments linked list 
  
  newppm->height = oldppm->height;                                    //copy height, width and max
  newppm->width = oldppm->width;
  newppm->max = oldppm->max;
  
  newppm->pixels = malloc(oldppm->height * oldppm->width * sizeof(Pixel));       //copy pixel array
  for(int i = 0; i < (oldppm->height*oldppm->width); i++){
			newppm->pixels[i].red = oldppm->pixels[i].red;  
			newppm->pixels[i].green = oldppm->pixels[i].green; 
			newppm->pixels[i].blue = oldppm->pixels[i].blue;  
		}
  return newppm;
}


/*
  Returns the PPM struct from the file fd
*/
PPM * getPPM(FILE* fd){	
  PPM* ppm = (PPM*) malloc(sizeof(PPM));	
  char com[100];
  /* TODO: Question 2a */                                    
  fscanf(fd,"%s",ppm->format);                                    //parse first line as format
  if(ppm->format[0]!='P' && ppm->format[1]!='3'){                 //check for correct format 
    printf("invalid image format");
    exit(0);
  }                                            

  ppm->comments = malloc(sizeof(Comments));                       //allocate space for comments pointer
  Comments *p = ppm->comments;                      
  char c = getc(fd);                                              //parse one charecter 
  while ((c = getc(fd)) == '#'){                                  //check if comment
    fseek(fd, -1, SEEK_CUR);                                      //move file pointer one byte back
    fgets(com,sizeof(com), fd);                                   //parse one comment
    p->text = malloc(sizeof(Comments));                           //allocate space for text field of linked list
    p->text = com;                                                //assign comment
    p->text[strlen(com) - 1] = '\0';                              //append end of string constant to comment
    p->next = malloc(sizeof(Comments));                           //allocate space for next field
    p = p->next;                                                  //goto next comment if any (while loop)
  }

  fseek(fd, -1, SEEK_CUR);                                        //if not comment move file pointer back one byte
  fscanf(fd, "%d", &ppm->width);                                  //parse width
  fscanf(fd, "%d", &ppm->height);                                 //parse height
  fscanf(fd, "%d", &ppm->max);                                    //parse max 

  ppm->pixels = (Pixel *) malloc(ppm->height * ppm->width * sizeof(Pixel)); //allocate space for pixel array
  for(int i = 0; i < (ppm->height*ppm->width); i++){                        //for loop until end of 2D array
			fscanf(fd," %d",&ppm->pixels[i].red);                                 //parse red field of pixel
			fscanf(fd," %d",&ppm->pixels[i].green);                               //parse green field of pixel
			fscanf(fd," %d",&ppm->pixels[i].blue);                                //parse blue field of pixel
		}

  return ppm;                                                      //return PPM struct
}

/*
  Prints a PPM struct in the format of a PPM file
*/
void showPPM(PPM* ppm){

  /* TODO: Question 2b */
  printf("%s\n",ppm->format);                                      //print format of ppm file

  Comments *p = ppm->comments;                                     //initialize pointer to print comments
  while(p->next != NULL){                                          
    printf("%s\n",p->text);                                        //print text field (comments) of linked list
    p = p->next;
  }

  printf("%d %d\n%d\n", ppm->width, ppm->height, ppm->max);        //print width, height and max

  for(int i=0; i<ppm->height*ppm->width; ++i){
    printf("%d %d %d\n", ppm->pixels[i].red, ppm->pixels[i].green, ppm->pixels[i].blue);       //print pixel array elements 
  }
}

/*
  Loads a file and returns it
  Prints error message and returns NULL if file can't be found or isn't a valid PPM
*/
PPM* loadPPMFromFile(char* filename){
  FILE* file;
  /* TODO: step 1, open the file */
  file = fopen(filename,"r");
  /* step 2: Check that file has been loaded correctly; show error otherwise*/
  if(file != NULL){
    PPM* ppm;
    /* TODO: step 3, get the PPM data from the file with the getPPM function */
    ppm = getPPM(file);
    if(ppm == NULL){
      /* TODO: step 4, display error if file cannot been parsed into a PPM struct*/
      printf("error:file cannot been parsed into a PPM struct ");
      exit(0);
    }

    /* TODO: step 5, close the file */
    fclose(file);
    /* step 6: return the PPM */
    return ppm;
  } else {
    fclose(file);
    return NULL;
  }
}

/*
  Encodes text into red field of PPM
  Returns the encoded PPM
*/
PPM* encode(char* text, PPM* i){	 

  /* TODO: Question 2c */ 
  PPM * p = copyPPM(i);                                  //initialize pointer to point to ppm created using copyPPM function
  srand(time(0));                                        //initialize random number generator
  int r = (rand()%100)+100;                              //random number generator made so that value no too high nor negative values are produced
  int k =0;
  int j = r;
  for(k=0;k<strlen(text);k++){                           
  p->pixels[j].red = text[k];                            //replace random red fields in array 
  j = j+r;                                               //make sure elements encoded in pixels succesfully
  }
  return p;
}

/*
  Compares 2 PPM structs and decodes the message hidden within
  Returns the decoded message if the images have the same dimensions
  Returns NULL otherwise
*/
char* decode(PPM* i1, PPM* i2){
  char* decodedMsg = malloc(sizeof(char));                    
  /* TODO: Question 2d */
  int j = 0;
	for(int i = 0; i < (i1->height*i1->width); i++){              
			if( i1->pixels[i].red != i2->pixels[i].red){                     //compare pixels of original array with encoded array
				decodedMsg = realloc(decodedMsg,100);                          //resize memory block pointed by decodedMsg
				decodedMsg[j] = i2->pixels[i].red;                             //append the value on decodedmsg pointer if descrepancy found between pixels of arrays
				j = j + 1;
				}
		} 
  decodedMsg[j]='\0';                                                  //append end of string constant to decodedMsg
	return decodedMsg;
}

/* TODO: Question 3 */
int main(int argc, char* argv[]){
	
  /* check arguments */
  
  switch(argc) {
  case 2:
    /* TODO: not enough arguments, print an error message */
    if(argc!=3 && argc!=4){
    printf("Incorrect no. of arguments(Encoding:-3, Decoding:-4");
    exit(0);
  }
    break;
  case 3:
    if(strcmp(argv[1], "e") == 0){ //Argument "e" - encode PPM
      PPM* ppm = loadPPMFromFile(argv[2]);
				
      /*Check that PPM is valid; continue if it is, exit otherwise */
      if(ppm != NULL){
        PPM* encodedPPM;
        /* TODO: encode the ppm with the text and assign to encodedPPM */
        char * message = malloc(255 * sizeof(char));
        fprintf(stderr,"Enter message to encode: ");
        fgets(message,255,stdin);
        encodedPPM = encode(message,ppm);
        /*Check that no error occured*/
        if(encodedPPM == NULL){
          return EXIT_FAILURE;
        } else {
          /* TODO: print a confirmation that encoding was successful */
          fprintf(stderr,"Encoding complete\n");
          /* TODO: print the PPM to standard output with showPPM */
					showPPM(encodedPPM);
          return EXIT_SUCCESS;				
        }
      } else {					
        return EXIT_FAILURE;
      }
    } else {
      fprintf(stderr, "Unrecognised or incomplete command line.\n");
      return EXIT_FAILURE;			
    }
    break;
  case 4:
    if(strcmp(argv[1], "d") == 0){ //Argument "d" - decode PPM
      PPM* comparisonPPM;
      PPM* encodedPPM;

      /* TODO: get comparison file filename from argv, load it with
         loadPPMFromFile then assign to comparisonPPM */
      
      comparisonPPM = loadPPMFromFile(argv[2]);
      /* TODO: get encoded file filename from argv, load it with
         loadPPMFromFile then assign to encodedPPM */
			
      encodedPPM = loadPPMFromFile(argv[3]);		
      /*Check both PPMs are valid; continue if so, exit otherwise */
      if(comparisonPPM != NULL && encodedPPM != NULL){
        char* decodedMsg;
        
        /* TODO: decode the encodedPPM with the comparisonPPM and assign to decodedMsg */ 
        decodedMsg = decode(comparisonPPM,encodedPPM);
        /*Check that the message has been decoded*/
        if(decodedMsg != NULL){
          /* TODO: print a confirmation message that the message was decoded */ 
          fprintf(stderr, "Decoding complete\n");

          /* TODO: print the decoded message */
          printf("Decoded message: %s",decodedMsg);
          return EXIT_SUCCESS;
        } else {
          return EXIT_FAILURE;
        }				
      } else {
        return EXIT_FAILURE;
      }			
    } else {
      fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
      return EXIT_FAILURE;			
    }
    break;
  default:
    fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
    return EXIT_FAILURE;
  }
}

