/*Project Documentation
Author:Dameruppula Abhinay
Project Title:LSB Image Steganography
Date of submission:30/12/2024
Sample input:./a.out -e beautiful.bmp secret.txt stego.bmp 
🔸🔸🔸🔸You selected encoding type ✍️🔸🔸🔸🔸

                source image name is :beautiful.bmp
                source file name is secret.c
                stego image name is stego.bmp
                1️⃣ Arguments validated successfully
                2️⃣ Files open successfully
                width = 1024
                height = 768
                3️⃣ Success on calculation of sizes
                4️⃣ Copied Bmp header success
                5️⃣ Magic string encoded done
                6️⃣ Secret file extension size encoded successfully
                7️⃣ Secret file extension encoded successfully
                8️⃣ Secret file size encoded successfully
                9️⃣ Secret file data encoded successfully
                🔟Remaining image data copied successfully
         🔸🔸🔸🔸Encoding successfully completed🤝🔸🔸🔸🔸

Sample output: ./a.out stego.bmp 
🔸🔸🔸🔸You selected decoding type🔄️ 🔸🔸🔸🔸

                1️⃣ Arguments validated successfully
                2️⃣ Open files is done success
                3️⃣ Magic string is done success
                4️⃣ Decode secret file extn size is success
                5️⃣ Decode secret file extn is success
                Secret data size = 25
                6️⃣ Decode secret file size is success
                7️⃣ Decode secret file data is success 
         🔸🔸🔸🔸Decoding successfully completed🤝 🔸🔸🔸🔸

*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include<unistd.h>
#include<stdlib.h>
#include "decode.h"
int main(int argc,char *argv[])
{   
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size;
    if(!(argc> 3 && argc<=5))
    { 
        printf("For encoding pass the arguments like:./a.out -e beautiful.bmp secret.sh\n");
        printf("for decoding ./a.out -d stego.bmp secret.sh");
        return e_failure;
    }
    if(check_operation_type(argv)==e_encode)
    {   
        sleep(1);
        printf("\t 🔸🔸🔸🔸You selected encoding type ✍️🔸🔸🔸🔸\n");
        printf("\n");
    }
    else if(check_operation_type(argv)==e_decode)
    {   
        sleep(1);
        printf("\t 🔸🔸🔸🔸You selected decoding type🔄️ 🔸🔸🔸🔸\n");
        printf("\n");
        if(read_and_validate_decode_args(argc,argv,&decInfo)==d_success)
        {    sleep(1);
            printf("\t\t1️⃣ Arguments validated successfully\n");
            if(do_decoding(&decInfo)==d_success)
            {
                printf("\t 🔸🔸🔸🔸Decoding successfully completed🤝 🔸🔸🔸🔸\n");
                printf("\n");
                return 0;
            }
        }
        else
        return 0;
    }
    else
    {
    printf("Invalid,unsupported selection\n");
    }
     if(read_and_validate_encode_args(argc,argv,&encInfo)==e_success)
    {   
         printf("\t\t1️⃣ Arguments validated successfully\n");
         if(do_encoding(&encInfo)==e_success)
         {
            printf("\t 🔸🔸🔸🔸Encoding successfully completed🤝🔸🔸🔸🔸\n");
            printf("\n");
            return 0;
         }
         else
         {
         printf("!!!Encoding failed!!!❌\n");
         return 0;
         }
    }
    else
    {
        printf("\t\tArguments validation failed❌\n");
        return 0;
    }
    return 0;
} 
OperationType check_operation_type(char* argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)
{  
    if(argc>3 && argc<=5)
    {
    
        if(strcmp(strstr(argv[2],"."),".bmp")==0)
        {
        encInfo->src_image_fname=argv[2];
        sleep(1);
        printf("\t\tsource image name is :%s\n",encInfo->src_image_fname);
        }
        else{
        return e_failure;
        }
        if(strcmp(strstr(argv[3],"."),".txt")==0 ||strcmp(strstr(argv[3],"."),".c")==0||strcmp(strstr(argv[3],"."),".sh")==0)
            {
        encInfo->secret_fname=argv[3];

        encInfo->extn_secret_file=strstr(argv[3],".");

         strcpy(encInfo->extn_secret_file, strstr(argv[3], "."));
        //encInfo->extn_secret_file[MAX_FILE_SUFFIX] = '\0';
        sleep(1);
        printf("\t\tsource file name is %s\n",encInfo->secret_fname);
            }
         else
            {
        return e_failure;
    }
    if(argv[4]==NULL)
    {
        //argv[4]="stego.bmp";
        encInfo->stego_image_fname="stego.bmp";
        sleep(1);
        printf("\t\tstego image name is %s\n",encInfo->stego_image_fname);
    }
    else if(strcmp(strstr(argv[4],"."),".bmp")==0)
        {
            encInfo->stego_image_fname=argv[4];
            sleep(1);
              printf("\t\tstego image name is %s\n",encInfo->stego_image_fname);
        }
    else if(strcmp(strstr(argv[4],"."),".bmp")!=0)
    {   
        sleep(1);
        printf("\t\tenter only '.bmp' file only\n");
        return e_failure;
    }
 return e_success; 
    }
    else
    {
        printf("\t\tEnter atleast 4 arguments❌\n");
        return 0;
        //return e_failure;
    } 
}
Status_d read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo)
{
     if(argc>=3 && argc<5)
     {
    if(strcmp((strstr(argv[2],".")),".bmp") == 0)
    {
        decInfo->src_image_fname = argv[2];
        //printf("%s\n",decInfo->src_image_fname);
    }
     else
     {
       return d_failure;
     }
         if(argv[3] == NULL)
        {
            decInfo->secret_fname = malloc(50);
            strcpy(decInfo->secret_fname,"output");
        }
        else if(strstr(argv[3],".") != NULL)
            {
                decInfo->secret_fname = argv[3];
            }
            else
            {
                decInfo->secret_fname =strtok(argv[3],"."); // 
            }
    return d_success ;
     }
     else
     {
        printf("\t\tEnter -d for decoding,and output.bmp and output(optional)❌\n");
        return d_failure;
    }
}
   