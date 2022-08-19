#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

#include "../include/png_info.h"

int readBinaryFile(char * filename, unsigned char **ptr, size_t *lenght);

/**
 * @brief Open and read a binary file.
 * The first parameter is the filename of the file to open
 * The second parameter is the pointer to stock on it all data from file
 * The third parameter is a pointer to stock the lenght of the file.
 * 
 * If the function works it will return 0 else it return 1.
 * 
 * The function alloc the pointer at the lenght size to put all the data.
 * 
 * @param filename 
 * @param ptr 
 * @param lenght 
 * @return int 
 */
int readBinaryFile(char * filename, unsigned char **ptr, size_t *lenght)
{
    struct stat file_info;
    stat(filename, &file_info);

    (*lenght) =  file_info.st_size;
    (*ptr) = malloc(sizeof(unsigned char) * file_info.st_size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Error to allocate memory\n");
        perror("");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error to open file %s\n", filename);
        perror("");
        free((*ptr));
        return EXIT_FAILURE;
    }

    if (fread((unsigned char *)(*ptr), sizeof(unsigned char) * file_info.st_size, 1, file) != 1)
    {
        fprintf(stderr, "Error to read the file %s\n", filename);
        perror("");
        fclose(file);
        free((*ptr));
        return EXIT_FAILURE;
    }

    fclose(file);
    return EXIT_SUCCESS;
}







int main(int argc, char **argv)
{   
    //vérifie si l'utilisateur donne bien un fichier
    if (argc != 2)
    {
        fprintf(stderr, "Error wrong argument\nYou must give a PNG file as an argument\n");
        return EXIT_FAILURE;
    }

    PNG_info_t file_png = {.bits_depth = 0, .color_type = 0, .compression = 0, .index = 8, .interlace = 0, .width_picture = 0, .height_picture = 0, .data_file = NULL, .filtre = 0};
    
    if (readBinaryFile(argv[1], &file_png.data_file, &file_png.lenght) == 1)
    {
        return EXIT_FAILURE;
    }

   
    if (check_pngFile(file_png.data_file) == 1) //on check si le programme est de type PNG
    {
        printf("\n%s is a PNG FILE\nSize of the file : %lu bytes\n", argv[1], file_png.lenght);
        extract_IHDR_chunk(&file_png); //on extrait les informations du chunk IHDR

        while(isNextChunk_IEND(&file_png) == 1)
        {
            int chunck_value = browseTo_IDAT_PLTE(&file_png);
            if (chunck_value == 0)
            {
                extract_PLTE_chunk(&file_png);
            }
            else if (chunck_value == 1)
            {
                extract_IDAT_chunk(&file_png);
            }
        }
        extract_IEND_chunk(&file_png);

    }
    else
    {
        printf("\n%s is not a PNG FILE\n", argv[1]);
        free(file_png.data_file);
        return EXIT_FAILURE;
    }

    free(file_png.data_file);
    return EXIT_SUCCESS;
}