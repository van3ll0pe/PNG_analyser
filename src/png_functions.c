#include "../include/png_info.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief This function returns 1 if the file is a PNG file else it returns 0.
 * The function checks the PNG signature
 * 
 * @param data 
 * @return int 
 */
int check_pngFile(unsigned char *data)       
{
    if (data[0] == 137 && data[1] == 80 && data[2] == 78 && data[3] == 71 && data[4] == 13 && data[5] == 10 && data[6] == 26 && data[7] == 10)
    {
        return 1;
    }
    return 0;
}


/**
 * @brief Function to extract and browse each element of the first chunk of the PNG file.
 * The parameter is a pointer of the structure to storage each element
 * 
 * @param data 
 * @param index 
 */
void extract_IHDR_chunk(PNG_info_t *png_info)
{
    uint32_t lenght_data_chunk = 0; //la taille du chunk
    uint32_t crc = 0; //le crc du chunk
    

    //la taille du chunk se calcule sur les 4 premiers octets du chunk, i parcourt les 4 octets, index permet de savoir la position générale et bitwise_decal permet d'additionner les octets sur 32 bits
    for (int i = 0, bitwise_decal = 24;  i < 4 ; i++, png_info->index++, bitwise_decal -= 8) 
    {
        lenght_data_chunk = lenght_data_chunk | (png_info->data_file[png_info->index] << bitwise_decal);
    }
    printf("\n-----------------------------------\n\n");

    printf("CHUNK : ");
    //permet de récupérer le nom du chunk sur 4 octets
    for (int i = 0; i < 4; i++, png_info->index++)
    {
        printf("%c", png_info->data_file[png_info->index]);
    }
    printf("\n");
    printf("Taille : %u\n\n", lenght_data_chunk);

    //permet de récupérer les données du chunk IHDR en fonction de la longueur du chunk
    for (uint32_t i = 0; i < lenght_data_chunk; i++, png_info->index++)
    {
        for (int bitwise_decal = 24; i < 4; i++, bitwise_decal -= 8, png_info->index++) //cette boucle permet de récupérer les 4 premiers octets (largeur de l'image)
        {
            png_info->width_picture |= (png_info->data_file[png_info->index] << bitwise_decal);
        }
        

        for (int bitwise_decal = 24; i < 8; i++, png_info->index++, bitwise_decal -= 8) //cette boucle permet de récupérer les 4 seconds octets (hauteur de l'image)
        {
            png_info->height_picture |= (png_info->data_file[png_info->index] << bitwise_decal);
        }
        

        switch (i) {
            case 8: //la profondeur en bits
                    printf("La largeur de l'image  : %u\n", png_info->width_picture);
                    printf("La hauteur de l'image  : %u\n", png_info->height_picture);

                    png_info->bits_depth = png_info->data_file[png_info->index];

                    printf("La profondeur de bits  : %d\n", png_info->bits_depth);
                    break;

            case 9: //le color type
                    png_info->color_type = png_info->data_file[png_info->index];
                    printf("Le color type          : %d\n", png_info->color_type);
                    break;

            case 10: //la compression
                     png_info->compression = png_info->data_file[png_info->index];
                     printf("La compression        : %d\n", png_info->compression);
                     break;

            case 11: //le filtre
                     png_info->filtre = png_info->data_file[png_info->index];
                     printf("Le filtre             : %d\n", png_info->filtre);
                     break;

            case 12: //interlace
                     png_info->interlace = png_info->data_file[png_info->index];
                     printf("L'interlace           : %d\n", png_info->interlace);
                     break;

            default : break;
        }
    }

    //boucle pour parcourir le CRC du chunk IHDR
    for (int i = 0, bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8)
    {
        crc |= (png_info->data_file[png_info->index] << bitwise_decal);
    }
    
    printf("Le CRC : %u\n", crc);
    printf("\n-----------------------------------\n");
}

/**
 * @brief Function to check if the next chunk is the last chunk of the file
 * If the next chunk, is the last the function will return 0 else it will return 1
 * @param png_info  (c'est pour la condition de la boucle qui va s'arrêter si elle est au dernier chunk)
 * @return int 
 */
int isNextChunk_IEND(PNG_info_t *png_info)
{
    char chunk_name[5];
    int i = 0;
    for (; i < 4; i++) {}; //passe les 4 octets qui représente la taille du chunk

    for (int j = 0; i < 8 ; i++, j++)
    {
        chunk_name[j] = png_info->data_file[png_info->index + i];
    }

    chunk_name[4] = '\0';

    if (strcmp(chunk_name, "IEND") == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/**
 * @brief Fonction qui parcourt le tableau jusqu'à arriver à un chunk PLTE ou IDAT ou IEND
 * Lorsque la fonction trouve un chunk PLTE ou IDAT il retourne les valeurs associées.
 * Le chunk IEND est aussi cherché car comme cette fonction est récursive elle n'a aucun moyen de s'arrêter si elle ne trouve pas les deux autres chunks.
 * Cela permet  de dire  qu'on arrive à la fin du fichier et qu'il faut arrêter de rechercher
 * @param png_info 
 * @return int 
 */
int browseTo_IDAT_PLTE(PNG_info_t *png_info)
{
    uint32_t i = 0; //permet de parcourir la longueur ainsi que le nom du chunk
    char chunk_name[5];
    uint32_t length_chunk = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       

    //récupérer la taille du chunk
    for (int bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8)
    {
        length_chunk |= (png_info->data_file[png_info->index] << bitwise_decal);   
    }

    //récupérer le nom du chunk
    for (int j = 0; i < 8; i++, j++, png_info->index++) //j permet de se déplacer dans le tableau
    {
        chunk_name[j] = png_info->data_file[png_info->index];
    }
    chunk_name[4] = '\0';

    

    if (strcmp(chunk_name, "PLTE") == 0) //Si on trouve le chunk PLTE
    {
        //je retourne en arrière de 8 octets pour récupérer la taille du chunk et son nom (que j'avais parcouru précédemment)
        png_info->index = png_info->index - 8;
        return 0;
    }
    else if (strcmp(chunk_name, "IDAT") == 0) //si il y a le chunk IDAT
    {
        //je retourne en arrière de 8 octets pour récupérer la taille du chunk et son nom (que j'avais parcouru précédemment)
        png_info->index = png_info->index - 8;
        return 1;
    }
    else if (strcmp(chunk_name, "IEND") == 0) //si il y a le chunk IEND (si on arrive au dernier chunk pour eviter de checker alors qu'on arrive à la fin)
    {
        //je retourne en arrière de 8 octets pour récupérer la taille du chunk et son nom (que j'avais parcouru précédemment)
        png_info->index = png_info->index - 8;
        return 2;
    }
    else //sinon on affiche le nom ddu chunk et on relance cette fonction jusqu'à atteindre le PLTE ou le IDAT
    {
        printf("\nChunk : %s\n", chunk_name);
        printf("TAILLE : %u bytes\n", length_chunk);
        printf("\n-----------------------------------\n");
        

        for (i = 0; i < length_chunk; i++, png_info->index++) {}; //on parcourt les données du chunk
        for (i = 0; i < 4; i++, png_info->index++) {}; //on parcourt le CRC du chunk

        return browseTo_IDAT_PLTE(png_info); //récursivité pour continuer de chercher jusqu'à atteindre le IDAT ou le PLTE
    }
}

/**
 * @brief function to extract information of the PLTE chunk 
 * 
 * @param file_png 
 */
void extract_PLTE_chunk(PNG_info_t *png_info)
{
    

    uint32_t i = 0;
    char chunk_name[5];
    uint32_t length_chunk = 0;
    uint32_t crc = 0; //le crc du chunk                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   

    for (int bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8)
    {
        length_chunk |= (png_info->data_file[png_info->index] << bitwise_decal);   
    }

    for (int j = 0; i < 8; i++, j++, png_info->index++) //j permet de se déplacer dans le tableau
    {
        chunk_name[j] = png_info->data_file[png_info->index];
    }
    chunk_name[4] = '\0';

    printf("\nCHUNK : %s\n", chunk_name);
    printf("Taille: %u\n", length_chunk);

    for (i = 0; i < (length_chunk / 3); i++, png_info->index+=3) //on parcourt la longueur /3 car les palettes sont composées de 3 octets à chaque fois (R - G - B)
    {
        printf("PALETTE numero %u :\n", i + 1);
        printf("R : %d\n", png_info->data_file[png_info->index]);
        printf("G : %d\n", png_info->data_file[png_info->index + 1]);
        printf("B : %d\n", png_info->data_file[png_info->index + 2]);
    }

    //boucle pour récupérer le CRC du chunk PLTE
    i = 0;
    for (int bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8)
    {
        crc |= (png_info->data_file[png_info->index] << bitwise_decal);
    }

    printf("\nCRC : %u\n\n", crc);
    printf("\n-----------------------------------\n");
}


/**
 * @brief Fonction qui permet d'extraire les informations d'un chunk IDAT
 * Si les informations sont filtrées et/ou compréssées, il faudra les décomprésser.
 * 
 * @param png_info 
 */
void extract_IDAT_chunk(PNG_info_t *png_info)
{
    uint32_t chunk_lenght = 0;
    uint32_t i = 0;
    char chunk_name[5];
    uint32_t crc = 0;

    for (int bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8) //on parcourt les 4 premiers octets pour connaitre la longueur du chunk
    {
        chunk_lenght |= (png_info->data_file[png_info->index] << bitwise_decal);
    }

    for (int j = 0; i < 8; i++, j++, png_info->index++) //on récupère le nom du chunk
    {
        chunk_name[j] = png_info->data_file[png_info->index];
    }
    chunk_name[4] = '\0';

    printf("\nCHUNK : %s\n", chunk_name);
    printf("TAILLE : %u\n", chunk_lenght);

    printf("DATA from the CHUNK :\n");
    for(i = 0; i < chunk_lenght; i++, png_info->index++)
    {
        if (i == chunk_lenght -1)
            printf("%d\n", png_info->data_file[png_info->index]);
        else
            printf("%d ", png_info->data_file[png_info->index]);
    }

    i = 0;
    for (int bitwise_decal = 24; i < 4; i++, bitwise_decal -= 8, png_info->index++)
    {
        crc |= (png_info->data_file[png_info->index] << bitwise_decal);
    }
    
    printf("CRC : %u\n\n");
    printf("\n-----------------------------------\n");
}

/**
 * @brief Function permettant d'extraire le dernier chunk qui est le chunk IEND
 * 
 * @param png_info 
 */
void extract_IEND_chunk(PNG_info_t *png_info)
{
    uint32_t i = 0;
    uint32_t crc = 0;
    uint32_t chunk_lenght = 0;

    for (int bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8) //la taille du chunk IEND
    {
        chunk_lenght |= (png_info->data_file[png_info->index] << bitwise_decal);
    }

    printf("\nCHUNK : ");

    for (; i < 8; i++, png_info->index++) //le nom du chunk IEND
    {
        printf("%c", png_info->data_file[png_info->index]);
    }
    printf("\nTAILLE : %u\n", chunk_lenght);
    printf("DATA : ");
    for (i = 0; i < chunk_lenght; i++, png_info->index++) //les données du chunk IEND
    {
        printf("%i ", png_info->data_file[png_info->index]);
    }

    i = 0;
    for (int bitwise_decal = 24; i < 4; i++, png_info->index++, bitwise_decal -= 8)
    {
        crc |= (png_info->data_file[png_info->index] << bitwise_decal);
    }

    printf("\nCRC : %u\n\n", crc);
    printf("\n-----------------------------------\n");
}