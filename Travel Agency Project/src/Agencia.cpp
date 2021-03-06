/*
 * Agencia.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: Diogo
 */

#include "Agencia.h"

time_t Agencia::tempo_local = time(0);
tm* Agencia::tempo_info = localtime(&tempo_local);

/* Class: Agencia
 *
 *
 *
 */

Agencia::Agencia(string nome) :
		nome(nome), destinos(Destino()) {

}

Agencia::~Agencia() {

}

string Agencia::getNome() const {
	return nome;
}

bool Agencia::addCliente(Cliente* c) {
	vector<Cliente*>::const_iterator it;
	it = find(clientes.begin(), clientes.end(), c);
	if (it != clientes.end())
		return false; //TODO change to a throw eventually
	else {
		if (c->getStatus() == "Frequente") {
			addClienteFrequente(c);
		} else {
			addClienteAntigo(c);
		}
		clientes.push_back(c);
	}
	return true;
}

vector<Cliente*> Agencia::getClientes() const {
	return clientes;
}

vector<Viagem> Agencia::getViagens() const {
	return viagens;
}

bool Agencia::addViagem(Viagem v) {
	vector<Viagem>::const_iterator it;
	it = find(viagens.begin(), viagens.end(), v);
	if (it != viagens.end())
		return false; //TODO change to a throw eventually
	viagens.push_back(v);
	return true;
}

bool Agencia::addPais(Pais p) {
	vector<Pais>::const_iterator it;
	it = find(paises.begin(), paises.end(), p);
	if (it != paises.end())
		return false; //TODO change to a throw eventually
	paises.push_back(p);
	return true;
}

vector<Pais> Agencia::getPaises() const {
	return paises;
}

Agencia::ClienteInexistente::ClienteInexistente(string nome) :
		nome(nome) {

}

Cliente* Agencia::getCliente(string nome) const {
	int it = -1;
	for (unsigned int i = 0; i < clientes.size(); i++) {
		if (clientes[i]->getNome() == nome)
			it = i;
	}
	if (it == -1)
		throw ClienteInexistente(nome);
	return clientes[it];
}

Pais& Agencia::getPais(string nome) {
	int it = -1;
	for (unsigned int i = 0; i < paises.size(); i++) {
		if (nome == paises[i].getNome())
			it = i;
	}
	if (it == -1)
		throw PaisInexistente(nome);
	return paises[it];
}

Agencia::PaisInexistente::PaisInexistente(string nome) :
		nome(nome) {

}

string Agencia::PaisInexistente::getNome() const {
	return this->nome;
}

Viagem* Agencia::getViagem(int id) {
	int it = -1;
	for (unsigned int i = 0; i < viagens.size(); i++) {
		if (viagens[i].getId() == id)
			it = i;
	}
	if (it == -1)
		throw ViagemInexistente(id);
	return &viagens[it];
}

Agencia::ViagemInexistente::ViagemInexistente(int id) :
		id(id) {

}

int Agencia::ViagemInexistente::getId() const {
	return id;
}

void Agencia::loadCidades() {
	string filename = "../cidades" + nome + ".txt";
	ifstream file(filename.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string pais = "";
			getline(file, pais, '-');
			string cidade = "";
			getline(file, cidade);
			addPais(Pais(pais));
			getPais(pais).addCidade(Cidade(cidade, pais));
		}
		file.close();
	} else {

	}
}

void Agencia::loadAlojamentos() {
	string filename = "../alojamentos" + nome + ".txt";
	ifstream file(filename.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string nome = "";
			getline(file, nome, '-');
			string tipo = "";
			getline(file, tipo, '-');
			string preco = "";
			getline(file, preco, '-');
			string pais = "";
			getline(file, pais, ',');
			string cidade = "";
			getline(file, cidade);
			getPais(pais).getCidade(cidade)->addAlojamento(
					Alojamento(tipo, nome, atof(preco.c_str())));
		}
		file.close();
	} else {

	}
}

void Agencia::loadViagens() {
	string filename = "../viagens" + nome + ".txt";
	ifstream file(filename.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			//Ler id
			string id;
			getline(file, id, '-');
			//Ler preco
			string preco;
			getline(file, preco, '-');
			//Ler itinerario/trocos
			string itinerario;
			getline(file, itinerario, '-');
			istringstream istream(itinerario);
			vector<Cidade*> vc;
			while (!istream.eof()) {
				string temp = "";
				getline(istream, temp, ';');
				istringstream tempstream(temp);
				string pais = "", cidade = "";
				getline(tempstream, pais, ',');
				getline(tempstream, cidade);
				try {
					vc.push_back(getPais(pais).getCidade(cidade));
				} catch (Pais::CidadeInexistente &ci) {
					cout << "Cidade " << ci.getNome() << " nao existe!" << endl;
					getch();
					//should never happen
				} catch (PaisInexistente &pi) {
					cout << "Pais " << pi.getNome() << " nao existe!" << endl;
					getch();
					//should never happen
				}
			}
			Itinerario iti(vc[0], vc[vc.size() - 1]);
			//Ler transportes
			string transportes;
			getline(file, transportes, '-');
			istringstream tstream(transportes);
			vector<string> vt;
			while (!tstream.eof()) {
				string temp = "";
				getline(tstream, temp, ';');
				vt.push_back(temp);
			}
			//Ler datas
			string datas;
			getline(file, datas);
			istringstream dstream(datas);
			vector<tm> vd;
			while (!dstream.eof()) {
				tm nd;
				time_t now;
				time(&now);
				nd = *localtime(&now);
				string dia = "";
				getline(dstream, dia, '/');
				nd.tm_mday = atoi(dia.c_str());
				string mes = "";
				getline(dstream, mes, '/');
				nd.tm_mon = atoi(mes.c_str()) - 1;
				string ano = "";
				getline(dstream, ano, ' ');
				nd.tm_year = atoi(ano.c_str()) + 100;
				string hora = "";
				getline(dstream, hora, ':');
				nd.tm_hour = atoi(hora.c_str());
				string minuto = "";
				getline(dstream, minuto, ';');
				nd.tm_min = atoi(minuto.c_str());
				vd.push_back(nd);
			}
			//Adicionar os trocos
			for (unsigned int i = 0; i < vc.size() - 1; i++) {
				iti.addTroco(Troco(vc[i], vc[i + 1], vt[i], vd[i]));
			}
			//Adicionar viagem
			addViagem(Viagem(iti, atoi(preco.c_str()), atoi(id.c_str())));
		}
		file.close();
	} else {

	}
}

void Agencia::loadClientes() {
	string filename = "../clientes" + nome + ".txt";
	ifstream file(filename.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string nome = "";
			getline(file, nome, '-');
			string email = "";
			getline(file, email, '-');
			string morada = "";
			getline(file, morada, '-');
			string tipo = "";
			getline(file, tipo, '-');
			Cliente* c;
			if (tipo == "P") {
				c = new Particular(nome, email, morada);
			} else if (tipo == "C") {
				string nop = "";
				getline(file, nop, '-');
				c = new Comercial(nome, email, morada, atoi(nop.c_str()));
			}
			string vids = "";
			getline(file, vids);
			istringstream svids(vids);
			while (!svids.eof()) {
				string id = "";
				getline(svids, id, ' ');
				try {
					Destino* d = getDestino(atoi(id.c_str()));
					c->addDestino(d);
				} catch (Agencia::ViagemInexistente &vi) {
					//should work still
				} catch (Agencia::DestinoInexistente &vi) {
					//should work still
				}
			}
			addCliente(c);
		}
		file.close();
	} else {

	}
}

void Agencia::loadDestinos() {
	string filename = "../destinos" + nome + ".txt";
	ifstream file(filename.c_str());
	if (file.is_open()) {
		while (!file.eof()) {
			string id = "";
			getline(file, id, '-');
			string vid = "";
			getline(file, vid, '-');
			string alojamento = "";
			getline(file, alojamento, '-');
			string desconto = "";
			getline(file, desconto);
			Viagem *v;
			try {
				v = getViagem(atoi(vid.c_str()));
			} catch (ViagemInexistente &vi) {
				cout << "Viagem " << vi.getId() << " nao existe!" << endl;
				getch();
				//should never happen
			}
			Alojamento* a;
			try {
				a = v->getItinerario().getDestino()->getAlojamento(alojamento);
			} catch (Cidade::AlojamentoInexistente &ai) {
				a = new Alojamento();
			}
			addDestino(
					Destino(atoi(desconto.c_str()),
							v->getItinerario().getDestino(), v, a,
							atoi(id.c_str())));
			/*cout << "destino: "<< v->getItinerario().getDestino()->getNome() << endl;
			 cout << "alojamento: " << alojamento << endl;
			 cout << "desconto: " << desconto << endl;
			 getch();*/
		}
		file.close();
	} else {

	}
}

void Agencia::saveCidades() {
	string filename = "../cidades" + nome + ".txt";
	if (ifstream(filename.c_str())) {
		remove(filename.c_str());
	}
	ofstream file(filename.c_str());
	for (unsigned int i = 0; i < paises.size(); i++) {
		for (unsigned int j = 0; j < paises[i].getCidades().size(); j++) {
			if (!(i == 0 && j == 0)) {
				file << endl;
			}
			file << paises[i].getNome() << "-"
					<< paises[i].getCidades()[j].getNome();
		}
	}
	file.close();
}

void Agencia::saveAlojamentos() {
	string filename = "../alojamentos" + nome + ".txt";
	if (ifstream(filename.c_str())) {
		remove(filename.c_str());
	}
	ofstream file(filename.c_str());
	for (unsigned int i = 0; i < paises.size(); i++) {
		for (unsigned int j = 0; j < paises[i].getCidades().size(); j++) {
			for (unsigned int k = 0;
					k < paises[i].getCidades()[j].getAlojamentos().size();
					k++) {
				if (!(i == 0 && j == 0 && k == 0)) {
					file << endl;
				}
				file << paises[i].getCidades()[j].getAlojamentos()[k].getNome()
						<< "-"
						<< paises[i].getCidades()[j].getAlojamentos()[k].getTipo()
						<< "-"
						<< paises[i].getCidades()[j].getAlojamentos()[k].getPreco()
						<< "-" << paises[i].getNome() << ","
						<< paises[i].getCidades()[j].getNome();
			}
		}
	}
	file.close();
}

void Agencia::saveViagens() {
	string filename = "../viagens" + nome + ".txt";
	if (ifstream(filename.c_str())) {
		remove(filename.c_str());
	}
	ofstream file(filename.c_str());
	for (unsigned int i = 0; i < viagens.size(); i++) {
		if (i != 0) {
			file << endl;
		}
		file << viagens[i].getId() << "-" << viagens[i].getPreco() << "-";
		vector<Troco> trocos = viagens[i].getItinerario().getTrocos();
		file << trocos[0].getCidadeOrigem()->getPais() << ","
				<< trocos[0].getCidadeOrigem()->getNome() << ";";
		for (unsigned int j = 0; j < trocos.size(); j++) {
			if (j != 0) {
				file << ";";
			}
			file << trocos[j].getCidadeDestino()->getPais() << ","
					<< trocos[j].getCidadeDestino()->getNome();
		}
		file << "-";
		for (unsigned int j = 0; j < trocos.size(); j++) {
			if (j != 0) {
				file << ";";
			}
			file << trocos[j].getTransporte().getTipo();
		}
		file << "-";
		for (unsigned int j = 0; j < trocos.size(); j++) {
			if (j != 0) {
				file << ";";
			}
			file << trocos[j].getData().tm_mday << "/"
					<< trocos[j].getData().tm_mon + 1 << "/"
					<< trocos[j].getData().tm_year - 100 << " "
					<< trocos[j].getData().tm_hour << ":"
					<< trocos[j].getData().tm_min;
		}
	}
}

void Agencia::saveClientes() {
	string filename = "../clientes" + nome + ".txt";
	if (ifstream(filename.c_str())) {
		remove(filename.c_str());
	}
	ofstream file(filename.c_str());
	for (unsigned int i = 0; i < clientes.size(); i++) {
		if (i != 0) {
			file << endl;
		}
		file << clientes[i]->getNome() << "-";
		file << clientes[i]->getEmail() << "-";
		file << clientes[i]->getMorada() << "-";
		if (clientes[i]->getTipo() == "Comercial") {
			file << "C" << "-";
			file << clientes[i]->getNoParticipantes() << "-";
		} else if (clientes[i]->getTipo() == "Particular") {
			file << "P" << "-";
		}
		for (unsigned int j = 0; j < clientes[i]->getDestinos().size(); j++) {
			if (j != 0) {
				file << " ";
			}
			file << clientes[i]->getDestinos()[j]->getId();
		}
	}
}

void Agencia::saveDestinos() {
	string filename = "../destinos" + nome + ".txt";
	if (ifstream(filename.c_str())) {
		remove(filename.c_str());
	}
	ofstream file(filename.c_str());
	BSTItrIn<Destino> it(destinos);
	while (!it.isAtEnd()) {
		Destino d = it.retrieve();
		file << d.getId() << "-" << d.getViagem()->getId() << "-"
				<< d.getAlojamento()->getNome() << "-" << d.getDesconto();
		it.advance();
		if (!it.isAtEnd()) {
			file << endl;
		}
	}
	file.close();
}

tm* Agencia::getTempo_Info() {
	return tempo_info;
}

Agencia::DestinoInexistente::DestinoInexistente(string nome) :
		nome(nome) {

}

string Agencia::DestinoInexistente::getNome() const {
	return nome;
}

Destino* Agencia::addDestino(Destino d1){
	destinos.insert(d1);
	return &d1;
}

Destino* Agencia::getDestino(string nome) const {
	BSTItrIn<Destino> it(destinos);
	while (!it.isAtEnd()) {
		if (it.retrieve().getCidade()->getNome() == nome)
			return &it.retrieve();
		it.advance();
	}
	throw DestinoInexistente(nome);
}

Destino* Agencia::getDestino(int id) const {
	BSTItrIn<Destino> it(destinos);
	while (!it.isAtEnd()) {
		if (it.retrieve().getId() == id)
			return &it.retrieve();
		it.advance();
	}
	throw DestinoInexistente(nome);
}

BST<Destino> Agencia::getDestinos() const {
	return destinos;
}

void Agencia::printClientesAntigos() const {
	tr1::unordered_set<Cliente*, hstr, hstr>::const_iterator it;
	for (it = clientes_antigos.begin(); it != clientes_antigos.end(); it++) {
		cout << "Nome: " << (*it)->getNome() << endl;
		cout << "Email: " << (*it)->getEmail() << endl;
		cout << "Morada: " << (*it)->getMorada() << endl;
	}
}

void Agencia::printClientesFrequentes() const {

	priority_queue<Cliente*> copia = clientes_frequentes;
	while (copia.size() > 0) {

		cout << "Nome: " << copia.top()->getNome() << endl;
		cout << "Email: " << copia.top()->getEmail() << endl;
		cout << "Morada: " << copia.top()->getMorada() << endl;

		copia.pop();
	}
}

void Agencia::procurarClienteAntigoEmail(string e) {
	tr1::unordered_set<Cliente*, hstr, hstr>::iterator it;

	for (it = clientes_antigos.begin(); it != clientes_antigos.end(); it++) {
		if ((*it)->getEmail() == e) {
			cout << "Nome:" << (*it)->getNome() << " Morada:"
					<< (*it)->getMorada() << endl;
		}
	}

}

void Agencia::procurarClienteAntigoMorada(string m) {
	tr1::unordered_set<Cliente*, hstr, hstr>::iterator it;

	for (it = clientes_antigos.begin(); it != clientes_antigos.end(); it++) {
		if ((*it)->getMorada() == m) {
			cout << "Nome:" << (*it)->getNome() << " Email:"
					<< (*it)->getEmail() << endl;
		}
	}

}

void Agencia::procurarClienteAntigoNome(string n) {
	tr1::unordered_set<Cliente*, hstr, hstr>::iterator it;
	for (it = clientes_antigos.begin(); it != clientes_antigos.end(); it++) {
		if ((*it)->getNome() == n) {
			cout << "Morada:" << (*it)->getMorada() << " Email:"
					<< (*it)->getEmail() << endl;
		}
	}
}

void Agencia::addClienteAntigo(Cliente* c) {
	clientes_antigos.insert(c);
}

void Agencia::addClienteFrequente(Cliente* c) {
	clientes_frequentes.push(c);
}

void Agencia::procurarClienteFrequentePontos(Cliente* c) {

	priority_queue<Cliente*> temp = clientes_frequentes;
	while (!temp.empty()) {
		if (temp.top() == c) {
			if (c->getPontos() > 100)
				cout << "Cliente de Ouro/n" << "Pontos: " << c->getPontos();
			else if (c->getPontos() > 50)
				cout << "Cliente de Prata/n" << "Pontos: " << c->getPontos();
			else if (c->getPontos() > 0)
				cout << "Cliente de Bronze/n" << "Pontos: " << c->getPontos();
			temp.pop();
		} else
			temp.pop();
	}

}

void Agencia::printDestinos() const {
	BSTItrLevel<Destino> it(destinos);
	while (!it.isAtEnd()) {
		Destino d = it.retrieve();
		cout << "Destino: " << d.getCidade()->getNome() << endl;
		cout << "Preco: " << d.getPreco() << endl;
		cout << "Id Viagem: " << d.getViagem()->getId() << endl << endl;
		it.advance();
	}
}

void Agencia::printCidades() const {
	vector<Pais> vp = paises;
	for (unsigned int i = 0; i < vp.size(); i++) {
		vector<Cidade> vc = vp[i].getCidades();
		for (unsigned int j = 0; j < vc.size(); j++) {
			cout << vp[i].getNome() << "," << vc[j].getNome() << endl;
		}
	}
}

void Agencia::printViagens() const {
	vector<Viagem> vv = viagens;
	for (unsigned int i = 0; i < vv.size(); i++) {
		Cidade* origem = vv[i].getItinerario().getOrigem();
		Cidade* destino = vv[i].getItinerario().getDestino();
		cout << "ID: " << vv[i].getId() << endl;
		cout << "Origem: " << origem->getPais() << "," << origem->getNome()
				<< endl;
		cout << "Destino: " << destino->getPais() << "," << destino->getNome()
				<< endl;
		cout << "Preco: " << vv[i].getPreco() << endl;
		cout << endl;
	}
}

void Agencia::printClientes() const {
	for (unsigned int i = 0; i < clientes.size(); i++)
		cout << (*clientes[i]) << endl;
}

void Agencia::removeCliente(Cliente *c) {
	vector<Cliente*>::iterator it;
	for (it = clientes.begin(); it != clientes.end(); it++) {
		if ((*it)->getNome() == c->getNome()) {
			clientes.erase(it);
		}
	}
	if (c->getStatus() == "Frequente") {
		priority_queue<Cliente*> temp = clientes_frequentes;
		while (temp.size() > 0) {
			if (temp.top()->getNome() == c->getNome()) {
				temp.pop();
			}
		}
		clientes_frequentes = temp;
	}
	if (c->getStatus() == "Antigo") {
		tr1::unordered_set<Cliente*, hstr, hstr>::iterator it2;
		for (it2 = clientes_antigos.begin(); it2 != clientes_antigos.end();
				it2++) {
			if ((*it2)->getNome() == c->getNome()) {
				it2 = clientes_antigos.erase(it2);
			}
		}
	}
	c->~Cliente();
}

void Agencia::updateCliente(Cliente *c) {
	c->updateStatus();
	removeCliente(c);
	addCliente(c);
}

void Agencia::removeDestino(Destino* d) {
	destinos.remove((*d));
}

void Agencia::printAlojamentos() const {
	for (unsigned int i = 0; i < paises.size(); i++) {
		for (unsigned int j = 0; j < paises[i].getCidades().size(); j++) {
			for (unsigned int k = 0;
					k < paises[i].getCidades()[j].getAlojamentos().size();
					k++) {
				cout << paises[i].getNome() << ","
						<< paises[i].getCidades()[j].getNome() << " - "
						<< paises[i].getCidades()[j].getAlojamentos()[k].getNome()
						<< endl;
			}
		}
	}
}

void Agencia::removeViagem(Viagem* v)
{
	vector<Viagem>::iterator it;
	it = find(viagens.begin(),viagens.end(),v);
	if (it!=viagens.end()){
		throw ViagemInexistente(v->getId());
	}
	viagens.erase(it);
}
