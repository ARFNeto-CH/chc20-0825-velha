#pragma once
#include "suporte.h"


int         box(char l, char c)
{
    // desenha o box em (l,c)
    const char simbolo[13] = {
   201,205,209,187,186,179,
   199,196,197,182,200,207,
   188
    };  // codepage 437
    const char bloco[4] = { 176,177,178,219 };  // 437
    const char olw = 34; // altura em linhas
    const char olh = 29;

    char topbar[256];
    char intbar[256];
    char lines[6][6] = {
        { 192, 196, 217, 179, 191, 218 },
        { 200, 205, 188, 186, 187, 201 },
        { 176, 176, 176, 176, 176, 176 },
        { 177, 177, 177, 177, 177, 177 },
        { 178, 178, 178, 178, 178, 178 },
        { 219, 219, 219, 219, 219, 219 }
    };
    // prepara a cor
    text_color(_verde_, _preto_);
    // prepara o box
    memset(topbar, simbolo[7], 34); // prepara a barra inferior
    topbar[olw] = 0;
    topbar[0] = simbolo[0];
    topbar[olw - 1] = simbolo[3];
    memset(intbar, ' ', olw); // prepara a barra interna
    intbar[olw] = 0;
    intbar[0] = simbolo[4];
    intbar[olw - 1] = simbolo[4];
    int boxL = l;
    int boxC = c;
    // log box position
    gotoYX(boxL, boxC);
    printf("%s", topbar); // acima
    gotoYX(boxL + olh - 1, boxC);
    topbar[0] = simbolo[0];
    topbar[olw - 1] = simbolo[0];
    printf("%s", topbar); // abaixo
    for (int b = 1; b < (olh - 1); b += 1)
    {
        gotoYX(boxL + b, boxC);
        printf("%s", intbar);
    };  // for()
    return 0;
};  // box()

int			cls()
{	// limpa a tela no windows, do jeito oficial
	CONSOLE_SCREEN_BUFFER_INFO		info;
	HANDLE		H = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD		origem = { 0,0 };
	DWORD			total;
	if (H == INVALID_HANDLE_VALUE) return -1;
	GetConsoleScreenBufferInfo(H, &info);
	int r = FillConsoleOutputCharacter(H, (TCHAR)' ',
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	int s = FillConsoleOutputAttribute(
		H, info.wAttributes,
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	SetConsoleCursorPosition(H, origem);
	return 0;
};	// end cls()

int         define_fonte(const LPCWSTR nome, const char X, const char Y)
{
    int res = 0;
    CONSOLE_SCREEN_BUFFER_INFOEX    console;
    CONSOLE_FONT_INFOEX             fonte;

    fonte.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    lstrcpyW(fonte.FaceName, nome);
    fonte.nFont = 0;
    fonte.dwFontSize.X = X;
    fonte.dwFontSize.Y = Y;
    fonte.FontFamily = 54;
    fonte.FontWeight = 400;
    res = SetCurrentConsoleFontEx(
        GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fonte);
    if (res == 0)
    {
        printf("SetCurrentConsoleFontEx() falhou: %d\n", GetLastError());
        exit(-1);
    };  // if()
    printf("fonte alterada com sucesso\n");
    _mostra_csb_info();
    return 0;
};  // define_fonte()

void	    gotoYX(int linha, int coluna)
{
    static COORD	coord;
    HANDLE			H = GetStdHandle(STD_OUTPUT_HANDLE);
    coord.X = (short) coluna; coord.Y = (short) linha;
    SetConsoleCursorPosition(H, coord);
    return;
};	// gotoXY

_cs_status* guarda()
{
    int res = 0;
    _cs_status* antes = (_cs_status*) malloc ( sizeof(_cs_status) );

    // Code pages
	antes->codePage =       GetConsoleCP();
	antes->outputCodePage = GetConsoleOutputCP();

    // locale
    char buffer[80];
    char* p = buffer;
	p = setlocale(LC_ALL, NULL);
    strcpy(antes->locale, p);

    // console info
    antes->console_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    res = GetConsoleScreenBufferInfoEx(
        GetStdHandle(STD_OUTPUT_HANDLE), &(antes->console_info)
    );
    if (res == 0)
    {
        printf("guarda() Erro em GetConsoleScreenBufferInfoEx(): %d\n",
            GetLastError());
        exit(-1);
    };  // if()

    // atributos, tamanho da janela no buffer
    antes->atributos = antes->console_info.wAttributes;
    antes->tamanho.X = antes->console_info.srWindow.Right
        - antes->console_info.srWindow.Left + 1;
    antes->tamanho.Y = antes->console_info.srWindow.Bottom  
        - antes->console_info.srWindow.Top + 1;

    // console mode: eco, insert mode etc
    GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &antes->modo);

    // agora a fonte
	antes->fonte.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	res = GetCurrentConsoleFontEx(
        GetStdHandle(STD_OUTPUT_HANDLE),
        FALSE, 
        &(antes->fonte));
	// A fonte em uso fica aqui no registro
	//    Computador\HKEY_LOCAL_MACHINE\SOFTWARE\
    //    Microsoft\Windows NT\CurrentVersion\Console\TrueTypeFont
	//
    return antes;
};  // guarda()

_cs_status* prepara_tela(const _cs_status* antes)
{
    _cs_status* config = (_cs_status*)malloc(sizeof(_cs_status));
    memcpy(
        &(config->console_info.ColorTable),
        &(antes->console_info.ColorTable),
        sizeof(antes->console_info.ColorTable)
    );
    // prepara a tela e devolve a configuracao
    // muda a fonte para Courier New
    define_fonte(L"Courier New", 13, 24);
    SetConsoleOutputCP(437);
    SetConsoleCP(437);

    // agora a tela 
    //CONSOLE_SCREEN_BUFFER_INFOEX  console_info;
    //CONSOLE_SCREEN_BUFFER_INFOEX* screen = &console_info;
    //DWORD   modo = antes->modo;
    //// DISABLE_NEWLINE_AUTO_RETURN 0x0008 
    //// ENABLE_WRAP_AT_EOL_OUTPUT   0x0002
    //modo = modo & 0xFFF5; //1111 0101
    //printf("modo: fff5 %d e modo antes = %d\n", 0xFFF5, modo);
    //SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), modo);
    //if (res == 0)
    //{
    //    printf("*Erro (%d) em SetConsoleMode()\n", GetLastError());
    //    exit(-5);
    //};
    // agora verifica os tamanhos da tela e de smallrect
    //printf("\tTamanho da tela dwSize: (%dx%d)\n",
    //    antes->console_info.dwSize.Y,
    //    antes->console_info.dwSize.X);
    //printf("\tSMALLRECT: (%dx%d)\n",
    //    antes->console_info.srWindow.Bottom -
    //    antes->console_info.srWindow.Top + 1,
    //    antes->console_info.srWindow.Right -
    //    antes->console_info.srWindow.Left + 1);

    // agora retira o scroll
    //COORD   xy;
    //xy.Y = screen->srWindow.Bottom - screen->srWindow.Top;
    //xy.X = screen->srWindow.Right - screen->srWindow.Left;
    //res = SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), xy);
    //if (res == 0)
    //{  
    //    res = GetLastError();
    //    printf("SetConsoleScreenBufferSize() falhou! Erro : %d \n",
    //        res);
    //    exit (-1);
    //};  // if()

    return 0;
};  // prepara_tela()

void		restaura(_cs_status* status)
{
	// restaura locale e paginas de codigo
    gotoYX(0, 0);
    cls();
    SetConsoleCP(status->codePage);
	SetConsoleOutputCP(status->outputCodePage);
	setlocale(LC_ALL, status->locale);

	// restaura a fonte
    status->fonte.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	int res = SetCurrentConsoleFontEx(
        GetStdHandle(STD_OUTPUT_HANDLE),
        FALSE, 
        &status->fonte);
	if (res == 0)
	    printf("SetCurrentConsoleFontEx() falhou: %d\n", GetLastError());
    printf("Restaurada a fonte\n"); fgetc(stdin);

	// console mode
	SetConsoleMode(
        GetStdHandle(STD_OUTPUT_HANDLE), status->modo );
    printf("Restaurado o modo da console para %d\n", status ->modo);
    printf("  ao final de restaura(). Tecle algo ==> "); fgetc(stdin);
    // cor e fundo
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        status->console_info.wAttributes
    );
    printf("Restaurados os atributos em uso para %d\n", status->console_info.wAttributes);
    printf("  ao final de restaura(). Tecle algo ==> "); fgetc(stdin);
    return;
};  // restaura()

void	    text_color(int letras, int fundo)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) letras | ((WORD)fundo << 4));
}	// text_color

int         _mostra_console(const char* msg)
{
    const char* _atributos[16] =
    {
        "preto",
        "azul",
        "verde",
        "ciano",
        "vermelho",
        "magenta",
        "marron",
        "cinza_claro",
        "cinza_escuro",
        "azul_claro",
        "verde_claro",
        "ciano_claro",
        "vermelho_claro",
        "magenta_claro",
        "amarelo",
        "branco"
    };

    cls();
    printf("\n\n==> Console Info | %s\n\n", msg);
    printf(("\tConsole CodePage %d\n\tConsole Output CodePage %d\n"),
        GetConsoleCP(),
        GetConsoleOutputCP());
    // locale
    char local[40];
    char* pChar = local;
    pChar = setlocale(LC_ALL, NULL);
    printf("\tLocale: '%s'\n", pChar);

    // console mode: eco, insert mode etc
    DWORD mode = 0;
    GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode);
    printf("\tConsole Mode: '%d'\n", mode);

    // fonte
    CONSOLE_FONT_INFOEX    fonte;
    fonte.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    int res = GetCurrentConsoleFontEx(
        GetStdHandle(STD_OUTPUT_HANDLE),
        FALSE,
        &fonte);
    if (res == 0)
    {
        printf("Erro %d em GetCurrentConsoleFontEx\n",
            GetLastError());
        exit(-1);
    };  // if()
    wprintf(L"\tFonte atual %s\n", fonte.FaceName);
    printf("\tSize (X,Y) = (%d,%d) Family = %d Weight = %d\n",
        fonte.dwFontSize.X, fonte.dwFontSize.Y,
        fonte.FontFamily, fonte.FontWeight);

    // console info
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info .cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx( GetStdHandle(STD_OUTPUT_HANDLE), &info );
    if (res == 0)
    {
        printf("Erro %d em GetConsoleScreenBufferInfoEx\n",
            GetLastError());
        exit(-2);
    };  // if()

    // atributos
    printf("\tAtributos: '%d'\n", info.wAttributes);

    // a cor:
    WORD letras = info.wAttributes & 0x0F;
    WORD fundo = (info.wAttributes & 0xF0) >> 4;

    printf("\tCores em uso: %s sobre %s\n",
        _atributos[letras], _atributos[fundo]);

    _mostra_csb_info();
    
    printf("\n\nAo final de mostra() ==> "); fgetc(stdin);
    return 0;
};  // _mostra_console()

int        _mostra_csb_info()
{   
    const char* _atributos[16] =
    {
        "preto",
        "azul",
        "verde",
        "ciano",
        "vermelho",
        "magenta",
        "marron",
        "cinza_claro",
        "cinza_escuro",
        "azul_claro",
        "verde_claro",
        "ciano_claro",
        "vermelho_claro",
        "magenta_claro",
        "amarelo",
        "branco"
    };

    // mostra a partir do buffer da console
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    CONSOLE_SCREEN_BUFFER_INFOEX* buffer = &info;
    buffer->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    int res = GetConsoleScreenBufferInfoEx(
        GetStdHandle(STD_OUTPUT_HANDLE), buffer );
    if (res == 0)
    {
        printf("guarda() Erro em GetConsoleScreenBufferInfoEx(): %d\n",
            GetLastError());
        exit(-1);
    };  // if()

    // SMALLRECT
    COORD tamanho;
    tamanho.X = buffer->srWindow.Right - buffer->srWindow.Left + 1;
    tamanho.Y = buffer->srWindow.Bottom - buffer->srWindow.Top + 1;
    printf("\n\tBuffer da console: (%d,%d)\n", buffer->dwSize.Y, buffer->dwSize.X);
    printf("\tSmallRect: (Y,X) = (%d,%d)\n",
        tamanho.Y, tamanho.X
    );

    printf("\tPosicao do Cursor: (%d,%d)\n",
        buffer->dwCursorPosition.Y, buffer->dwCursorPosition.X);

    printf("\twAttributes em uso: %d\n", buffer->wAttributes);
    printf("\tCores em uso: %s sobre %s\n",
        _atributos[buffer->wAttributes & 0x0F],
        _atributos[(buffer->wAttributes & 0xF0) >> 4]);

    printf("\tMaxima janela para essa fonte: (%d,%d)\n",
        buffer->dwMaximumWindowSize.Y, buffer->dwMaximumWindowSize.X);

    printf("\tPopupAttributes = %d\n", buffer->wPopupAttributes);
    printf("\tFullScreenSupported = %d\n", buffer->bFullscreenSupported);
    printf("\tCOLORTABLE nao usada\n");
    return 0;
};
