#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include "common.h"
#include "decode.h"
#include "types.h"


//open files   
Status_d open_files_d(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return d_failure;
    }
    return d_success;
}
Status_d open_secret_file(DecodeInfo*decInfo)
{
    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w+");
    if(decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR:unable to open file in writemode\n");  
        return d_failure; 
    }
    return d_success;
}
Status_d do_decoding(DecodeInfo *decInfo)
{
    if(open_files_d(decInfo) == e_success)
    {
        sleep(1);
        printf("\t\t2️⃣ Open files is done success\n");
    }
        else{
            printf("failed in opening files\n");
            return d_failure;
            }
    if(decode_magic_string(MAGIC_STRING,decInfo) == d_success)
        {
        sleep(1);
        printf("\t\t3️⃣ Magic string is done success\n");
        }
        else
        {
            printf("Failed in Decoding magic string\n");
            return d_failure;
        }

    if(decode_secret_file_extn_size(decInfo) == d_success)
    {
        sleep(1);
        printf("\t\t4️⃣ Decode secret file extn size is success\n");
    }
        else
        {
            printf("Failed in decoding secret file\n");
            return d_failure;
        }
    if(decode_secret_file_extn(decInfo) == d_success)
        {
            sleep(1);
            printf("\t\t5️⃣ Decode secret file extn is success\n");
                        //printf("%s\n", decInfo->secret_fname);
        }
        else
        {
            printf("failed in decoding secret file extn\n");
            return d_failure;
        }
     if(decode_secret_data_size(decInfo) == d_success)
        {
            sleep(1);
            printf("\t\t6️⃣ Decode secret file size is success\n");
        }
            else
            {
                printf("Failed in decoding secret file size\n");
                return d_failure;
            }

    if(decode_secret_file_data(decInfo) == d_success)
        {
            sleep(1);
            printf("\t\t7️⃣ Decode secret file data is success \n");
            return d_success;
        }  
            else
            {
                printf("Failed in decoding secret file data\n");
                return d_failure;
            }
        return d_success;
}

Status_d decode_magic_string(const char* magic_string,DecodeInfo* decInfo)
{
    fseek(decInfo->fptr_src_image,54,SEEK_SET); // skip header file size
    char buffer[strlen(MAGIC_STRING)+1];
    if(decode_data_from_image(buffer,strlen(magic_string),decInfo ) == d_success)
    {
        buffer[strlen(MAGIC_STRING)] ='\0';

        if(strcmp(buffer,MAGIC_STRING) == 0)
        {
            return d_success;
        }
        else
        {
            printf("Error! mismatch in magic string\n");
            return d_failure;
        }
    }
    else
    {
        return d_failure;
    }
}

Status_d decode_data_from_image(char* data,int size,DecodeInfo* decInfo) //
{
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(image_buffer,sizeof(char),8,decInfo->fptr_src_image);
        decode_byte_from_lsb(&data[i],image_buffer);
    }
    return d_success;
}

Status_d decode_byte_from_lsb(char *size,char* buffer)
{
    char ch =0;
    for(int i=0; i<8; i++)
    {
        int bit = (buffer[i] & 1); // extract the lsb of buffer
        
        ch = ch |(bit <<(7 - i));
    }
    *size = ch;
    return d_success;
}

Status_d decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    //printf("\nposition secret:%ld\n",ftell(decInfo->fptr_src_image));
    uint extn_size =0;

    fread(buffer,sizeof(char),32,decInfo->fptr_src_image);
    decode_size_from_lsb(&extn_size,buffer);
    decInfo->extn_size = extn_size;
   // printf("extn size:%d\n",decInfo->extn_size);

    return d_success ;
}

Status_d decode_size_from_lsb(int *size,char* buffer)
{
    for(int i =0; i < 32;i++)
    {
        int bit = (buffer [i] & 1); // extract the lsb of buffer

        *size = *size | (bit <<(31 - i)); 
    }
    return d_success;
}

Status_d decode_secret_file_extn(DecodeInfo* decInfo)
{
    char extn[decInfo->extn_size +1];
    char buffer[8];
    char output[15];

    for(int i=0;i<decInfo->extn_size;i++)
    {
        fread(buffer,sizeof(char),8,decInfo->fptr_src_image);

       decode_byte_from_lsb(&extn[i],buffer);
    }
    extn[decInfo->extn_size] = '\0';

    // printf("extn :%s\n",extn);

    strcpy(output,decInfo->secret_fname);
    strcat(output,extn);
    strcpy(decInfo->secret_fname,output);
    // printf("%s",decInfo->secret_fname);
    return d_success;
}

Status_d decode_secret_data_size(DecodeInfo *decInfo)
{
    char buffer[32];
    uint file_size = 0;
    int byte_read=fread(buffer,sizeof(char),32,decInfo->fptr_src_image);
    if(byte_read != 32)
    {
        printf("Failed to read 32 bytes for secret data size from the image\n");
        return e_failure;
    }
    decode_size_from_lsb(&file_size,buffer);
    decInfo->secret_data_size = file_size;
    printf("\t\tSecret data size = %ld\n",decInfo->secret_data_size);
    return d_success;
}

Status_d decode_secret_file_data(DecodeInfo* decInfo)
{
    if(open_secret_file(decInfo)==d_success)
    {
    char buffer[decInfo->secret_data_size + 1];
    if(decode_data_from_image(buffer, decInfo->secret_data_size, decInfo) == d_success)
    {
        buffer[decInfo->secret_data_size] = '\0';

        fwrite(buffer, sizeof(char), decInfo->secret_data_size, decInfo->fptr_secret);
    }
    return d_success;
    }
    else
    {
        printf("unable to create the secret_fname\n");
    }
}



