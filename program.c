#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// BMP file header structure
typedef struct BmpFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} __attribute__ ((packed)) BmpFileHeader;

typedef struct BmpImageHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} __attribute__ ((packed)) BmpImageHeader;

// Function declaration
extern void diagonal_swap(uint8_t* img, int size);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments\n");
        return 1;
    }
    
    char* filename = argv[1];

    // Open the file
    FILE *file_ptr = fopen(filename, "rb");
    if (file_ptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Get the file size
    fseek(file_ptr, 0, SEEK_END);
    long fileSize = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);

    // Allocate memory to hold the entire file
    uint8_t *fileData = (uint8_t *)malloc(fileSize);
    if (fileData == NULL) {
        perror("Error allocating memory");
        fclose(file_ptr);
        return 1;
    }

    // Read the entire file into memory
    fread(fileData, fileSize, 1, file_ptr);
    fclose(file_ptr);

    // Pointers to headers within the buffer
    BmpFileHeader *fileHeader = (BmpFileHeader *)fileData;
    uint8_t *imageData = fileHeader->bfOffBits + fileData;
    BmpImageHeader *imageHeader = (BmpImageHeader *)(fileData + sizeof(BmpFileHeader));

    int width = imageHeader->biWidth;
    int height = imageHeader->biHeight;
    int bitCount = imageHeader->biBitCount;

    printf("Width: %d, Height: %d, BitCount: %d\n", width, height, bitCount);

    if (bitCount != 1) {
        printf("Not a 1bpp BMP file\n");
        free(fileData);
        return 1;
    }

    // Call the x86 function
    diagonal_swap(imageData, width);

    // Write the modified file data back to a new file
    file_ptr = fopen("result.bmp", "wb");
    if (file_ptr == NULL) {
        perror("Error opening file for writing");
        free(fileData);
        return 1;
    }

    fwrite(fileData, fileSize, 1, file_ptr);
    fclose(file_ptr);

    free(fileData);
    return 0;
}
