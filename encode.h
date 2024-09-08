#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Magic String Info */
    int magic_str_size;				// int variable to store magic string size

    /* Source Image info */			
    char *src_image_fname;			// char pointer to store source image filename
    FILE *fptr_src_image;			// file pointer for source img
//    uint image_capacity;			
//    char image_data[MAX_IMAGE_BUF_SIZE];	
//    uint bits_per_pixel;

    /* Secret File Info */
    char *secret_fname;				// char pointer to store secret text filename
    FILE *fptr_secret;				// file pointer for secret file
    int secret_file_extn_size;			// int variable to store size of secret file extn
    char *secret_file_extn;			// char pointer to store secret file extn
    uint secret_file_size;			// unsigned int type variable to store secret file/data size
//    char secret_data[MAX_SECRET_BUF_SIZE];	

    /* Stego Image Info */			
    char *stego_image_fname;			// char pointer to store stegged img filename
    FILE *fptr_stego_image;			// file pointer for stegged img
    
} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
//OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(EncodeInfo *encInfo);

/* Get file extension size */
int get_extn_size(EncodeInfo *encInfo);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode secret file size */
Status encode_secret_file_size(uint file_size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode secret file data*/
Status encode_secret_file_data(int file_size, FILE *fptr_src_image, FILE *fptr_secret, FILE *fptr_stego_image);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Encode int size into LSB of image data array */
Status encode_size_to_lsb(int data, char *temp_arr);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/* Encode secret file extenstion size*/
Status encode_secret_file_extn_size(int file_extn_size, FILE *fptr_src_image, FILE *fptr_stego_image);

#endif
