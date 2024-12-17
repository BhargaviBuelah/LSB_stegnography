/*
Name:N.Bhargavi
Date:28/09/24
Description:Stegnograhy Project
 */
#ifndef DECODE_H
#define DECODE_H

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

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    

    /* Secret File Info */
    int secret_file_exten_size;
    char secret_fname[10];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long secret_file_size;

} DecodeInfo;


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

char * decode_data_from_image(int size,DecodeInfo *decInfo);

/* decode  a byte of LSB from image data array */
char decode_byte_from_lsb(char *image_buffer);

/*decode secret file extension size*/
Status decode_secret_file_exten_size(DecodeInfo *decInfo);

int decode_size_from_lsb(int size,char * Image_buffer);

/*decode secret file extension*/
Status decode_secret_file_exten(DecodeInfo *decInfo);

/*decode secret file  size*/
Status decode_secret_file_data_size(DecodeInfo *decInfo);

/*decode secret file */
Status decode_secret_file_data(DecodeInfo *decInfo);
#endif