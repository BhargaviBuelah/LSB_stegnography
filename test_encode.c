/*
Name:N.Bhargavi
Date:28/09/24
Description: Stegnograhy Project
 */
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

Status main(int argc,char *argv[])
{
    if(check_argc_count(argc,argv)==e_failure)      /*check argument count */
    {
        printf("\033[0;31mError: ❌Unable To perform Operation.Arrguments Should be >3.\033[0m\n");
        printf("\033[0;33mFor Encoding Arguments should be:\na.out -e <sourcefile.bmp> <secreatefile.txt> <outputfile.bmp>(outputfile is optional)\nFor decoding Arguments should be:\na.out -d <sourcefile.bmp> <outputfile.bmp>.\033[0m\n");
        return e_failure;
    }
    else
    {
        if(check_operation_type(argv)==e_encode)        /*check operation type*/
        {
             printf("\033[4;33mEncoding Started.\033[0m\n"); 
            printf("\033[0;35mChecking arguments is stated\033[0m\n");
            /*Allocating memory for the structure */
            EncodeInfo encInfo;
            /* Read and validate Encode args from argv  is .bmp file or not*/
            if(read_and_validate_encode_args(argv,&encInfo)==e_success)     
            {
                printf("\033[0;32mArguments verified successfully.\033[0m\n"); // Green
                //printf("Source image filename: %s\n", encInfo.src_image_fname);
                //printf("Secret file name: %s\n", encInfo.secret_fname);
                  /* Perform the encoding */
                if(do_encoding(&encInfo)==e_success)
                {   
                    printf("\033[4;33mEncoding is completed.\033[0m\n");
                }
                else
                {
                    printf("\033[0;31m Error ❌: In Encoding.\033[0m\n");   // Red
                }
            }
            else
            {
                printf("\033[0;31mDificulty in checking file extensions\033[0m\n");                
                return e_failure;
            }
          
        }
        else if(check_operation_type(argv)==e_decode)
        {
            printf("\033[4;33mDcoding is stated.\033[0m\n"); 
            printf("\033[0;35mChecking arguments is stated\033[0m\n");
            /*Allocating memory for the structure */
            DecodeInfo decInfo;
            
            // Read and validate Encode args from argv  is .bmp file or not
            if(read_and_validate_decode_args(argv,&decInfo)==e_success)
            {   
                printf("\033[0;32mArguments verified successfully.\033[0m\n");
                //printf("Source image filename: %s\n", decInfo.src_image_fname);
                if(do_decoding(&decInfo)==e_success)
                {   
                    printf("\033[4;33mDcoding is Complited.\033[0m\n"); 
                }
                else
                {
                    printf("\033[0;31m Error ❌: In Decoding.\033[0m\n");
                }
                
            }
            else
            {
               printf("\033[0;31mDificulty in checking file extensions.\033[0m\n");           
                return e_failure;
            }

        } 
        else if(check_operation_type(argv)==e_unsupported)
        {
            printf("\033[0;31mError:❌ Unable to proceed operation provide valid type(-e or -d).\033[0m\n");   // Red
        }
    }
    
}
