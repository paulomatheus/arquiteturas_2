#include <iostream>
#include<cstdlib> 
#include<string>
/*
	Alunos:
	Breno Costa Ferreira Alves - 1078
	Lucas Negrão Reys Sarno - 1099
	Paulo Matheus de Araujo Silva - 1311
	
	---------------------------------EPC1---------------------------------------	
	
	Arquitetura de 16 bits
	(Endereço 1) --- (Endereço 2) --- ( Endereço Destino ) --- ( OP CODE )  ADD
		0000 	 ---	0001  	  ---		0010 		   ---	0000    
	(Endereço 1) --- (Endereço 2) --- ( Endereço Destino ) --- ( OP CODE )  SUB
		0000 	 ---	0001  	  ---		0010 		   ---	0001   
	(Endereço 1) ---         ( Endereço Memo Dados) 	   --- ( OP CODE )  LOAD
		0000     ---             00000001            	   ---	0010     
	(Endereço 1) ---         ( Endereço Memo Dados) 	   --- ( OP CODE )  STORE
	    0000     ---             00000001           	   ---	0011 
	
	Código das intruções:
	Tipo de instrução 0 -> ADD 0000  
	Tipo de instrução 1 -> SUB 0001  
	Tipo de instrução 2 -> LOAD 0010
	Tipo de instrução 3 -> STORE 0011 
	
	---------------------------------EPC2---------------------------------------
	
	Memória Cache
	4 palavras, 4 linhas
	tag = 16 - ( 2 + 2 ) = 12 bits
	
*/

using namespace std;



struct cache	
{
	int tag;
	bool bitValid;
	int dados[4]; 	
};

cache memoriaCache[3];
int Pc;
int Reg1;
int Reg2;
int RegDest;
int tipoInstruc;
int Instruc;
int RegMemo;
int Reg[10];


// Função usando os endereços + os op codes
unsigned int memoriaProgram[]{
	//load reg0-> reg0
	0b0000000000000010,
	//load reg1->reg1 
	0b0001000000010010,
	//add reg0+reg1 = reg2 
	0b0000000100100000,
	//load reg4->reg3
	0b0011000001000010,
	//sub reg2-reg3 = reg5 
	0b0010001101010001,
	//store reg5 -> reg6
	0b0101000010100011, 
};

//Função para declarar o valor inicial de cada posição do vetor
unsigned int memoriaDados[]{
	5,5,5,3,3,1,1,1,1,1,1,1,1,1,1,1
};

//Funcao para preencher os endereços de memória
int carregaCache(int programCount)
{
	int palavra,linha,aux,tag;
	
	palavra = programCount & 0b0000000000000011;
	linha = programCount & 0b0000000000001100;
	linha = linha >> 2;
	tag = memoriaProgram[programCount+1] >>4;
	
	aux = programCount - palavra;

	for(int i = 0; i < 4; i++)
	{		
		memoriaCache[linha].dados[i] = memoriaProgram[aux+i]; 		
	}	
	memoriaCache[linha].bitValid = true;
	memoriaCache[linha].tag = tag;
	
	return 	memoriaCache[linha].dados[palavra];
}

//Funcao para dizer se o endereco de memória encontrou "hit" ou "miss"
int validaCache(int programCounter)
{
	int linha,tag,palavra;
	
	palavra = programCounter & 0b0000000000000011;
	tag = memoriaProgram[programCounter] & 0b1111111111110000;
	tag = tag >> 4;
    linha = programCounter & 0b0000000000001100;
    linha = linha >> 2;
    
	if(memoriaCache[linha].bitValid == true)
	{
		
		if(memoriaCache[linha].tag == tag)
		{
			cout << " hit " << endl;
			return memoriaCache[linha].dados[palavra];
				
		}
		else {
			cout << " miss " << endl;
			return carregaCache(programCounter);
		 }		
	}
	else {
		cout << " miss " << endl;
		return carregaCache(programCounter);
	}
	
}



//Função de leitura dos OP codes, mascarando os bits e deslocando para posicao certa
void decode(void){
	tipoInstruc = Instruc & 0b0000000000001111;
	// add
	if(tipoInstruc == 0 || tipoInstruc == 1){
		Reg1 = Instruc >> 12;
		Reg1 = Reg1 & 0b0000000000001111;
		Reg2 = Instruc >> 8;
		Reg2 = Reg2 & 0b0000000000001111;
		RegDest = Instruc >> 4;
		RegDest = RegDest & 0b0000000000001111;

	}
	// load
	else if(tipoInstruc == 2){
		RegDest = Instruc >> 12;
		RegDest = RegDest & 0b0000000000001111;
		RegMemo = Instruc >> 4;
		RegMemo = RegMemo & 0b0000000011111111;
	}
	// store
	else if(tipoInstruc == 3){
		Reg1 = Instruc >> 12;
		Reg1 = Reg1 & 0b0000000000001111;
		RegMemo = Instruc >> 4;
		RegMemo = RegMemo & 0b0000000011111111;
	}
	    
}

// Função para realizar todas as instruções
void execute(void){ 
	tipoInstruc = Instruc & 0b0000000000001111;
	// add
	if(tipoInstruc == 0){
	    Reg[RegDest] = Reg[Reg1] + Reg[Reg2];
	}
	// sub
	else if(tipoInstruc == 1){
		Reg[RegDest] = Reg[Reg1] - Reg[Reg2];
	}
	// load
	else if(tipoInstruc == 2) {
		Reg[RegDest] = memoriaDados[RegMemo]; 
	}
	// store
	else if(tipoInstruc == 3){
		memoriaDados[RegMemo] = Reg[Reg1];
	}
}


int main()
{
	Pc = 0;
	//Inicializando o vetor
	for(int i = 0; i < 10; i++)
	{
		Reg[i] = 0;
		memoriaCache[i].bitValid = 0;
	}
	//Executando o código
	while(Pc < 6)
	{
		Instruc = validaCache(Pc);
		Pc++;
		decode();   
		execute();
	}

	return 0;
}

