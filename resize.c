/* 
 resize.c 
 
 A program that can enlarge an uncompressed image 
 with a factor of n. It takes two arguments
 
*/

#include <stdio.h>
#include <stdlib.h>

// include the bmp.h file 

#include "bmp.h"

int main(int argc, char* argv[])
{
    // check if user has added right number of arguments
    if(argc != 4)
    {
        printf("The argumets provided are not correct\n");
        return 1;
    }
    
    // check if the value of n is between 1 and 100 
    int n = atoi(argv[1]);
    if(n < 0 && n > 100)
    {
        printf("The values you have entered must be between 1 and 100\n");
        return 2;
    }
    
    // store the adress of input and output file 
    char* inputf = argv[2];
    char* outputf = argv[3];
    
    
    //open the input file to read from and check for null
    FILE* input = fopen(inputf, "r");
    if(input == NULL)
    {
        printf("There was some problem opening the file\n");
        return 3;
    }
    
    //open the output flie to write to 
    FILE* output = fopen(outputf, "w");
    if(output == NULL)
    {
        fclose(input);
        printf("There was some problem opening the file to copy to\n");
        return 4;
    }
    
    
    //read input file's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, input);
    
    //read input file's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, input);
    
    // store orignal width and height
    int OrignalW = bi.biWidth;
    int OrignalH = abs(bi.biHeight);
  
    
    
    // update width and height with the factor (n)
    bi.biWidth *= n;
    bi.biHeight *= n;
    
      
    // calculate padding for old and new file
    int OutPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int InPadding = (4 - (OrignalW * sizeof(RGBTRIPLE)) % 4) % 4;
    
    
    // change the members according the scale inputed
    bi.biSizeImage = ((abs(bi.biWidth) + OutPadding)) * (abs(bi.biHeight)) ; // size of the image in bytes
    bf.bfSize = (bi.biSizeImage + bf.bfOffBits); // size of the image + header
    
    //check if the file is a bitmap file 
    if(bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(input);
        fclose(output);
        printf("this is not a bmp file\n");
        return 5;
    }
    
    // write to the ouput file header
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, output);
    
    // write to the output file info header
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, output);
    
    
    // itterate throught each pixile
    for(int i = 0; i < OrignalH; i++)
    {
        // loop to add the row by n factor           
        for( int y = 0; y < n; y++)
        {
            // bring back to the begning of the row to print by n factor times
            // if(fseek(input, -((OrignalW *3) + InPadding), SEEK_CUR) != 0)
            // {
            //     printf("Something went wrong\n");
            //     return 6;
            // }
        
            
            // scaling throught every pixile        
            for ( int j = 0; j < OrignalW; j++)
            {
    
                
                RGBTRIPLE triple;
                
                // read the pixile from the input file 
                fread(&triple, sizeof(RGBTRIPLE), 1, input);
                    
                // print that pixile n number of times horizontally 
                for(int x = 0; x < n; x++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, output);
     
            }
              
            // skip over padding, if any
            fseek(input, InPadding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < OutPadding; k++)
                fputc(0x00, output);
                
            fseek(input, -((OrignalW *3) + InPadding), SEEK_CUR);    
            
        }  
      
            
         //seek to the next line
        fseek(input, (OrignalW *3) + InPadding, SEEK_CUR);   
        
    }
    
    // close input file
    fclose(input);
    
    // close output file
    fclose(output);
    
    return 0;
    
}