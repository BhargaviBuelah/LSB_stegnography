/*
Name:N.Bhargavi
Date:28/09/24
Description:Stegnograhy Project
 */
#include <stdio.h>
#include <unistd.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include<stdlib.h>

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp"))/* validate encodeed file is .bmp or not */
    {
        decInfo->src_image_fname=argv[2];
        //printf("%s",decInfo->src_image_fname);
    }
    else
    {
        printf("\033[0;31m Error ❌:Dificulty in checking source file extension in .bmp.\033[0m\n");   // Red 
        return e_failure;
    }
    if(argv[3]!=NULL)       //if not provided output file
    {
        strcpy(decInfo->secret_fname,(strtok(argv[3],".")));
    }
    else
    {
        strcpy(decInfo->secret_fname,"output");
        //printf("%s \n",decInfo->secret_fname);
    }
    
    //printf("%s",decInfo->secret_fname);
    return e_success;
}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    printf("\033[0;35mOpen files proccess started.\033[0m\n");
    sleep(1);
    if(open_decode_files(decInfo)==e_success)//opening files
    {
        printf("\033[0;32mFiles opend successfully.\033[0m\n"); // Green
        sleep(1);
        printf("\033[0;35mDecoding magic string..\033[0m\n");
        sleep(1);
        if(decode_magic_string(decInfo)==e_success)                 /*decoing magic string*/
        {
            printf("\033[0;32mMagic String Decoded Successfully.\033[0m\n");
            printf("\033[0;35mDecoding Secret file extension Size..\033[0m\n");     /*Decoding Secret file extension Size*/
            sleep(1);
            if(decode_secret_file_exten_size(decInfo)==e_success)
            {
                printf("\033[0;32mSecret file extension Size Decoded Successfully.\033[0m\n");
                printf("\033[0;35mDecoding Secret file extension..\033[0m\n");
                sleep(1);
                //printf("Size is %d\n",decInfo->secret_file_size);
                if(decode_secret_file_exten(decInfo)==e_success)                    /*Decoding Secret file extension*/
                {
                    printf("\033[0;32mSecret file extension Decoded Successfully.\033[0m\n");
                    printf("\033[0;35mDecoding Secret file size..\033[0m\n");
                    sleep(1);
                    if(decode_secret_file_data_size(decInfo)==e_success)            /*Decoding Secret file size*/
                    {
                        printf("\033[0;32mSecret file size Decoded successfully .\033[0m\n");
                        printf("\033[0;35mDecoding Secret data..\033[0m\n");
                        sleep(1);
                        if(decode_secret_file_data(decInfo)==e_success)         /*Decoding Secret data.*/
                        {
                            printf("\033[0;32mSecret data Decoded Successfully.\033[0m\n");
                            fclose(decInfo->fptr_secret);
                            return e_success;
                        }
                        else
                        {
                            printf("\033[0;31mError ❌:The Secret Data cannot be Decoded.\033[0m\n");
                            return e_failure;                                         
                        }
                    }
                    else
                    {
                        printf("\033[0;31mError ❌:The secret file size cannot be decoded..\033[0m\n");
                        return e_failure;                                         
                    }
                }
                else
                {
                    printf("\033[0;31mError ❌:Secret File Extension is cannot be decoded.\033[0m\n");
                    return e_failure;                                         
                }               
            }
            else
            {
                printf("\033[0;31mError ❌:Secret File Extension size cannot be Decoded.\033[0m\n");
                return e_failure;                                         
            }
        }
        else
        {
            printf("\033[0;31mError ❌:Magic String cannot be decoded.\033[0m\n"); 
            return e_failure;
        }
    }
    else
    {
        printf("\033[0;31mError ❌::Dificulty in opening files \033[0m\n"); 
        return e_failure;
    }
}
/*decode secret file */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    int size=decInfo->secret_file_size;
    char * str=decode_data_from_image(size,decInfo);
   // printf("message:: %s",str);       //for checking
    fprintf(decInfo->fptr_secret,"%s",str);
    return e_success;
}

/*decode secret file  size*/
Status decode_secret_file_data_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    fread(image_buffer,32,1,decInfo->fptr_src_image);
    long size=decode_size_from_lsb(sizeof(int),image_buffer);
    decInfo->secret_file_size=size;
   // printf("\t\tDecoded size of file is %ld\n",decInfo->secret_file_size);    //for checking
    return e_success;
}

/*decode secret file extension*/
Status decode_secret_file_exten(DecodeInfo *decInfo)
{    
   // printf("%s\n",decInfo->secret_fname);
    char * str=decode_data_from_image(decInfo->secret_file_exten_size,decInfo);
   // printf("extension:: %s\n",str);
    strcat(decInfo->secret_fname,str);      //concatinate the file name and extension
 // printf("output file name %s\n",decInfo->secret_fname);
   //opening file in write mode
   printf("\033[0;34mSecret message file:: %s\n\033[0m",decInfo->secret_fname);
   decInfo->fptr_secret=fopen(decInfo->secret_fname,"w");
   if(decInfo->fptr_secret==NULL)
   { 
        printf("\033[0;31mError ❌:Unable to open output file.\033[0m\n ");
        return e_failure;
   }
   else
   { 
        printf("\033[0;32mSecret File openned successfully.\033[0m\n");

   }
    return e_success;
}

/*decode secret file extension size*/
Status decode_secret_file_exten_size(DecodeInfo *decInfo)
{   
    char image_buffer[32];
    fread(image_buffer,32,1,decInfo->fptr_src_image);
    int size=decode_size_from_lsb(sizeof(int),image_buffer);
    decInfo->secret_file_exten_size=size;
    //printf("\t\tDecoded size is %d\n\n",decInfo->secret_file_exten_size);     //for checking
    
    return e_success;
}
/* decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image,54,SEEK_SET);     // Skip the BMP header
    //16 bytes we need to decode magic_string is"#*" 2 bytes:
    char *str=decode_data_from_image(2,decInfo);
    //printf("Decoded magic string: %s\n", str); 
    if(!strcmp(str,"#*"))
    {
        return e_success;
    }
    return e_failure;
    
}
char * decode_data_from_image(int size,DecodeInfo *decInfo)
{
    char image_buffer[8];
    char *decoded_string=malloc((size)*sizeof(char)); // Assuming the magic string length is 16 characters(bcz #*)
    int i ;
    for(i= 0;i<size;i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_src_image); // Read 8 bytes  
        // Print the image buffer in a readable way for checking purpose
       /* printf("Image buffer: ");
        for (int j = 0; j < 8; j++) {
            printf("%02x ", (unsigned char)image_buffer[j]); // Print as hex
        }
        printf("\n");*/
        decoded_string[i] = decode_byte_from_lsb(image_buffer); // Decode from LSB
       // printf("\t\tDecoded Byte: %c\n", decoded_string[i]); 
    } 
    decoded_string[i] = '\0'; // Null-terminate the string
  // printf("\t\tDecoded DATAAAA: %s\n", decoded_string);  // Output the decoded string for checking
    return decoded_string;
}
char decode_byte_from_lsb(char *image_buffer)
{
    char decoded_byte = 0; // Variable to hold the reconstructed byte   
    for(int i=0;i<8;i++)
    {
        // Shift left to make space for the next bit
      char bit = 0;   
        // Add the current LSB
       bit = (image_buffer[i] & 1);   
       bit=bit<<(i);
      // printf(" %d ",bit);        //checking each decoded bit
        decoded_byte=decoded_byte|bit;      
    }
   //printf("character is %c\n\n",decoded_byte);   //checking each decoded charecter(8bits)
    return decoded_byte;
}
int decode_size_from_lsb(int size,char * Image_buffer)
{
    int decoded_byte=0;
    for(int i=0;i<32;i++)
    {
        int bit=(Image_buffer[i]&1);   
        bit=bit<<(i);
        decoded_byte=decoded_byte|bit; 
    }
    
  //printf("size is %d\n\n",decoded_byte); 
    return decoded_byte;
}
//opening encoded file
Status open_decode_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");    //perror():::it will display actual error
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    // No failure return e_success
    return e_success;
}