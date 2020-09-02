#define NOT ! 
#include "suporte.h"

int     tictactoe(char*);
unsigned char* ref(unsigned char*);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf(
            "Faltou o nome do arquivo\n"
            "\n\tUse: programa arquivo\n");
        return -1;
    };  // if()

    int modo = 0;
    _cs_status*     antes =  guarda();
    _cs_status*     config = prepara_tela(antes);


    tictactoe(argv[1]);
    printf("\n\n\tDepois de tictactoe Tecle algo ==> "); fgetc(stdin);
    restaura(antes);
    _mostra_console("depois de restaura");
    cls();
    wprintf(L"Fonte inicial %s\n", antes->fonte.FaceName);
    free(antes);
    //free(config);
    printf("Tecle algo ==> "); fgetc(stdin);
    if (modo == 0) return 500;
    return 0;
};  // main()

int         tictactoe(char* arquivo)
{
    FILE* in = fopen(arquivo, "r");
    if (in < 0) return -2;

    unsigned char buffer[60];
    fprintf(stderr, "Usando arquivo '%s'\n", arquivo);
    fgets(buffer, 60, in);
    while (NOT feof(in))
    {
        buffer[strlen(buffer) - 2] = 0;
        printf("%s\n", ref(buffer));
        fgets(buffer, 60, in);
    };
    fclose(in);
    //  aaa << resulta >> bbb  |
    return 0;
};  // box()

unsigned char*     ref(unsigned char* buf)
{
    unsigned char T[256];
    memset(T, 0, 256);
    T['X'] = 32;  // espaco
    T['V'] = 186; // vertical dupla
    T['H'] = 205; // horizontal dupla
    T['.'] = 206; // intersecao dupla

    T['A'] = 201; // os cantos duplos
    T['B'] = 187;
    T['C'] = 200;
    T['D'] = 188;

    T['L'] = 199; // duplo com simples a direita
    T['R'] = 182; // duplo com simples a esquerda
    T['U'] = 209; // duplo hor com simples descendo
    T['^'] = 207; // duplo hor com simples subindo

    T['v'] = 179; // vertical simples
    T['h'] = 196; // horizontal simples
    T['#'] = 197; // intersecao simples


    T['a'] = 218; // os cantos simples
    T['b'] = 191;
    T['c'] = 192;
    T['d'] = 217;

    T['l'] = 198; // simples com duplo a direita
    T['r'] = 181; // simples com duplo a esquerda
    T['u'] = 210; // simples hor com duplo descendo
    T['*'] = 208; // simples hor com duplo subindo

    for(int i=0; buf[i]!=0; i+=1)
        if ( T[ buf[i] ] != 0) buf[i] = T[ buf[i] ];
    return buf;
}


//for (int i = 128; i < 255; i += 8)
//{

//    printf("\n[%3d]\t", i);
//    for (int j = 0; j < 8; j += 1)
//    {
//        printf("%4c", i+j);
//    };
//};
//printf("\n");

