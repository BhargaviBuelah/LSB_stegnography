/*
Name:N.Bhargavi
Date:28/09/24
Description:Stegnograhy Project
 */
#include <stdio.h>
#include <unistd.h>  // for sleep()
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

/* Checking argument count*/
Status check_argc_count(int argc,char *argv[])
{
    if(!strcmp(argv[1],"-e"))
    {
        if(argc<=3)
            return e_failure;
    }
    else if(!strcmp(argv[1],"-d"))
    {    
        if(argc<=2)
        {
            return e_failure;
        }      
    }
    return e_success; 
}

/* Check operation type  i.e Encoding or decoding*/
OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e"))
    {
        return e_encode;
    }
    else if(!strcmp(argv[1],"-d"))
    {
        return e_decode;
    }
    return e_unsupported;
}
/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp"))/* validate source file is .bmp or not */
    {
        encInfo->src_image_fname=argv[2];
    }
    else
    {
        printf("\033[0;31mError ❌::Dificulty in checking source file extension in .bmp\033[0m\n");  
        return e_failure;
    }
    /* validate secrate file extension  is .txt or .c or .sh file or not*/
    if(strstr(argv[3],".txt") != NULL||strstr(argv[3],".c")||strstr(argv[3],".sh"))
    {
        encInfo->secret_fname=argv[3];
    }
    else
    { 
        printf("\033[0;31mError ❌::Dificulty in checking secret message file extension in .txt|.c|.sh.\033[0m\n");                
        return e_failure;
    }
    /* check output file present or not*/
    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp"))
            encInfo->stego_image_fname=argv[4];
        else
        {
            printf("\033[0;31mError ❌:Provide Output File with .bmp extension.\033[0m\n"); 
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname="stego.bmp";
    }
    sleep(1);
    return e_success;
}

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    printf("\033[0;35mOpen files proccess started.\033[0m\n");
    sleep(1);
    if(open_files(encInfo)==e_success)//opening files
    {
        printf("\033[0;32mFiles opend successfully.\033[0m\n"); // Green
        /* check capacity  of the file to store the data*/
        printf("\033[0;35mChecking capacity to encode message.\033[0m\n");
        sleep(1);
        if(check_capacity(encInfo)==e_success)          /*Checking capacity to encode message*/
        {
            printf("\033[0;32mCapacity of source file is more than the secrete file.\033[0m\n");
            printf("\033[0;35mHeader is copying.\033[0m\n");
            sleep(1);
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
            {                                                                       /*Header is copy*/
                printf("\033[0;32mSuccessfully header is copied.\033[0m\n");
                printf("\033[0;35mEncoding the magic string.\033[0m\n");
                sleep(1);
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)      /*Encoding the magic string*/
                {
                    printf("\033[0;32mMagic string Encoded Succesfully.\033[0m\n");
                    printf("\033[0;35mEncoding the File Extension Size .\033[0m\n");
                    sleep(1);
                    if(encode_secret_file_exten_size(strstr(encInfo->secret_fname,"."),encInfo)==e_success)/*Encoding the File Extension Size*/
                    {
                        printf("\033[0;32mFile Extension Size Encoded Succesfully.\033[0m\n");
                        printf("\033[0;35mEncoding the File Extension .\033[0m\n");
                        sleep(1);
                        if(encode_secret_file_extn(strstr(encInfo->secret_fname,"."),encInfo)==e_success)/*Encoding the File Extension*/
                        {
                            printf("\033[0;32mFile Extension  Encoded Succesfully(Ex:.txt)\033[0m\n");
                            printf("\033[0;35mEncoding Secret File size(data size) .\033[0m\n");
                            sleep(1);
                            long file_size = get_file_size(encInfo->fptr_secret);
                            if (file_size != -1 && encode_secret_file_size(file_size, encInfo) == e_success)    /*Encoding Secret File size(data size)*/
                            {
                                printf("\033[0;32mFile Size is Encoded Succesfully\033[0m\n");
                                printf("\033[0;35mEncoding the Secret Data .\033[0m\n");
                                sleep(1);
                                if(encode_secret_file_data(encInfo,file_size)==e_success)       /*Encoding the Secret Data*/
                                {
                                    printf("\033[0;32mSecret Data  Encoded Succesfully.\033[0m\n");
                                    printf("\033[0;35mEncoding the Remaining Data.\033[0m\n");
                                    sleep(1);
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                                    {
                                        printf("\033[0;32mRemaining Data  Encoded Succesfully.\033[0m\n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("\033[0;31mError ❌::Remaining Data Not Encoded\033[0m\n");  
                                        return e_failure;                                       
                                    }
                                }
                                else
                                {
                                    printf("\033[0;31mError ❌:Secret Data is Not Encoded.\033[0m\n");
                                    return e_failure;                                         
                                }
                            }
                            else
                            {
                                printf("\033[0;31mError ❌:Secret File Size is Not Encoded.\033[0m\n");
                                return e_failure;                                         
                            }
                        }
                        else
                        {
                            printf("\033[0;31mError ❌:Secret File Extension is Not Encoded.\033[0m\n");
                            return e_failure;                                         
                        }
                    }
                    else
                    {
                        printf("\033[0;31mError ❌:Secret File Extension size is Not Encoded.\033[0m\n");
                        return e_failure;                                         
                    }
                }
                else
                {
                    printf("\033[0;31mError ❌:Magic String is not encoded.\033[0m\n"); 
                    return e_failure;
                }
            }
            else
            {
                printf("\033[0;31mError ❌:Unable to copy the header.\033[0m\n"); 
                return e_failure;
            }  
        }
        else
        {
            printf("\033[0;31mError ❌::Capacity of file is less.Can't store the message. \033[0m\n"); 
            return e_failure;
        }  
    }
    else
    {
        printf("\033[0;31mError ❌::Dificulty in opening files \033[0m\n"); 
        return e_failure;
    }
    return e_failure;
}
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1024]; // Create a buffer to hold chunks of data
    size_t bytes_read;

    // Check if file pointers are valid
    if (fptr_src == NULL || fptr_dest == NULL) {
        perror("Error: File pointer is NULL");
        return e_failure;
    }

    // Read data from source file and write to destination file
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fptr_src)) > 0) {
        if (fwrite(buffer, 1, bytes_read, fptr_dest) != bytes_read) {
            perror("Error writing to destination file");
            return e_failure;
        }
    }

    // Check for errors during fread
    if (ferror(fptr_src)) {
        perror("Error reading from source file");
        return e_failure;
    }

    return e_success;
}


/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo,long file_size)
{
    char file_data[file_size+1];
    // Read secret file data into buffer
    rewind(encInfo->fptr_secret);
    fread(file_data,file_size,1,encInfo->fptr_secret);
    
   file_data[file_size]='\0';
  // printf("file dataaaaaa   %s\n\n\n",file_data);     //for checking
    if(encode_data_to_image(file_data,file_size,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
        return e_success;
    return e_failure;
}
/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    //printf("file Size is %ld\n",file_size);
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, image_buffer);
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/* Store secret file extension(.txt)  in stego.bmp(o/p file)*/
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image(strstr(encInfo->secret_fname,"."),strlen(strstr(encInfo->secret_fname,".")),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
        return e_success;
    return e_failure;
}
/* Store secret file extension size in stego.bmp(o/p file)*/
Status encode_secret_file_exten_size(char*file_exten,EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer,32,1,encInfo->fptr_src_image);
    if(encode_size_to_lsb(strlen(file_exten),image_buffer)==e_success)
    {
        fwrite(image_buffer,32,1,encInfo->fptr_stego_image);
        return e_success;
    }
    return e_failure;
   
}
/* encoding secret file extension  size(4 or 3 or 2 ) in stego.bmp(o/p file)*/
Status encode_size_to_lsb(int size,char * Image_buffer)
{
    for(int i=0;i<32;i++)
    {
        Image_buffer[i]=(Image_buffer[i]&0xFE)|((size&(1<<i))>>i);  
       /* // Print the image buffer in a readable way for checking purpose
       printf("Image buffer: ");
        for (int j = 0; j < 8; j++) {
            printf("%02x ", (unsigned char)Image_buffer[j]); // Print as hex
        }
        printf("\n\n");  */     
    }
    return e_success;
}
/* Store Magic String  in the stego.bmp*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    //encode the message    //data AGIC_STRING: #*
    if(encode_data_to_image(MAGIC_STRING,strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {       
        return e_success;
    } 
    return e_failure;
}
/* Encode function, which does the real encoding  */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];           
    for(int i=0;i<size;i++)
    {
        fread(image_buffer,8,1,fptr_src_image);
        //printf("dataaa     %c      \n",data[i]);  //for checking purpose
        if(encode_byte_to_lsb(data[i],image_buffer)==e_success)
        {
            /* // Print the image buffer in a readable way for checking purpose
            printf("Image buffer: ");
            for (int j = 0; j < 8; j++) {
                printf("%02x ", (unsigned char)image_buffer[j]); // Print as hex
            }
            printf("\n\n"); */ 
            fwrite(image_buffer,8,1,fptr_stego_image);
        }
    }
    return e_success;
}
/* Encode a byte into LSB of image data array  */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //printf(" %c\n",data);
    for(int i=0;i<8;i++)
    {
        char val=(data&(1<<i));
        // printf("%d ",val);
        if(val)//if non zero value
        {
            image_buffer[i]=image_buffer[i]&(~1);
            image_buffer[i]=image_buffer[i]|1;
        }
        else
        {
            image_buffer[i]=image_buffer[i]&(~1);
            image_buffer[i]=image_buffer[i]|0;
        }
        //printf("Image Buffer %d",image_buffer[i]);
    }
    return e_success;
}
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    fseek(fptr_src_image,0,SEEK_SET);
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);
    return e_success;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);
    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);
    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);
    // Return image capacity
    return width * height * 3;  //3 is for RGB and returning the size in bytes
}

/* Get file size  secret*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
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
    	perror("fopen");    //perror():::it will display actual error
    	fprintf(stderr, "\033[0;31m ERROR❌ : Unable to open file %s\033[0m\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "\033[0;31m ERROR❌ : Unable to open file %s\033[0m\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "\033[0;31m ERROR❌ : Unable to open file %s\033[0m\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("\033[0;34mEncoding file:: %s\n\033[0m",encInfo->stego_image_fname);
    // No failure return e_success
    return e_success;
}
/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    int image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);     //calculating image size (line 83);
    int file_size=get_file_size(encInfo->fptr_secret);          //calculating secret file size  (line 101);
    //magic string length + extension size length +extension length+filesize+file data;
    int encode_size=54+(4+4+file_size+file_size)*8;     //*8 because=== 1 byte encoding needs  8bytes;
    if(image_capacity>encode_size)
    {
        return e_success;
    }
    else
    {
        printf("\033[0;31mError ❌::capacity of source file is less than the secrete file.\033[0m\n");
        return e_failure;
    }  
}