/* 
  --Trabalho de Estrutura de Dados.
  --Sistema de Cadastro em Hashing Extensível
  --Copyright 2018 by Lucas Neves, Andrew e Vinicius.  
  --Arquivo contém a implementação das classes e dos módulos;
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "metodos.hpp"

// Constantes
#define CAP_BLOCO 4
#define CAP_TABELA 16

using namespace std;

// Início da implementação do bloco

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

void BlocosDados::InsereBloco (Dado deus) {
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

int BlocosDados::PosicaoArquivo (BlocosDados* novoBloco) {
	ifstream Leitura;
	int pos = 0;
  	Leitura.open("asgard.bin", ios::binary);
  	if (Leitura) {
		Leitura.seekg(0, Leitura.end);
		int tamArq = Leitura.tellg();
		Leitura.seekg(0, Leitura.beg);
		int qntBloc = tamArq / sizeof(BlocosDados);
		BlocosDados* aux = new BlocosDados;
		for (int i = 0; i < qntBloc; ++i) {
  			Leitura.read((char*)(aux), sizeof(BlocosDados));
  			if (aux->mCabecalho == novoBloco->mCabecalho) {
				pos = i*sizeof(BlocosDados);
			}
  		}
  	} else if (!Leitura) {
  		cout << endl << "Essa deus não está cadastrado" << endl;
  		pos = -1;
  	}
  	Leitura.close();
  	cout << "posBytes: " << pos << endl; 
  	return pos;	
}

bool BlocosDados::EmUso () {
	return mUso;
}

bool BlocosDados::BlocoCheio () {
	return (mTamBloco == 4);
}

void BlocosDados::ImprimeBloco () {
	cout << ">>>>>>>>>>>>>>>>>>>>>>[BLOCO]<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << "Tamanho: " << mTamBloco << endl;
	cout << "Cabeçalho: " << mCabecalho << endl;
	for (int i = 0; i < mTamBloco; ++i) {
		cout << "=====================(Deus"<< " " << i <<")======================" << endl;
		cout << "ID: " << mBloco[i].id << endl;
		cout << "Nome:" << mBloco[i].nome << endl;
		cout << "Dominio: " << mBloco[i].dominio << endl;
		cout << "Biografia: " << mBloco[i].biografia << endl;
		cout << "===================================================" << endl;
	}
	cout << endl;
}

void BlocosDados::ImpressaoConsulta (int id) {
	bool existe = false;
	for (int i = 0; i < mTamBloco; ++i) {
		if (mBloco[i].id == id) {
			cout << "======================(Deus)======================" << endl;
			cout << "ID: " << mBloco[i].id << endl;
			cout << "Nome:" << mBloco[i].nome << endl;
			cout << "Dominio: " << mBloco[i].dominio << endl;
			cout << "Biografia: " << mBloco[i].biografia << endl;
			cout << "===================================================" << endl;
			existe = true;
		}
	}
	if (!existe)
		cout << "Esse id não está na cadastrado " << endl;
}

int BlocosDados::PosDeus (int id) {
	/*bool existe = false;
	for (int i = 0; i < mTamBloco; ++i) {
		if (mBloco[i].id == id) {
			cout << "======================(Deus)======================" << endl;
			cout << "ID: " << mBloco[i].id << endl;
			cout << "Nome:" << mBloco[i].nome << endl;
			cout << "Dominio: " << mBloco[i].dominio << endl;
			cout << "Biografia: " << mBloco[i].biografia << endl;
			cout << "===================================================" << endl;
			existe = true;
		}
	}
	if (!existe)
		cout << "Esse id não está na cadastrado " << endl;
		*/	
}

// Fim da implementação do bloco

// Início da implementação da tabela

TabelaH::TabelaH (int cap)  {
	mCapacidade = cap;
	mElementos = new int[mCapacidade];
	for (int i = 0; i < mCapacidade; ++i) {
		mElementos[i] = -1;
	}
}

TabelaH::~TabelaH () {
	ofstream saida("uppsala.txt");
	for (int i = 0; i < mCapacidade; ++i) {
		cout << "posTH: " << ConverteBinario(i) << "byte: " << mElementos[i] << endl;
  		cout <<  endl;
		saida << ConverteBinario(i) << "\t" << mElementos[i] << endl;
	}
	saida.close();

	delete[] mElementos;
}

void TabelaH::InsereTabela (Dado deus) {
	int pos = ConverteDecimal(FuncaoHash(deus.id));
	if (PosOcupada(pos)) { // Bloco existe
		BlocosDados* auxBloco = new BlocosDados;
		CarregaBloco(auxBloco, mElementos[pos]);
		auxBloco->InsereBloco(deus);
		EscreveArquivoVelho(auxBloco, mElementos[pos]);
		delete auxBloco;
		// Posição no arquivo = mElementos[pos]
		// Puxa no arquivo binário o bloco
		// Verifica se o bloco não está cheio
	} else { // Bloco não existe
		// Crie um novo bloco
		BlocosDados* novoBloco = new BlocosDados;
		novoBloco->InsereBloco(deus);
		EscreveArquivoNovo(novoBloco);
		int bytes = novoBloco->PosicaoArquivo(novoBloco);
		mElementos[pos] = bytes;
		delete novoBloco;
	}
}

void TabelaH::LeTabelaArquivo () {
	ifstream leituraTH("uppsala.txt");
  	int posTH, byte;
  	if (leituraTH.is_open()) {
  		for (int i = 0; i < CAP_TABELA; ++i) {
  			leituraTH >> posTH;
  			leituraTH >> byte;
  			mElementos[ConverteDecimal(posTH)] = byte;
  		}
  		leituraTH.close();
  		cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  		ImprimeTabela();
  	}
}

 void TabelaH::ImprimeTabela () {
 	for (int i = 0; i < mCapacidade; ++i) {
 		cout << ConverteBinario(i) << " - " << mElementos[i] << endl;
 	}
 }

bool TabelaH::PosOcupada (int pos) {
	if (mElementos[pos] == -1) {
		return false;
	} else {
		return true;
	}
}

int TabelaH::PosicaoBytes (int h) {
	return mElementos[h];
}

// Fim da implementação da tabela
// Início da implementação dos módulos globais

int FuncaoHash (int i) {
	int h = i % CAP_TABELA;
	return ConverteBinario(h);
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
	
	tabelaCadastro->InsereTabela(deus);
}

void EscreveArquivoNovo (BlocosDados* auxBloco) {
	ofstream salva("asgard.bin", ios::binary|ios::app);
	if (salva.is_open()) {
		salva.write(reinterpret_cast<const char*> (auxBloco), sizeof(BlocosDados));
	}	
	salva.close();
}

void EscreveArquivoVelho (BlocosDados* auxBloco, int posBytes) {
	fstream salva("asgard.bin", ios::binary| ios::out| ios::in);
	if (salva.is_open()) {
		salva.seekp(posBytes, ios::cur);
		salva.write(reinterpret_cast<const char*> (auxBloco), sizeof(BlocosDados));
	}	
	salva.close();
}

void CarregaBloco (BlocosDados* auxBloco, int posBytes) {
	ifstream Carregar;
	Carregar.open("asgard.bin", ios::binary);
	if (Carregar.is_open()) {
		Carregar.seekg(posBytes, ios::cur);
		Carregar.read((char*)(auxBloco), sizeof(BlocosDados));
	} else {
		cout << endl <<"Erro na leitura do arquivo ou arquivo inesistente !" << endl << endl;
	}
	Carregar.close();
}

void RemoveDados () {
	int id, numH;
	cout << "Insira um ID: " << endl;
	cin >> id;
	numH = ConverteDecimal(FuncaoHash(id));
	if (tabelaCadastro->PosOcupada(numH)) {
		BlocosDados* blocRem = new BlocosDados;
		CarregaBloco(blocRem, tabelaCadastro->PosicaoBytes(numH));
		blocRem->PosDeus(id);
	}
	return;
}

void ConsultaDados (TabelaH* tabelaCadastro) {
	int id, numH;
	cout << "Insira um ID: " << endl;
	cin >> id;
	numH = ConverteDecimal(FuncaoHash(id));
	if (tabelaCadastro->PosOcupada(numH)) {
		BlocosDados* blocCons = new BlocosDados;
		CarregaBloco(blocCons, tabelaCadastro->PosicaoBytes(numH));
		blocCons->ImpressaoConsulta(id);
		delete blocCons;
	}
}

void ImprimeArquivoOrdem () {
	ifstream leitura;
	leitura.open("asgard.bin", ios::binary);
	if (leitura.is_open()) {
		BlocosDados* impTodo = new BlocosDados;
		leitura.seekg(0, ios::end);
		int tamArq = leitura.tellg();
		leitura.seekg(0, ios::beg);
		int var = sizeof(BlocosDados);
		int qntBloco = tamArq/var;
		for (int i = 0; i < qntBloco; ++i) {		
			leitura.read(reinterpret_cast<char*> (impTodo), sizeof(BlocosDados));
			impTodo->ImprimeBloco();
		}
	}
	
  	leitura.close();
}
void ImprimeBlocoOrdem (TabelaH* tabelaCadastro) {
	int numBin;
	tabelaCadastro->ImprimeTabela();
	cout << endl << "Insira uma posicao em número binário correspondente a Tabela: " << endl;
	cin >> numBin;
	if (tabelaCadastro->PosicaoBytes(ConverteDecimal(numBin)) == -1) {
		cout << "Posição Desocupada ! " << endl << endl;
		char opcao;
		cout << "Deseja sair da posição [y/n] " << endl;
		cin >> opcao;
		if (opcao == 'n') {
			ImprimeBlocoOrdem(tabelaCadastro);
			cout << endl;
		} else if (opcao != 'n') {
			void Menu();
		}
	} else {
		BlocosDados* blocOrd = new BlocosDados;
		CarregaBloco(blocOrd, tabelaCadastro->PosicaoBytes(ConverteDecimal(numBin)));
		blocOrd->ImprimeBloco();
	}
}

void Menu() {
	cout << endl;
	cout << "              |&&&&&&&&&&&&&&&&&&&&&[MENU]&&&&&&&&&&&&&&&&&&&&&&&&&|    " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "             | |        Entre com a opção desejada:               | |   " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (1) - para inserir um novo deus no arquivo:        |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (2) - para remover um deus do arquivo:             |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (3) - para consultar um deus no arquivo:           |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (4) - para imprimir todos registros:               |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (5) - para imprimir Bloco:                         |    " << endl;
	cout << "              |                                                    |    " << endl;
	cout << "              | (0) - para sair:                                   |    " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "             | |                                                  | |   " << endl;
	cout << "              =                                                    =    " << endl;
	cout << "              |&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&|    " << endl;
	cout << endl;
}

// Fim da implementação dos módulos globais