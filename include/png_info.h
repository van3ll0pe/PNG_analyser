#pragma once
#ifndef PNG_INFO_H
#define PNG_INFO_H

#include <stdint.h>

//structure pour stocker tous les éléments d'un fichier PNG

typedef struct PNG_info
{
    uint32_t    width_picture;
    uint32_t    height_picture;
    uint8_t     bits_depth;
    uint8_t     color_type;
    uint8_t     compression;
    uint8_t     filtre;
    uint8_t     interlace;

    uint8_t *data_file; //all the data from file
    size_t  lenght; //the size of data_file
    size_t index;   //the index to browse the data of file

}PNG_info_t;


//PROTOTYPES

extern int check_pngFile(unsigned char *data);
extern void extract_IHDR_chunk(PNG_info_t *png_info);
extern int browseTo_IDAT_PLTE(PNG_info_t *png_info);
extern void extract_IDAT_chunk(PNG_info_t *png_info);
extern void extract_PLTE_chunk(PNG_info_t *file_png);
extern int isNextChunk_IEND(PNG_info_t *png_info);
extern void extract_IEND_chunk(PNG_info_t *png_info);

#endif //png_info_h