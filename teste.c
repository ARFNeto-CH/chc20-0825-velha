#define NOT ! 
#include "suporte.h"

int main(int argc, char** argv)
{
    _cs_status* antes = guarda();
    int f = 0;
    int ch = 0;
    int res = 0;
    _mostra_console("teste");
    _cs_status* config = prepara_tela(antes);
    gotoYX(0, 0);
    for(int i=0;i<5;i+=1)    printf("De volta a main()\n");
    _mostra_console("main() depois de prepara_tela()");
    restaura(antes);
    _mostra_console("main() depois de restaura()");
    free(antes);
    free(config);
    return 0;
};
