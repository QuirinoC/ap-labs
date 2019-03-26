#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <inttypes.h>

#define BUF_SIZE   0xFFFFFF
//CODE FROM---------------------------------------------------------------------
//https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void build_decoding_table() {

    decoding_table = malloc(256);
    int i;
    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}


unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
    if (decoding_table == NULL) build_decoding_table();
    if (input_length % 4 != 0) return NULL;
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    return decoded_data;
}


void base64_cleanup() {
    free(decoding_table);
}
//-----------------------------------------------------------------------------

void print_usage() {
    errorf("Usage: ./base64 --[encode/decode] filename\n");
}

int main(int argc, char *argv[]) {
    int opt= 0;
    int encode = -1; int decode = -1;

    //Specifying the expected options
    //The two options l and b expect numbers as argument
    static struct option long_options[] = {
        {"decode", required_argument,       0,  'd' },
        {"encode", required_argument,       0,  'e' },
        {0,        0,                 0,   0  }
    };

    char *filename = calloc(1024, sizeof(char));
    char *output_file = calloc(1024, sizeof(char));

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"de:", 
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 'd' : decode = 0;
                strcpy(filename, optarg);
                strcpy(output_file, "decoded.txt");
                break;
             case 'e' : encode = 0;
                strcpy(filename, optarg);
                strcpy(output_file, "encoded.txt");
                break;

             default: print_usage(); return -1;
        }
    }  
    //File stuff
    long size;
    char *buffer;
    long read_size = 0;


    FILE *ifp;
    ifp = fopen(filename,"r");
    if (ifp == NULL) {
        panicf("File not found\n");
        return -1;
    } else {
        fseek(ifp, 0L, SEEK_END);
        size = ftell(ifp);
        fseek(ifp, 0L, SEEK_SET);
        buffer = malloc(size);
        read_size = fread(buffer, 1, size, ifp);
        if (read_size == 0) {
            panicf("Cannot read from file [Empty?]\n");
            return -1;
        }
    }
    size_t output_size = 0;

    char *result;

    
    if (encode == 0) {
        result = base64_encode(buffer, strlen(buffer), &output_size);
    } else if (decode == 0) {
        result = (char *) base64_decode(buffer, strlen(buffer), &output_size);
    } else {
        print_usage();
        return -1;
    }    


    FILE *ofp;
    ofp = fopen(output_file,"w");
    if (ofp == NULL) {
        panicf("Error creating file [%s] for output\n", output_file);
        return -1;
    } else {
        infof("Creating file [%s] for output\n", output_file);
    }

    infof("%s %s\n", (encode == 0) ? "Encoding" : "Decoding", filename);

    //printf(result);
    fwrite(result, output_size, 1, ofp);

    fclose(ifp);
    fclose(ofp);
    

    return 0;
}