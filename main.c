#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"


/*
Name: Rupam Chakraborty
Date: 23/05/24
Description: LSB Image Stegnography project
*/

int main()
{
    printf("\n\n\t\t\t------------------- ::: LSB Image Steganography Project ::: --------------------\n\n");

    int i=1;

    while(i)
    {
        printf("Available options to perform -> \n");
	printf("\n1.Encode Data into Image\n2.Decode Data from Image\n\nEnter your choice(e-Encode, d-decode): ");
	char opt;
	scanf(" %c", &opt);

	switch(opt)
	{
	    case 'e': printf("\n\t\t::: Encoding started :::\n");
		      EncodeInfo encInfo;	// creating a structure variable for encoding 
		      encInfo.src_image_fname = "beautiful.bmp";
		      encInfo.stego_image_fname = "output.bmp";
		      encInfo.secret_fname = "secret.txt";
		      do_encoding(&encInfo);	// calling encoding function
	              printf("\n\t\t::: Encoding Completed :::\n");
		      break;
	    case 'd': printf("\n\t\t::: Decoding started ::: \n");
		      DecodeInfo decInfo;	// creating a structure variable for decoding
		      decInfo.stego_image_fname = "output.bmp";
		      //decInfo.output_file_fname = "output";
		      do_decoding(&decInfo);	// calling do-decoding function for performing decoding operation
	              printf("\n\t\t::: Decoding Completed :::\n");
		      break;
	    default: printf("\nInvalid Choice(Enter e to perform encoding or d for decoding)\n");
		     break;

	}

	printf("\nDo you want to continue(1-Y/0-N): ");
	scanf("%d", &i);
	printf("\n");
    }


    return 0;

}




/*

   print manu
   1. encode
   2. decode

   read the choice from the user

   based on choice do operation

   entered 1 -> do encoding
   entered 2 - > do decoding

   if encoding selected, 
   create structure variable
   store the filenames(i/p, data, o/p file) in structure members
   call doencoding function




 */
