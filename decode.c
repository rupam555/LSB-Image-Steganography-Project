#include <stdio.h>              // header file for standard input & output
#include <string.h>             // header file for string operations
#include "decode.h"             // custom header file for decoding function declarations
#include "types.h"              // custom header file for all types(like enum) used in project
#include "common.h"             // custom header file that contains common codes needed for project


Status open_source_file(DecodeInfo *decInfo)		// function definition of open_source_file()
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");		// opening the stego img file(source file here for decoding) in read mode

    if(decInfo->fptr_stego_image == NULL)		// if fopen returns NULL that means file is not present
    {
	perror("fopen");

	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);		// using fprintf, printing error message on standard error stream

	return e_failure;				// return e_failure and stop the program
    }

    return e_success;					// return e_success when file opened successfully
}


Status open_output_file(DecodeInfo *decInfo)		// function definition of open_output_file()
{
    decInfo->fptr_decoded_file = fopen(decInfo->output_file_fname, "w");	// opening the output file in write mode to write decoded secret data

    if(decInfo->fptr_decoded_file == NULL)		// if fopen returns NULL that means file is not present	
    {
	perror("fopen");

	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_file_fname);		// using fprintf, printing error message on standard error stream

	return e_failure;
    }

    return e_success;					// return e_success when file opened successfully
}


//////////////////////////////////////////   do-decoding  ///////////////////////////////////////////////


Status do_decoding(DecodeInfo *decInfo)			// function definition of do_decoding()
{

    // open source file or stego img file

    if(open_source_file(decInfo) == e_success)		// calling open_source_file() to open the source file/stegged image
    {
	printf("\nINFO: Source file is opened successfully\n");
    }
    else
    {
	printf("\nERROR: Unable to open Source file\n");
	return e_failure;
    }


    /* decode magic string and check whether decoded magic string is matching with the expected one or not
       if not matched we shall not proceed with the decoding, will stop there itself */


    if(decode_magic_string(decInfo->magic_str, decInfo->fptr_stego_image) == e_success)		// calling decode_magic_string() to decode the magic string/password from stegged img
    {
	printf("\nINFO: Magic string is decoded successfully\n");
//	printf("Magic string = %s\n", decInfo->magic_str);
	if(strcmp(decInfo->magic_str, MAGIC_STRING) == 0)					// checking whether the decoded magic string is matched with the actual one or not
	{
	    printf("\nINFO: Decoded Magic string is matched with the expected one\n");
	}
	else
	{
	    printf("\nERROR: Decoded Magic string not matched with the expected one\n");
	    return e_failure;
	}
    }
    else
    {
	printf("\nINFO: Unable to decode the Magic string\n");
	return e_failure;
    }


    // decode secret file extension size

    decInfo->secret_file_extn_size=0;			// assigning 0 to secret_file_extn_size variable 

    if(decode_secret_file_extn_size(&decInfo->secret_file_extn_size, decInfo->fptr_stego_image) == e_success)		// calling decode_secret_file_extn_size(), to decode extn size
    {
	printf("\nINFO: Secret file extension size is decoded successfully\n");
//	printf("secret file extn size = %d\n", decInfo->secret_file_extn_size);
    }
    else
    {
	printf("\nERROR: Unable to decode the secret file extension size\n");
	return e_failure;
    }

    // decode secret file extension 

    if(decode_secret_file_extn(decInfo->secret_file_extn_size ,decInfo->secret_file_extn, decInfo->fptr_stego_image) == e_success)     // calling decode_secret_file_extn() to decode extension using extn size
    {
	printf("\nINFO: Secret file extension is decoded successfully\n");
//	printf("extn = %s\n", decInfo->secret_file_extn);
    }
    else
    {
	printf("\nERROR: Unable to decode the secret file extension\n");
	return e_failure;
    }

    // concat the file extension with the output file name

    strcpy(decInfo->output_file_fname, "output");		        // copying the "output" source string into output_file_fname destination string

    strcat(decInfo->output_file_fname, decInfo->secret_file_extn);	// concat output_file_fname string with decoded file extension string to form the output filename with proper extension

//    printf("output filename = %s\n", decInfo->output_file_fname);


    // open output file

    if(open_output_file(decInfo) == e_success)				// calling open_output_file() to open the output file to be written with the decoded secret data
    {
	printf("\nINFO: Output file is opened successfully\n");
    }
    else
    {
	printf("\nERROR: Unable to open Output file\n");
	return e_failure;
    }


    // decode secret file size

    decInfo->secret_file_data_size=0;		// assigning 0 to secret_file_data_size variable

    if(decode_secret_file_size(&decInfo->secret_file_data_size, decInfo->fptr_stego_image) == e_success)	// calling decode_secret_file_size() to decode file size or secret data size
    {
	printf("\nINFO: Secret file size is decoded successfully\n");
//	printf("Secret file size = %d\n", decInfo->secret_file_data_size);
    }
    else
    {
	printf("\nERROR: Unable to decode Secret file size\n");
	return e_failure;
    }


    // decode secret file data

    if(decode_secret_file_data(decInfo->secret_file_data_size, decInfo->secret_file_data, decInfo->fptr_stego_image) == e_success)	// calling decode_secret_file_data() to decode secret data
    {
	printf("\nINFO: Secret file data is decoded successfully\n");
//	printf("Secret file data = %s\n", decInfo->secret_file_data);
    }
    else
    {
	printf("\nERROR: Unable to decode Secret file data\n");
	return e_failure;
    }

    // write the decoded secret data into output file

    if(fwrite(decInfo->secret_file_data, decInfo->secret_file_data_size, 1, decInfo->fptr_decoded_file))	// using fwrite(), writing the decoded secret data into output file
    {
	printf("\nINFO: Decoded secret file data is written to output file successfully\n");
    }
    else
    {
	printf("\nERROR: Unable to write secret file data into output file\n");
	return e_failure;
    }


    // close the output file pointer 
    fclose(decInfo->fptr_decoded_file);
    

}


///////////////////////////////////////////////////////////////////////////////////////////



Status decode_magic_string(char *str, FILE *fptr_stego_image)			// function definition of decode_magic_string()
{
    // STEP-1: move the offset to 54 byte of stegged img 
    // STEP-2: find the length of the magic string
    // STEP-3: call decode_image_to_data() to decode magic string using that length
   
    fseek(fptr_stego_image, 54, SEEK_SET);					// setting the offset position to 54 bytes to start reading bytes after the bmp header that takes 54 bytes(0 to 53)

    int len = strlen(MAGIC_STRING);						// calculating the length of the Magic string so as to know how many bytes to decode for magic string

    if(decode_image_to_data(len, str, fptr_stego_image) == e_success)		// calling decode_image_to_data(), to decode the magic string
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


Status decode_secret_file_extn_size(int *extn_size, FILE *fptr_stego_image)	// function definition of decode_secret_file_extn_size()
{
    //STEP-1: read 32 bytes from stegged img and store into data buffer
    //STEP-2: call decode_lsb_to_size() to decode extn size
   
    char img_data[32];		// taking a data buffer of size 32 bytes to decode the 32 bits extn size

    if(fread(img_data, 32, 1, fptr_stego_image))	// using fread(), reading 32 bytes of data from stegged img and storing into the data buffer
    {
	decode_lsb_to_size(extn_size, img_data);	// calling decode_lsb_to_size(), with extn_size variable to store the extension size and data buffer
    }

    return e_success;
}

Status decode_secret_file_extn(int extn_size, char *file_extn, FILE *fptr_stego_image)		// function definition of decode_secret_file_extn()
{
    //STEP-1: call decode_image_to_data() to decode file extn with previouly decoded extn size

    if(decode_image_to_data(extn_size, file_extn, fptr_stego_image) == e_success)	// calling decode_image_to_data() with the arguments needed for decoding secret file extension
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


Status decode_secret_file_size(int *file_size, FILE *fptr_stego_image)		// function definition of decode_secret_file_size()
{
    //STEP-1: read 32 bytes from stegged img and store into data buffer
    //STEP-2: call decode_lsb_to_size() to decode file/data size

    char img_data[32];						// declaring a char array that acts as a image data buffer to store bytes of data from stegged img temporarily

    if(fread(img_data, 32, 1, fptr_stego_image))		// using fread(), reading 32 bytes of data from the stegged img and storing into data buffer
    {
	decode_lsb_to_size(file_size, img_data);		// calling decode_lsb_to_size(), with file_size variable to store the size of the file and img_data buffer
    }

    return e_success;
}


Status decode_secret_file_data(int data_size, char *file_data, FILE *fptr_stego_image)		// function definition of decode_secret_file_data()
{
    //STEP-1: call decode_image_to_data() to decode file data with previouly decoded data size

    if(decode_image_to_data(data_size, file_data, fptr_stego_image) == e_success)		// calling decode_image_to_data() with the arguments needed for decoding secret file data
    {
    	return e_success;		// returning e_success on successful completion
    }
    else
    {
	return e_failure;		// returning e_failure on unsuccessful operation
    }
}


Status decode_image_to_data(int size, char *dec_data, FILE *fptr_stego_image)		// function definition of decode_image_to_data()
{
    //STEP-1: Read 8 bytes of data from stegged img and store into img data buffer
    //STEP-2: call decode_lsb_to_byte() to decode one byte from lsb of stegged img
    //STEP-3: store the decoded byte into a char array
    //STEP-4: Repeat the process for size times

    char image_data[8];    // declaring a char array that acts as a image data buffer to store bytes of data from stegged img temporarily

    char ch;							// declaring a char variable

    for(int i=0;i<size;i++)					// running the loop size times to fetch that many characters
    {
	ch=0;							// assigning 0 to the char variable
	if(fread(image_data, 8, 1, fptr_stego_image))		// using fread(), reading 8bytes of data from the stegged img and storing into image_data_buffer
	{
	    decode_lsb_to_byte(image_data, &ch);		// calling decode_lsb_to_byte(), with the image_data_buffer along with the char variable store the decoded character
	    dec_data[i] = ch;					// storing the decoded character into a string using indexing
	}
    }

    dec_data[size] = '\0';

    return e_success;						// returning e_success on successful completion
}

Status decode_lsb_to_byte(char *data, char *ch)			// function definition of decode_lsb_to_byte()
{
    //STEP-1: Get a bit from data[index]
    //STEP-2: Put into char variable
    //STEP-3: Repeat for 8 times to decode 8 bits for 1 byte

    for(int i=0;i<8;i++)					// running loop 8 times for decoding each encoded character (char is of 1 bytes so 8 bits needs to be decoded)
    {  
	*ch = (((data[i] & 1) << i) | *ch);			// get one bit from the data buffer and appending/putting into the character variable(pass by reference used)
    }

    return e_success;						// returning e_success on successful completion
}


Status decode_lsb_to_size(int *size, char *image_data)		// function definition of decode_lsb_to_size()
{
    //STEP-1: Get a bit from img_data[index]
    //STEP-2: Put into size variable of int type
    //STEP-3: Repeat for 32 times to decode 32 bits for 4 bytes

    for(int i=0;i<32;i++)					// running loop 32 times for decoding size (size is of type int and int has 32 bits encoded, so 32 bits needs to be decoded)
    {  
	*size = (((image_data[i] & 1) << i) | *size);		// get one bit from the image data buffer and appending/putting into a size variable(pass by reference used)
    }

    return e_success;						// returning e_success on successful completion
}

