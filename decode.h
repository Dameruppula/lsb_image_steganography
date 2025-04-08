#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname; // store the src image name
    FILE *fptr_src_image; // store the src image

    long secret_data_size;



    // /* Secret File Info */
    char *secret_fname; // store the secret file name
    FILE *fptr_secret; // store the secret file pointer
    uint extn_size; // to store the extensions size
    char *extn_secret_file;// store the extensions of given secret file 
    // char secret_data[MAX_SECRET_BUF_SIZE]; // store the data for extern 
    long size_secret_file;//store the size of secret file 

}DecodeInfo;

/* Encoding function prototype */

OperationType check_operation_type(char *argv[]);
Status_d open_files_d(DecodeInfo *decInfo);
Status_d open_secret_file(DecodeInfo*decInfo);
Status_d read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

Status_d do_decoding(DecodeInfo *decInfo);

Status_d decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

Status_d decode_data_from_image(char *data, int size, DecodeInfo* decInfo);

Status_d decode_byte_from_lsb(char *size,char *buffer);

Status_d decode_secret_file_extn_size(DecodeInfo *decInfo);

Status_d decode_size_from_lsb(int *size,char* buffer);

Status_d decode_secret_file_extn(DecodeInfo* decInfo);

Status_d decode_secret_data_size(DecodeInfo *decInfo);

Status_d decode_secret_file_data(DecodeInfo* decInfo);

//Status_d deopen_secret(DecodeInfo *decInfo);

#endif
