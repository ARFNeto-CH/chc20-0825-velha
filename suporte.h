#pragma once
// define para as cores para simplificar
#define		_preto_            0
#define		_azul_             1
#define		_verde_            2
#define		_ciano_            3
#define		_vermelho_         4
#define		_magenta_          5
#define		_marron_           6
#define		_cinza_claro_      7
#define		_cinza_escuro_     8
#define		_azul_claro_       9
#define		_verde_claro_     10
#define		_ciano_claro_     11
#define		_vermelho_claro_  12
#define		_magenta_claro_   13
#define		_amarelo_         14
#define		_branco_          15

#include <locale.h>
#include <stdio.h>
#include <windows.h>

typedef struct
{
	WORD	atributos;
	int     codePage;
	CONSOLE_SCREEN_BUFFER_INFOEX console_info;
	CONSOLE_FONT_INFOEX			 fonte;
	char    locale[80];
	FILE*	log;
	DWORD		modo;
	int     outputCodePage;
	COORD	tamanho;

}  _cs_status;


int			box(char,char);
int			cls();
int         define_fonte(const LPCWSTR, const char, const char);

_cs_status* guarda();
void		gotoYX(int,int);
_cs_status* prepara_tela(const _cs_status*);
void		restaura(_cs_status*);
void	    text_color(int,int);

// suporte
int         _mostra_console(const char*);
int			_mostra_csb_info();
