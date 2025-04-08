#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>
#include<unistd.h>
/* Function Definitions */
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

Status do_encoding(EncodeInfo *encInfo)
{
     if(open_files(encInfo)==e_success)
            {  
                sleep(1);
                printf("\t\t2️⃣ Files open successfully\n");
            }
            else
            {
                printf("Failed in files open\n");
                return e_failure;
            }
                if(check_capacity(encInfo)==e_success)
                {
                sleep(1);
                printf("\t\t3️⃣ Success on calculation of sizes\n");
                }
                else
                {
                    printf("Failed in calculation of sizes\n");
                    return e_failure;
                }
                if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                    { 
                    sleep(1);
                    printf("\t\t4️⃣ Copied Bmp header success\n");
                    }
                    else
                    {
                        printf("Failed in encoding bmp header\n");
                        return e_failure;
                    }
                    if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                        {
                        printf("\t\t5️⃣ Magic string encoded done\n");
                        } 
                        else
                        {
                            printf("Failed in encoding in magic string\n");
                        }
                         if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo)==e_success)
                        {
                            sleep(1);
                            printf("\t\t6️⃣ Secret file extension size encoded successfully\n");
                        }
                        else
                        {
                            printf("Failed in encoding secret file extension size\n");
                            return e_failure;
                        }
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
                        {
                            sleep(1);
                            printf("\t\t7️⃣ Secret file extension encoded successfully\n");
                        }
                        else
                        {
                            printf("Failed in encoding secret file extension\n");
                            return e_failure;
                        }
                        if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                        {
                            sleep(1);
                            printf("\t\t8️⃣ Secret file size encoded successfully\n");
                        }
                        else
                        {
                            printf("Failed in encoding secret file\n");
                            return e_failure;
                        }
                        if(encode_secret_file_data(encInfo)==e_success)
                        {
                            sleep(1);
                            printf("\t\t9️⃣ Secret file data encoded successfully\n");
                        }
                        else
                        {
                            printf("Failed in encoding the secret file data\n");
                            return e_failure;
                        }
                        if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                        {
                            sleep(1);
                            printf("\t\t🔟Remaining image data copied successfully\n");
                            return e_success;
                        }
}
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r"); 
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r+");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {  
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }
    // Stego Image filefptr_dest_image
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{    
   
    if(encode_data_to_image(MAGIC_STRING,strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    { 
        
        return e_success;
    }
}
Status encode_data_to_image(char* str,int size,FILE* fptr_src_image,FILE* fptr_dest_image)
{
    int i=0;
    char image_dat[8];
    for( i=0;i<size;i++)
    {  
         fread(image_dat,sizeof(char),8,fptr_src_image);
     //   printf("---->%c\n",str[i]);
        encode_byte_to_lsb(str[i],image_dat);
        fwrite(image_dat,sizeof(char),8,fptr_dest_image);
    }
    return e_success;
}
Status  encode_byte_to_lsb(char data,char* imag_buff)
{
    for(int i = 0; i<8; i++)
    {
        //printf("Before value: data = %x, image_buffer[%d] = %x \n", data, i, image_buffer[i]);
        imag_buff[i] = (imag_buff[i] & (~1)) | ((data & (1<<(7-i)))>>(7-i));
        //printf("After value: data = %x, image_buffer[%d] = %x \n", data, i, image_buffer[i]);
    }
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{  
     char buffer[54];
    rewind(fptr_src_image);
    fread(buffer,sizeof(char),54,fptr_src_image);
    fwrite(buffer,sizeof(char),54,fptr_dest_image); //if we want to print it will print unprintable characters
    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
   encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
   encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
   if(encInfo->image_capacity>(54+(strlen("MAGIC_STRING")*8)+(4*8)+strlen(encInfo->extn_secret_file)*8+(4*8)+(encInfo->size_secret_file*8)))
   {
    return e_success;
   }
   return e_failure;
}
uint get_file_size(FILE *fptr_sec)
{
    fseek(fptr_sec,0,SEEK_END);
    long size=ftell(fptr_sec);
   fseek(fptr_sec,0,SEEK_SET);
    return size;
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte --
    fseek(fptr_image, 18, SEEK_SET);
    // Read the width (an int) Bytes 18-21: Width of the image (4 bytes).
    fread(&width, sizeof(int), 1, fptr_image);
    printf("\t\twidth = %u\n", width);
    fread(&height, sizeof(int), 1, fptr_image);
    printf("\t\theight = %u\n", height);
    // Return image capacity
    return width * height * 3;
}

Status encode_secret_file_extn_size(long file_extn, EncodeInfo *encInfo)
{  
    char buff[32];
    fread(buff,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_extn,buff);
    fwrite(buff,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{ 
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image)!=e_success)
    {
        printf("ERROR :");
        return e_failure;
    }
    return e_success;
}
Status encode_secret_file_size(long file_size,EncodeInfo* encInfo)
{   
    char buff[32];
    fread(buff,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buff);
    fwrite(buff,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_size_to_lsb(int size, char *buffer)
{
    // uint mask = 1 << 31, i;
    // for (i = 0; i < 32; i++)
    // {
    //     buffer[i] = (buffer[i] & 0xFFFFFFFE) | ((size & mask) >> (31 - i));
    //     mask = mask >> 1;
    // }
    for(int i=0;i<32;i++)
    {
        int get=size & (1<< (31-i));
        int clear =buffer[i] &(~1);
        buffer[i]=clear | (get>>(31-i));
    }
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo) { 
    rewind(encInfo->fptr_secret);
    //fseek(encInfo->fptr_secret, 0, SEEK_SET);
    char str[encInfo->size_secret_file+1];
    fread(str,sizeof(char),encInfo->size_secret_file, encInfo->fptr_secret);
    str[encInfo->size_secret_file]='\0';
    encode_data_to_image(str,encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_dest_image)
{
  char ch;
    while((fread(&ch, sizeof(char), 1, fptr_src_image)) >0)
    {
        fwrite(&ch, sizeof(char), 1,fptr_dest_image);
    }
    return e_success;
}
