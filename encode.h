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
    /*Beautiful.bmp*/
    /* Source Image info */
    char *src_image_fname;/*Store the src_image_name*/
    FILE *fptr_src_image;/*To store the src_image itself*/
    uint image_capacity;/*TO store the size of your source image*/
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];/*To store the data*/

    /* Secret File Info */
    char *secret_fname;/*TO store the secret_file_name*/
    FILE *fptr_secret;/*store the secret file pointer*/
    char *extn_secret_file;/*store the extn of given secret file*/
    //char extn_secret_file_size[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];/*store the data*/
    long size_secret_file;/*size of the secret file*/

    /* Stego Image Info */

    char *stego_image_fname;/*store the output file name*/
    FILE *fptr_stego_image;/*store the output.bmp file pointer*/

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */ 
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr_secret);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

Status encode_secret_file_extn_size( long file_extn, EncodeInfo *encInfo); 
/* Encode secret file extenstion */
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo); 

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

Status encode_size_to_lsb(int size, char *buffer);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_dest_image);
// Status encode_to_lsb(char*,char*);
/* Encode a byte into LSB of image data array */

Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest_image);
#endif
