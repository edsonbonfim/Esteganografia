#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CRIPTOG_CifraDeCesar(unsigned char *vet, int ini, int fim, int funcao)
{
	if (funcao == 0)   //criptografar
	{
		int i;
		for (i = ini; i <= fim; i++)
		{
			vet[i] = (vet[i] + 3) % 256;
		}
	}
	else //descriptografar
	{
		int i;
		for (i = ini; i <= fim; i++)
		{
			vet[i] = (vet[i] + 256 - 3) % 256;
		}
	}
}

void Swap(unsigned char val1, unsigned char val2)
{
	val1 = (unsigned char)(val1 ^ val2); // val1 XOR val2
	val2 = (unsigned char)(val1 ^ val2);
	val1 = (unsigned char)(val1 ^ val2);
}

void KSA(unsigned char *S, char *chave)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		S[i] = (unsigned char)i;
	}

	int j = 0;

	for (int i = 0; i < 256; i++)
	{
		j = (j + S[i] + chave[i % strlen(chave)]) % 256;
		Swap(S[i], S[j]);
	}
}

void CRIPTOG_RC4(unsigned char *vet, int ini, int fim, char *chave)
{
	int i = 0, j = 0, k;
	// a mensagem original está armazenada na variável "input".
	unsigned char *result = (unsigned char*) malloc((fim+1)*sizeof(unsigned char));

	unsigned char S[256];
	KSA(S, chave);

	for (k = ini; k <= fim; k++)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		Swap(S[i],S[j]);
		result[k] = (unsigned char)(S[(S[i] + S[j]) % 256] ^ vet[k]);
	}

	for (k = ini; k <= fim; k++)
	{
		vet[k] = result[k];
	}

	free (result);
}
