#define _CRT_SECURE_NO_WARNINGS 0

#define NUMBITS 5
#define CRIPTO CG_NONE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estegranografia.h"

int main(void)
{
	int op;
	char chave[256] = "";

	do
	{
		printf("\n\nMenu:");
		printf("\n1. Esconder um arquivo em uma imagem");
		printf("\n2. Resgatar um arquivo em uma imagem");
		printf("\n3. Sair");
		printf("\nOpcao: ");
		scanf("%d", &op);
		switch (op)
		{
			case 1:
			{
				char nomeArqInput[256];
				char nomeArqImagem[256];

				printf("\nDigite o nome do arquivo da imagem: ");
				scanf("%s", nomeArqImagem);
				printf("\nDigite o nome do arquivo a ser escondido: ");
				scanf("%s", nomeArqInput);

				if (CRIPTO == CG_RC4)
				{
					printf("\nDigite o chave para a criptografia: ");
					scanf("%s", chave);
				}

				ESTEG_Inicializar(NUMBITS, CRIPTO, chave);

				if (ESTEG_VerificarInput(nomeArqImagem, nomeArqInput) == 0)
				{
					char nomeArqImagemOutput[256];

					printf("\nDigite o nome do arquivo da imagem resultante (.bmp): ");
					scanf("%s", nomeArqImagemOutput);
					//strcpy(nomeArqImagemOutput, "saida.bmp");

					ESTEG_Esconder(nomeArqImagem, nomeArqInput, nomeArqImagemOutput);
				}
			}
			break;
			case 2:
			{
				char nomeArqImagem[256];

				printf("\nDigite o nome do arquivo da imagem: ");
				scanf("%s", nomeArqImagem);

				if (CRIPTO == CG_RC4)
				{
					printf("\nDigite o chave para a criptografia: ");
					scanf("%s", chave);
				}

				ESTEG_Inicializar(NUMBITS, CRIPTO, chave);

				if (ESTEG_VerificarOutput(nomeArqImagem) == 0)
				{
					char nomeArqOutput[256];

					printf("\nDigite o nome do arquivo a ser recuperado: ");
					scanf("%s", nomeArqOutput);

					ESTEG_Recuperar(nomeArqImagem, nomeArqOutput);
				}
			}
			break;
			case 3:
			break;
			default:
				printf("\nOpcao de menu invalida!\n");
			break;
		}
	} while (op != 3);

	return 0;
}