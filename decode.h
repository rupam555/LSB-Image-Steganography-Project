#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding stego image and extracting secret data 
*/

typedef struct _DecodeInfo
{
    /* magic string */
    char magic_str[16];				// string to store decoded magic stirng

    /* Stego Image/Encoded Image Info*/	
    char *stego_image_fname;			// char pointer to store the stegged img filename
    FILE *fptr_stego_image;			// file pointer for stegged img

    /* secret file Info */
    int secret_file_extn_size;			// integer to store decoded file extension size
    char secret_file_extn[10];			// string to store decoded file extension
    int secret_file_data_size;			// integer to store decoded file data size
    char secret_file_data[100];			// string to store decoded file data

    /* Output decoded file Info */
    char output_file_fname[20];			// string to store the output filename
    FILE *fptr_decoded_file;			// file pointer for output file
    
} DecodeInfo;	


/* Decoding function prototype */

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointer for i/p ot stegged img file */
Status open_source_file(DecodeInfo *decInfo);

/* Get File pointer for decoded data output file */
Status open_output_file(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(char *magic_str, FILE *fptr_stego_image);

/* Decode secret file extenstion size*/
Status decode_secret_file_extn_size(int *extn_size, FILE *fptr_stego_image);

/* Decode secret file extenstion */
Status decode_secret_file_extn(int extn_size, char *file_extn, FILE *fptr_stego_image);

/* Decode secret file/data size */
Status decode_secret_file_size(int *data_size, FILE *fptr_stego_image);

/* Decode secret file data*/
Status decode_secret_file_data(int data_size, char *data, FILE *fptr_stego_image);

/* Decode function, which does the real decoding from stegged img and extract data */
Status decode_image_to_data(int size, char *data, FILE *fptr_stego_image);

/* Decode a byte from LSB of stegged image */
Status decode_lsb_to_byte(char *data, char *ch);

/* Decode int size from LSB of stegged image */
Status decode_lsb_to_size(int *size, char *temp_arr);


#endif
