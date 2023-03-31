/* 
Criptografia AES_128 + CBC + ANSI X9.23

Autores: Thalis Duarte e Thales Pinheiro

*/


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "aes.h"

uint8_t * xor(uint8_t * a, uint8_t * b, uint8_t * resp){
    for(int i = 0; i < 16; i++){
        resp[i] = a[i] ^ b[i];
    }
    return resp;
}

void cbc_enc(uint8_t * input, uint8_t * key, uint8_t * iv, uint8_t * output){
    uint8_t xor_r[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    xor(input, iv, xor_r);

    AES128_Encrypt(xor_r, key, output);
}

void cbc_dec(uint8_t * input, uint8_t * key, uint8_t * iv, uint8_t * output){
    uint8_t cipher_key[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    AES128_Encrypt(input, key, cipher_key);
    
    xor(cipher_key, iv, output);
}



int main(int argc, char * argv[]) {
    // CBC + ANSI X9.23
    
    uint8_t key[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // 16 bytes = 128 bits
    uint8_t input[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t output[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t dec[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    FILE * input_file;
    FILE * output_file;
    
    if(argc != 5){
        printf("Usage: ./[file] [enc|dec] [input file] -o [output file]/n");
        return 0;
    }
    input_file = fopen(argv[2], "r");
    if(!input_file){
        printf("Input file not found!");
        return 0;
    }
    output_file = fopen(argv[4], "w");
    // if(!input_file){
    //     printf("Input file not found!");
    //     return 0;
    // }
    
       
    
    // while(fread(input, 1, 16, input_file) != 0){
    //     printf("%s\n", input);
    // }

    
    printf("Enter passphrase for key: ");
    fgets(key, 16, stdin );
    int bytes_readed = 0;
    
    if(!strcmp(argv[1], "enc")){
        
        uint8_t * last_block_output = iv;
        while((bytes_readed = fread(input, 1, 16, input_file)) != 0){
            // implementar adiciona padding aqui

            // AES128_Encrypt(input, key, output);

            cbc_enc(input, key, last_block_output, output);

            fwrite(output, 1, bytes_readed, output_file);
            last_block_output = output;
            
            printf("! %d\n", bytes_readed);
        }
        fscanf(output_file,"%s",output);
    
        // printf("Encrypted message: \n");
        // for( int i = 0; i < 16; i++ ) {
        //     printf("%.2X", output[i] );
        // }
        // printf("\n");

    }else if(!strcmp(argv[1], "dec")){

        uint8_t * last_block_input = iv;
        while((bytes_readed = fread(input, 1, 16, input_file)) != 0){

            // AES128_Decrypt(input, key, output);

            cbc_dec(input, key, last_block_input, output);
            fwrite(output, 1, bytes_readed, output_file);
            last_block_input = input;
            
            // printf("@ %d '%s'\n", bytes_readed, output);
        }
        // fscanf(output_file,"%s",output);
        
        // printf("Decrypted message: \n");
        // printf("%s\n", output);
        
    }else{
        printf("Invalid option. Use [enc|dec]");
    }
  
  

    fclose(input_file);
    fclose(output_file);
    return 0;
       
}
