#include <iostream>

using namespace std;
/*
	Alunos:
	Breno
	Lucas Negrão
	Paulo Matheus
		
	
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
*/


// Declaração de variáveis
unsigned int Pc;
unsigned int RegMemo;
unsigned int Reg[10];
unsigned int Reg1;
unsigned int Reg2;
unsigned int RegDest;
unsigned int tipoInstruc;
unsigned int Instruc;


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
int main(){
	Pc = 0;
	//Inicializando o vetor
	for(int i = 0; i < 10; i++){
		Reg[i] = 0;
	}
	//Executando o código
	while(Pc < 6){
		Instruc = memoriaProgram[Pc]; 
		Pc++;
		decode();   
		execute();
	}

	return 0;
}