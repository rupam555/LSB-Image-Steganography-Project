#include <stdio.h>		// header file for standard input & output
#include <string.h>		// header file for string operations
#include "encode.h"		// custom header file for encoding function declarations
#include "types.h"		// custom header file for all types(like enum) used in project
#include "common.h"		// custom header file that contains common codes needed for project


/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)			// function definition of get_image_size_for_bmp()		
{
    uint width, height;						// declaring variables for finding width & height of source bmp file	
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);				// using fseek(), setting the offset position to 18th byte position

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);			// using fread(), fetching 4 bytes from source img
//    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);			// using fread(), fetching next 4 bytes from source img
//    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;					// returning size of source bmp file
}


uint get_file_size(EncodeInfo *encInfo)				// function definition of get_file_size()
{
    fseek(encInfo->fptr_secret, 0, SEEK_END);			// using fseek() function, moving the offset position to end of file
   
    encInfo->secret_file_size = ftell(encInfo->fptr_secret);	// using ftell() function, storing current offset position value

//    printf("size = %d\n", encInfo->secret_file_size);

    return encInfo->secret_file_size;				// returning secret file size
}


int get_extn_size(EncodeInfo *encInfo)				// function definition of get_extn_size()
{
    encInfo->secret_file_extn = strstr(encInfo->secret_fname, ".");	// using strstr(), extracting the extension from secret filename

//    printf("extn = %s\n", encInfo->secret_file_extn);

    encInfo->secret_file_extn_size = strlen(encInfo->secret_file_extn);	// calculating the length of the file extn

//    printf("extn size = %d\n", encInfo->secret_file_extn_size);

    return encInfo->secret_file_extn_size;			// returning file extn size
}


/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)				// function definition of open_files()
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");		// using fopen(), opening source bmp img file in read mode
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)					// error checking for source bmp file whether file is able to open or not
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");			// using fopen(), opening secret text file in read mode
    // Do Error handling
    if (encInfo->fptr_secret == NULL)						// error checking for secret text file whether file is able to open or not
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");		// using fopen(), opening output bmp img file in write mode
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)					// error checking for output bmp file whether file is able to open or not
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status do_encoding(EncodeInfo *encInfo)			// function definition of do_encoding()
{

    // open all files

    if(open_files(encInfo) == e_success)		// calling open_files() to open all files
    {
	printf("\nINFO : All files are opened successfully\n");
    }
    else
    {
	printf("\nINFO : Files are not opened successfully\n");
	return e_failure;
    }

    // check capacity before encoding

    if(check_capacity(encInfo) == e_success)		// calling check_capacity() to check whether source img has enough space to encode data or not
    {
	printf("\nINFO: Required bytes are available to encode data\n");
    }
    else
    {
	printf("\nINFO: Required bytes are not available to encode data\n");
	return e_failure;
    }

    // copy bmp header

    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)	// calling copy_bmp_header() to copy whole 54 bytes of bmp header into stegged img
    {
	printf("\nINFO: Whole 54 bytes of bmp header is copied to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to write the bmp header to stego image\n");
	return e_failure;
    }

    // encode magic string

    if(encode_magic_string(MAGIC_STRING, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)	// calling encode_magic_string() to encode magic string into stegged img
    {
	printf("\nINFO: Magic String is successfully encoded to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to encode the Magic String to stego image\n");
	return e_failure;
    }


    // encode secret file extn size

    int file_extn_size = get_extn_size(encInfo);	// finding the file extn size

    if(encode_secret_file_extn_size(file_extn_size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)	// calling encode_secret_file_extn_size() to encode file extn size
    {
	
	printf("\nINFO: Secret file extension size is successfully encoded to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to encode Secret file extension size to stego image\n");
	return e_failure;
    }

    // encode secret file extn

    if( encode_secret_file_extn(encInfo->secret_file_extn, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success) // calling encode_secret_file_extn() to encode file extn
    {
	printf("\nINFO: Secret file extension is successfully encoded to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to encode Secret file extension to stego image\n");
	return e_failure;
    }

    // encode secret file size

    int file_size = get_file_size(encInfo);		// finding the file size or data size

    if( encode_secret_file_size(file_size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)	// calling encode_secret_file_size() to encode secret file size
    {
	printf("\nINFO: Secret file size is successfully encoded to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to encode Secret file size to stego image\n");
	return e_failure;
    }

    // encode secret file data

  if( encode_secret_file_data(file_size, encInfo->fptr_src_image, encInfo->fptr_secret, encInfo->fptr_stego_image) == e_success)    // calling encode_secret_file_data() to encode secret file data
    {
	printf("\nINFO: Secret file data is successfully encoded to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to encode Secret file data to stego image\n");
	return e_failure;
    }


    // copy remaining source img data to stego img

    if( copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)	// calling copy_remaining_img_data() to copy remaining all data of source img into stegged img
    {
	printf("\nINFO: All remaining data is successfully copied to stego image\n");
    }
    else
    {
	printf("\nINFO: Unable to copy all remaining data to stego image\n");
	return e_failure;
    }

}


Status check_capacity(EncodeInfo *encInfo)		// function definition of check_capacity()
{
    encInfo->magic_str_size = strlen(MAGIC_STRING);

    if( get_image_size_for_bmp(encInfo->fptr_src_image) >= ( encInfo->magic_str_size*8 + sizeof(int) + get_extn_size(encInfo)*8 + sizeof(int) + get_file_size(encInfo)*8 ))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)	// function definition of copy_bmp_header() 
{
    char img_data[54];					// taking a character array of size 54 bytes

    rewind(fptr_src_image);				// moving the offset position to 1st byte of source img 

    if(fread(img_data, 54, 1, fptr_src_image))		// using fread(), reading 54 bytes of data from source img and storing into a data buffer
    {
	fwrite(img_data, 54, 1, fptr_stego_image);	// using fwrite(), writing 54 bytes data into stegged img from data buffer
	return e_success;
    }
    else
    {
	return e_failure;
    }

}


Status encode_magic_string(char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image)		// function definition of encode_magic_string()
{
    //STEP-1: call encode_data_to_image() to encode magic string

    if(encode_data_to_image(magic_string, fptr_src_image, fptr_stego_image) == e_success)	// calling encode_data_to_image() to encode magic string into stegged img
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}


Status encode_secret_file_extn_size(int extn_size, FILE *fptr_src_image, FILE *fptr_stego_image)	// function definition of encode_secret_file_extn_size()
{
    //STEP-1: Read 32 bytes of data from source file and store into in temporary array;
    //STEP-2: call encode_size_to_lsb()
    //STEP_3: write the encoded data into stego image

    char img_data[32];					// taking char array of 32 bytes to store img data temporarily

//    printf("Extn size = %d\n", extn_size);

    if(fread(img_data, 32, 1, fptr_src_image))		// reading 32 bytes of data from src img and storing into img buffer
    {
	encode_size_to_lsb(extn_size, img_data);	// calling encode_size_to_lsb(), to encode extn size into img buffer data
	fwrite(img_data, 32, 1, fptr_stego_image);	// writing the encoded buffer data into stegged img
    }

    return e_success;

}


Status encode_secret_file_extn(char *file_extn, FILE *fptr_src_image, FILE *fptr_stego_image)		// function definition of encode_secret_file_extn()
{
    //STEP-1: call encode_data_to_image()

    if(encode_data_to_image(file_extn, fptr_src_image, fptr_stego_image) == e_success) 		// calling encode_data_to_image(), to encode file extn into stegged img
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

Status encode_secret_file_size(uint file_size, FILE *fptr_src_image, FILE *fptr_stego_image)		// function definition of encode_secret_file_size()
{
    //STEP-1: Read 32 bytes of data from source file and store into in temporary array;
    //STEP-2: call encode_size_to_lsb()
    //STEP_3: write the encoded data into stego image

    char img_data[32];						// taking char array of 32 bytes to store img data temporarily

    if(fread(img_data, 32, 1, fptr_src_image))			// reading 32 bytes of data from src img and storing into img buffer
    {
	encode_size_to_lsb(file_size, img_data);		// calling encode_size_to_lsb(), to encode extn size into img buffer data
	fwrite(img_data, 32, 1, fptr_stego_image);		// writing the encoded buffer data into stegged img
    }

    return e_success;
}


Status encode_secret_file_data(int secret_file_size, FILE *fptr_src_image, FILE *fptr_secret, FILE *fptr_stego_image)		// function definition of encode_secret_file_data()
{
    //STEP-1: rewind the secret file
    //STEP-2: Read the entire data from secret file and store into temp_arr
    //STEP-3: call encode_data_to_image()

    char temp_arr[secret_file_size];		// taking a char array to store secret file data temporarily

    rewind(fptr_secret);			// moving the offset position to beginning of file or 1st byte

//    printf("%ld\n", ftell(fptr_src_image));

    fread(temp_arr, secret_file_size, 1, fptr_secret);		// reading bytes of secret_file_size from secret file and storing into temporary buffer

    if(encode_data_to_image(temp_arr, fptr_src_image, fptr_stego_image) == e_success)		// calling encode_data_to_image() to encode buffer data into stegged img
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }

}

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)		// function definition of copy_remaining_img_data()
{
    char ch;						// taking a char variable

    while(fread(&ch, 1, 1, fptr_src_image) > 0)	 	// using fread(), reading one byte from src img and storing into char variable
    {
	fwrite(&ch, 1, 1, fptr_stego_image);		// writing the char data into stegged img
    }

    return e_success;
}


Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image)		// function definition of encode_data_to_image()
{

    //STEP-1: Read 8 bytes of data from source img and store into a image_data
    //STEP-2: Fetch 1 byte from data
    //STEP-3: Encode part -> call encode_byte_to_lsb()
    //STEP-4: Write the encoded data into stego_image
    //STEP-5: Repeat for length of the data times

    char image_data[8];			// taking a char array of size 8 bytes to store img data

    int length = strlen(data);		// finding the length of the data

    for(int i=0;i<length;i++)		// running loop length times
    {
	if(fread(image_data, 8, 1, fptr_src_image))		// reading 8 bytes of data from src img and storing into temporary img buffer
	{
	    encode_byte_to_lsb(data[i], image_data);		// calling encode_byte_to_lsb() to encode byte from actual data to the temp img buffer
	    fwrite(image_data, 8, 1, fptr_stego_image);		// writing the encoded buffer data into stegged img
	}
    }

    return e_success;

}


Status encode_byte_to_lsb(char ch, char *image_data)		// function definition of encode_byte_to_lsb()
{
    //STEP-1: clear the LSB bit of image_data[0]
    //STEP-2: get a bit from ch
    //STEP-3: Replace -> STEP-1 | STEP-2;
    //STEP 4: Repeat for 8 times

    for(int i=0;i<8;i++)	// running loop for 8 times to encode 8 bits into lsb of stegged img
    {
	image_data[i] = ( ((ch & (1 << i)) >> i) | (image_data[i] & (~1)) );	// get a bit from character and replace a bit from img buffer and putting the data into img buffer
    }

    return e_success;
}


Status encode_size_to_lsb(int size, char *temp_arr)		// function definition of encode_size_to_lsb()
{
    //STEP-1: Clear a bit from arr[0]
    //STEP-2: Get a bit from size
    //STEP-3: Replace -> STEP-1 | STEP-2
    //STEP-4: Repeat for 32 times

    for(int i=0;i<32;i++)		// running loop for 32 times to encode 32 bits for 4 bytes of size
    {
	temp_arr[i] = ( ((size & (1 << i)) >> i) | (temp_arr[i] & (~1)) );	// get a bit from size and replace a bit from img buffer and putting the data into img buffer
    }

    return e_success;

}

