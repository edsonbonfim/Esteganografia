#define _CRT_SECURE_NO_WARNINGS 0

#define TAMBUF 10

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "estegranografia.h"
#include "criptografia.h"

using namespace cv;
using namespace std;

//////Variáveis globais///////////////////////////
int numBits = 1;
int cripto = CG_NONE;
char chave[256] = "";
//////////////////////////////////////////////////

///////////////////////Funções auxiliares//////////////////////
int ObterTamanhoArquivo(char *nome)
{
	int tam = 0;
	FILE *arq;

	arq = fopen(nome, "rb");

	if (arq != NULL)
	{
		fseek(arq, 0, SEEK_END);
		tam = ftell(arq);

		fclose(arq);
	}

	return tam;
}

///////////////////////////////////////////////////////////


int ESTEG_Inicializar(int iNumBits, int iCripto, char *sChave)
{
	if (iNumBits <= 0 || iNumBits > 8)
	{
		printf("\nO numero de bits deve estar entre 1 e 8!");
		return 1;
	}

	if (iCripto < CG_NONE || iCripto > CG_RC4)
	{
		printf("\nMetodo de criptografia utilizado não definido!");
		return 1;
	}

	numBits = iNumBits;
	cripto = iCripto;
	strcpy(chave, sChave);
	return 0;
}

int ESTEG_VerificarInput(char *nomeArqImagem, char *nomeArqInput)
{
	Mat image;
	image = imread(nomeArqImagem, IMREAD_COLOR); // Read the file
	if (image.empty()) // Check for invalid input
	{
		printf("\nArquivo de imagem nao encontrado!");
		return 1;
	}

	int tamPossivel = (int)((image.size().height*image.size().width * 3 * numBits) / 8);

	image.deallocate();

	FILE *arq = fopen(nomeArqInput, "rb");
	if (arq == NULL)
	{
		printf("\nArquivo de entrada nao encontrado!");
		return 1;
	}	
	
	int tamInput = ObterTamanhoArquivo(nomeArqInput);
	tamInput += (int)(tamInput / TAMBUF);
	tamInput++;

	if (tamInput > tamPossivel)
	{
		printf("\nO arquivo de entrada nao cabe na imagem!");
		return 1;
	}

	return 0;
}

int ESTEG_VerificarOutput(char *nomeArqImagem)
{
	Mat image;
	image = imread(nomeArqImagem, IMREAD_COLOR); // Read the file
	if (image.empty()) // Check for invalid input
	{
		printf("\nArquivo de imagem nao encontrado!");
		return 1;
	}

	return 0;
}

void Criptografia(unsigned char *vet, int ini, int fim, int funcao)
{
	switch (cripto)
	{
	case CG_CESAR:
		CRIPTOG_CifraDeCesar(vet, ini, fim, funcao);
		break;
	case CG_RC4:
		CRIPTOG_RC4(vet, ini, fim, chave);
		break;
	}
}

void ESTEG_Esconder(char *nomeArqImagem, char *nomeArqInput, char *nomeArqImagemOutput)
{
	FILE *arq;

	arq = fopen(nomeArqInput, "rb");

	Mat imageA = imread(nomeArqImagem, IMREAD_COLOR);
	Mat imageB;

	imageA.copyTo(imageB);

	int linhaAtual = 0;
	int colunaAtual = 0;
	int indice = 0;
	int bit = 0;

	Vec3b val;
	val = imageA.at<Vec3b>(linhaAtual, colunaAtual);

	do
	{
		int i;
		unsigned char ch[TAMBUF+2];
		size_t num = fread(ch, sizeof(unsigned char), TAMBUF, arq);
		Criptografia(ch, 0, TAMBUF - 1, FC_CRIPTOGRAFAR);

		int cont = (int)num;

		for (i = cont; i > 0; i--)
		{
			ch[i] = ch[i - 1];
		}
		ch[0] = cont;
		cont++;

		if (feof(arq))
		{
			ch[cont] = 0;
			cont++;
		}

		for (i = 0; i < cont; i++)
		{
			unsigned char c = ch[i];
			int j;

			for (j = 0; j < 8; j++)
			{
				unsigned char aux = 1;
				unsigned char valor;

				aux = aux << (8 - j - 1);
				valor = aux & c;

				if (valor == 0)
				{
					aux = 1;
					aux = aux << (numBits - bit - 1);
					aux = ~aux;

					val[indice] = val[indice] & aux; 
				}
				else
				{
					aux = 1;
					aux = aux << (numBits - bit - 1);

					val[indice] = val[indice] | aux;
				}

				//Atualizando as variáveis
				if (bit < numBits - 1)
				{
					bit++;
				}
				else if (indice < 2)
				{
					indice++;
					bit = 0;
				}
				else if (colunaAtual<imageA.size().width - 1)
				{
					imageB.at<Vec3b>(linhaAtual, colunaAtual) = val;
					colunaAtual++;
					val = imageA.at<Vec3b>(linhaAtual, colunaAtual);
					indice = 0;
					bit = 0;
				}
				else
				{
					imageB.at<Vec3b>(linhaAtual, colunaAtual) = val;
					linhaAtual++;
					colunaAtual = 0;
					val = imageA.at<Vec3b>(linhaAtual, colunaAtual);
					indice = 0;
					bit = 0;
				}
			}
		}
	} while (!feof(arq));

	if (indice > 0)
	{
		imageB.at<Vec3b>(linhaAtual, colunaAtual) = val;
	}

	imwrite(nomeArqImagemOutput, imageB);

	fclose(arq);
}

void ESTEG_Recuperar(char *nomeArqImagem, char *nomeArqOutput)
{
	FILE *arq;

	arq = fopen(nomeArqOutput, "wb");

	Mat imageA = imread(nomeArqImagem, IMREAD_COLOR);
	int linhaAtual = 0;
	int colunaAtual = 0;

	unsigned char c = 0;
	int cont = 0;
	int total = -1;
	unsigned char ch[TAMBUF+2];

	Vec3b val;
	val = imageA.at<Vec3b>(linhaAtual, colunaAtual);
	int indice = 0;
	int bit = 0;

	do
	{
		int i;
		
		for (i = 0; i < 8; i++)
		{
			unsigned char aux = 1;
			unsigned char valor;

			aux = aux << (numBits - bit - 1);
			valor = aux & val[indice];

			if (valor == 0)
			{
				aux = 1;
				aux = aux << (8 - i - 1);
				aux = ~aux;

				c = c & aux;
			}
			else
			{
				aux = 1;
				aux = aux << (8 - i - 1);

				c = c | aux;
			}

			//Atualizando as variáveis
			if (bit < numBits - 1)
			{
				bit++;
			}
			else if (indice < 2)
			{
				indice++;
				bit = 0;
			}
			else if (colunaAtual<imageA.size().width - 1)
			{
				colunaAtual++;
				val = imageA.at<Vec3b>(linhaAtual, colunaAtual);
				indice = 0;
				bit = 0;
			}
			else
			{
				linhaAtual++;
				colunaAtual = 0;
				val = imageA.at<Vec3b>(linhaAtual, colunaAtual);
				indice = 0;
				bit = 0;
			}
		}

		if (total == -1)
		{
			total = c;
			c = 0;
		}
		else
		{
			ch[cont] = c;
			cont++;
			c = 0;
		}

		if (total != 0 && cont == total)
		{
			Criptografia(ch, 0, cont - 1, FC_DESCRIPTOGRAFAR);
			fwrite(ch, sizeof(unsigned char), cont, arq);
			cont = 0;
			total = -1;
		}
	} while (total != 0);

	fclose(arq);
}
