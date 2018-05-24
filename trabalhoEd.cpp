/* 
  --Trabalho de Estrutura de Dados.
  --Sistema de Cadastro em Hashing Extensível
  --Copyright 2018 by Lucas Neves, Andrew e Vinicius.  
  --Arquivo contém as implementações das classes;
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "trabalhoEd.h"
#define Cap_TAB 16
#define CAP_BLOCO 4

/*
	int id;
	char nome[50];
	char dominio[10];
	char biografia[200]
*/

int FuncaoHash (int i) {
	int h = i % Cap_TAB;
	return ConverteBinario(h);
}

BlocosDados::BlocosDados () {
	for (int i = 0; i < CAP_BLOCO; ++i) {
		mBloco[i].id = -1;
		mBloco[i].nome[50] = ' ';
		mBloco[i].dominio[10] = ' ';
		mBloco[i].biografia[200] = ' ';
	}
	mTamBloco = 0;
	mUso = false;
}

void BlocosDados::Insere (Dado deus) {
	if (!EmUso()) {
		mBloco[mTamBloco] = deus;
		++mTamBloco;
		mUso = true;
		mCabecalho = FuncaoHash(deus.id);
	} else if (BlocoCheio()) {
		cout << "|***************[ERRO]***************|" << endl;
		cout << "| O bloco está cheio.                |" << endl;
		cout << "| Não é possível inserir mais dados. |" << endl;
		cout << "|************************************|" << endl;
		exit(EXIT_FAILURE);
	} else {
		mBloco[mTamBloco] = deus;
		++mTamBloco;
	}
}

BlocosDados::~BlocosDados () {
	for (int i = 0; i < CAP_BLOCO; ++i) {
		mBloco[i].id = -1;
		mBloco[i].nome[50] = ' ';
		mBloco[i].dominio[10] = ' ';
		mBloco[i].biografia[200] = ' ';
	}
	mTamBloco = 0;
	mCabecalho = -1;
}

bool BlocosDados::EmUso () {
	return mUso;
}

bool BlocosDados::BlocoCheio () {
	return (mTamBloco == 4);
}

void BlocosDados::Imprime () {
	cout << "Cabeçalho" << mCabecalho << endl;
	for (int i = 0; i < mTamBloco; ++i) {
		cout <<" Id: " << mBloco[i].id << endl;
		cout <<"Nome : " << mBloco[i].nome << endl;
		cout <<"Dominio : " << mBloco[i].dominio << endl;
		cout <<"Biografia: " << endl;
		cout << mBloco[i].biografia << endl;
	}
	cout << "Tamanho: " << mTamBloco << endl;
}

TabelaH::TabelaH (int cap)  {
	mCapacidade = cap;
	mElementos = new int[mCapacidade];
	for (int i = 0; i < mCapacidade; ++i) {
		mElementos[i] = -1;
	}
}

void TabelaH::Insere (Dado deus) {
	int pos = ConverteDecimal(FuncaoHash(deus.id));
	if (PosOcupada(pos)) { // Bloco existe
		BlocosDados* auxBloco = new BlocosDados;
		CarregaBloco(auxBloco, mElementos[pos]);
		auxBloco->Insere(deus);
		EscreveArquivo(auxBloco, mElementos[pos]);
		int bytes = auxBloco->PosicaoArquivo(auxBloco);
		mElementos[pos] = bytes;
		delete auxBloco;
		// Posição no arquivo = mElementos[pos]
		// Puxa no arquivo binário o bloco
		// Verifica se o bloco não está cheio
	} else { // Bloco não existe
		// Crie um novo bloco
		BlocosDados* novoBloco = new BlocosDados;
		novoBloco->Insere(deus);
		EscreveArquivo(novoBloco, mElementos[pos]);
		int bytes = novoBloco->PosicaoArquivo(novoBloco);
		mElementos[pos] = bytes;
		delete novoBloco;
	}
}

void TabelaH::LeTabelaArquivo () {
	cout << "Aqui3 !!!" << endl;
	ifstream leituraTH("Enderecos.txt");
  	int posTH, byte;
  	while (!leituraTH.eof()) {
  		leituraTH >> posTH;
  		leituraTH >> byte;
  		mElementos[posTH] = byte;
  	}
  	leituraTH.close();
  	cout << "Aqui4 !!!" << endl;
}

 void TabelaH::Imprime () {
 	for (int i = 0; i < mCapacidade; ++i) {
 		cout << ConverteBinario(i) << " " <<mElementos[i] << endl;
 	}
 }

TabelaH::~TabelaH () {
	ofstream saida("uppsala.txt");
	for (int i = 0; i < mCapacidade; ++i) {
		saida << i  << endl;
	}
	saida.close();

	delete[] mElementos;
}

bool TabelaH::PosOcupada (int pos) {
	if (mElementos[pos] == -1) {
		return false;
	} else {
		return true;
	}
}

int ConverteBinario (int decimal) {
    int aux[4];
    int resto, binario;

    aux[3] = decimal % 2;
    resto = decimal / 2;
    aux[2] = resto % 2;
    resto = resto / 2;
    aux[1] = resto % 2;
    resto = resto / 2;
    aux[0] = resto % 2;

    binario = aux[0]*1000 + aux[1]*100 + aux[2]*10 + aux[3]*1;

    return binario;
}

int ConverteDecimal (int binario) {
    int aux[4];
    int decimal;

    aux[0] = binario / 1000;
    binario = binario % 1000;
    aux[1] = binario / 100;
    binario = binario % 100;
    aux[2] = binario / 10;
    aux[3] = binario % 10;

    decimal = (int)(aux[0] * pow(2, 3) + aux[1] * pow(2, 2) + aux[2] * pow(2, 1) + aux[3] * pow(2, 0));
    return decimal;
}

void InsereDados (TabelaH* tabelaCadastro) {
	Dado deus;
	// Leitura de Dados:
	cout << "Entre com o id do deus:" << endl;
	cin >> deus.id;
	cin.ignore();
	cout << "Entre com o nome do deus:" << endl;
	cin.getline(deus.nome, 50);
	cout << "Entre com o domínio do deus:" << endl;
	cin.getline(deus.dominio, 10);
	cout << "Entre com a biografia do deus:" << endl;
	cin.getline(deus.biografia, 200);
	
	tabelaCadastro->Insere(deus);
}

void EscreveArquivo (BlocosDados* auxBloco, int posBytes) {
	ofstream salva("asgard.bin", ios::binary|ios::app);
	if (salva.eof()) {
		salva.write(reinterpret_cast<const char*> (&auxBloco), sizeof(BlocosDados));
	} else {
		salva.seekp(posBytes, ios::cur);
		salva.write((const char*) (&auxBloco), sizeof(BlocosDados));
	}
	salva.close();
}

int BlocosDados::PosicaoArquivo (BlocosDados* novoBloco) {
	ifstream Leitura;
	int pos;
  	Leitura.open("asgard.bin", ios::binary);
  	if (Leitura) {
		Leitura.seekg(0, Leitura.end);
		int tamArq = Leitura.tellg();
		Leitura.seekg(0, Leitura.beg);
		int tamBloc = tamArq / sizeof(BlocosDados);
		BlocosDados* aux = new BlocosDados;
		int pos = 0;
		for (int i = 0; i < tamBloc; i++) {
  			Leitura.read((char*)(&aux), sizeof(BlocosDados));
  			if (aux->mCabecalho == novoBloco->mCabecalho) {
				pos = i*sizeof(BlocosDados);
				cout << endl << "posição em bytes : " << pos  << endl <<endl;
			}
  		}
  	} if (!Leitura) {
  		cout << endl << "Essa deus não está cadastrado" << endl;
  		pos = -1;
  	}
  	Leitura.close();
  	return pos;	
}

void CarregaBloco (BlocosDados* auxBloco, int posBytes) {
	ifstream Carregar;
	Carregar.open("asgard.bin", ios::binary);
	if (Carregar) {
		Carregar.seekg(posBytes, ios::cur);
		Carregar.read((char*)(&auxBloco), sizeof(BlocosDados));
	} else {
		cout << endl <<"Erro na leitura do arquivo ou arquivo inesistente !" << endl << endl;
	}
	Carregar.close();
}

void RemoveDados () {

	return;
}

void ConsultaDados () {

	return;
}

void ImprimeArquivoOrdem () {
	ifstream leitura;
	leitura.open("asgard.bin", ios::binary);
	cout << "OPA !!" << endl;
	if (leitura){
		leitura.seekg(0, leitura.end);
		int tamArq = leitura.tellg();
		leitura.seekg(0, leitura.beg);
		
		int quantBloc = tamArq / sizeof(BlocosDados);
  		BlocosDados* auxBloco = new BlocosDados;
  		cout << "OPA 2 !!" << endl;
  		for(int i = 0; i < quantBloc; i++){
       		leitura.read((char*)(&auxBloco), sizeof(BlocosDados));
       		auxBloco->Imprime();
  		}
  		leitura.close();
	}
	cout << "OPA3 !!" << endl;
}
void ImprimeBlocoOrdem () {return;}

void Menu() {
	cout << endl;
	cout << "              |*********************[MENU]*************************|    " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "             | |        Entre com a opção desejada:               | |   " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (1) - para inserir um novo deus no arquivo:        |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (2) - para remover um deus do arquivo:             |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (3) - para consultar um deus no arquivo:           |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (4) - para imprimir todos registros:               |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (0) - para sair:                                   |    " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "             | |                                                  | |   " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "              |****************************************************|    " << endl;
	cout << endl;
}
