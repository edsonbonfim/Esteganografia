#ifndef _INCLUDE_ESTEGRANOGRAFIA_H
#define _INCLUDE_ESTEGRANOGRAFIA_H

enum criptografia { CG_NONE = 0, CG_CESAR = 1, CG_RC4 = 2 };

int ESTEG_Inicializar(int iNumBits, int iCripto, char *sChave);
int ESTEG_VerificarInput(char *nomeArqImagem, char *nomeArqInput);
int ESTEG_VerificarOutput(char *nomeArqImagem);
void ESTEG_Esconder(char *nomeArqImagem, char *nomeArqInput, char *nomeArqImagemOutput);
void ESTEG_Recuperar(char *nomeArqImagem, char *nomeArqOutput);

#endif // !_INCLUDE_ESTEGRANOGRAFIA_H

