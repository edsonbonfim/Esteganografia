#ifndef _INCLUDE_CRIPTOGRAFIA_H
#define _INCLUDE_CRIPTOGRAFIA_H

enum funcionalidade { FC_CRIPTOGRAFAR = 0, FC_DESCRIPTOGRAFAR = 1 };

void CRIPTOG_CifraDeCesar(unsigned char *vet, int ini, int fim, int funcao);
void CRIPTOG_RC4(unsigned char *vet, int ini, int fim, char *chave);

#endif // !_INCLUDE_CRIPTOGRAFIA_H
