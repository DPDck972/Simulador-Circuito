#include <fstream>
#include "circuito.h"

///
/// CLASSE CIRCUITO
///

/// ***********************
/// Inicializacao e finalizacao
/// ***********************

// Construtor por copia
Circuito::Circuito(const Circuito& C):
    Circuito()
{
    ///Confirmar e modificar para vetor se copiar com vetor(vetor)
    this->Nin_circ = C.Nin_circ;
    ///Colocar .capacity do vetor para tamanho certo ?
    //Laço de copia do conteiner de portas
    for(auto p : C.ports) this->ports.push_back(p->clone());

    for(auto p : C.id_in) this->id_in.push_back(p);

    //Laço que copia saidas
    for(int i = 0; i < C.getNumOutputs(); i++){
        out_circ.push_back(C.out_circ[i]);
        id_out.push_back(C.id_out[i]);
    }
}

// Construtor por movimento
Circuito::Circuito(Circuito&& C) noexcept:  Circuito(){
    std::swap(Nin_circ, C.Nin_circ);
    std::swap(ports,C.ports);
    std::swap(out_circ, C.out_circ);
    std::swap(id_in, C.id_in);
    std::swap(id_out, C.id_out);
}

// Limpa todo o conteudo do circuito.
void Circuito::clear() noexcept
{
    Nin_circ = 0;
    for(auto p : this->ports) delete p;
    ports.clear();
    out_circ.clear();
    for(auto p : this->id_in) p.clear();
    id_in.clear();
    id_out.clear();
}

// Operador de atribuicao por copia
Circuito& Circuito::operator=(const Circuito& C)
{
    ///Colocar .capacity do vetor para tamanho certo ?
    if(*this != C){
        this->clear();
        this->Nin_circ = C.Nin_circ;

        //Laço de copia do conteiner de portas
        for(auto p : C.ports) this->ports.push_back(p->clone());

        for(auto p : C.id_in) this->id_in.push_back(p);

        //Laço que copia saidas
        for(int i = 0; i < C.getNumOutputs(); i++){
            out_circ.push_back(C.out_circ[i]);
            id_out.push_back(C.id_out[i]);
        }
    }
    return *this;
}

// Operador de atribuicao por movimento
Circuito& Circuito::operator=(Circuito&& C) noexcept
{
    this->clear(); //O clear já retorna o circuito ao padrão do construtor default

    std::swap(Nin_circ, C.Nin_circ);
    std::swap(ports,C.ports);
    std::swap(out_circ, C.out_circ);
    std::swap(id_in, C.id_in);
    std::swap(id_out, C.id_out);

    return *this;
}

// Redimensiona o circuito
void Circuito::resize(int NI, int NO, int NP)
{
    ///Função resize deixa vetores com valores nulos (zeros)
    ///implementar apenas alocando uma area de memoria com capacity e deixando os vetores sem nada ?
    if (NI<=0 || NO<=0 || NP<=0) return;

    //Limpa todo o circuito antes de mudar dimensões dos vetores
    this->clear();

    //Redimensiona o tamanho dos vetores (que estão em 0) para tamanho desejado
    //preenche os vetores com 0
    this->Nin_circ = NI;
    this->ports.resize(NP);

    //Vetores internos de id_in já estão zerados.
    this->id_in.resize(NP);

    this->out_circ.resize(NO);
    this->id_out.resize(NO);
}

/// ***********************
/// Funcoes de testagem
/// ***********************

// Testa igualdade entre circuitos
bool Circuito::operator==(const Circuito& C) const
{
  // Testa a igualdade do numero de entradas, saidas e portas
  if (getNumInputs() != C.getNumInputs() ||
      getNumOutputs() != C.getNumOutputs() ||
      getNumPorts() != C.getNumPorts()) return false;

  int id;
  // Testa a igualdade das portas
  for (id=1; id<=getNumPorts(); ++id)
  {
    if (definedPort(id))
    {
      if (!C.definedPort(id)) return false;
      if (getNamePort(id) != C.getNamePort(id)) return false;
      if (getNumInputsPort(id) != C.getNumInputsPort(id)) return false;
      for (int j=0; j<getNumInputsPort(id); ++j)
      {
        if (getIdInPort(id,j) != C.getIdInPort(id,j)) return false;
      }
    }
    else if (C.definedPort(id)) return false;
  }
  // Testa a igualdade das saidas
  for (id=1; id<=getNumOutputs(); ++id)
  {
    if (getIdOutputCirc(id) != C.getIdOutputCirc(id)) return false;
  }
  // Tudo igual!
  return true;
}

// Testa circuito valido
bool Circuito::valid() const
{
  int id;
  // Testa o numero de entradas, saidas e portas
  if (getNumInputs()<=0 || getNumOutputs()<=0 || getNumPorts()<=0) return false;
  // Testa cada porta
  for (id=1; id<=getNumPorts(); ++id)
  {
    if (!definedPort(id)) return false;
    for (int j=0; j<getNumInputsPort(id); ++j)
    {
      if (!validIdOrig(getIdInPort(id,j))) return false;
    }
  }
  // Testa cada saida
  for (id=1; id<=getNumOutputs(); ++id)
  {
    if (!validIdOrig(getIdOutputCirc(id))) return false;
  }
  // Tudo valido!
  return true;
}

/// ***********************
/// Funcoes de modificacao
/// ***********************

// A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com Nin entradas.
// Tambem altera o numero de conexoes dessa porta no vetor id_in, para ser igual ao novo
// numero de entradas da porta.
// Caso necessario, converte os caracteres da string Tipo para maiusculas.
// Se der tudo certo, retorna true. Se algum parametro for invalido, retorna false.
bool Circuito::setPort(int IdPort, std::string& Tipo, int Nin)
{
  // Chegagem dos parametros
  if (!validIdPort(IdPort)) return false;
  if (Tipo.size()!=2) return false;
  Tipo.at(0) = toupper(Tipo.at(0));
  Tipo.at(1) = toupper(Tipo.at(1));
  if (Tipo!="NT" &&
      Tipo!="AN" && Tipo!="NA" &&
      Tipo!="OR" && Tipo!="NO" &&
      Tipo!="XO" && Tipo!="NX") return false;
  if (Tipo=="NT" && Nin!=1) return false;
  if (Tipo!="NT" && Nin<2) return false;

  // Altera a porta:
  ptr_Porta prov(nullptr);

  if(Tipo == "NT"){
      prov = new PortaNOT();
  }else if(Tipo == "AN"){
      prov = new PortaAND(Nin);
  }else if(Tipo == "NA"){
      prov = new PortaNAND(Nin);
  }else if(Tipo == "OR"){
      prov = new PortaOR(Nin);
  }else if(Tipo == "NO"){
      prov = new PortaNOR(Nin);
  }else if(Tipo == "XO"){
      prov = new PortaXOR(Nin);
  }else if(Tipo == "NX"){
      prov = new PortaNXOR(Nin);
  }else{
      //Jamais chegaria aqui devido ao teste anterior, mas a IDE estava reclamando....
      return false;
  }
  //Atribui no vetor de porta da posição IdPort a nova porta
  this->ports.at(IdPort-1) = prov;

  // - redimensiona o vetor de conexoes da porta
  this->id_in.at(IdPort-1).resize(Nin);

  return true;
}

// Altera a origem de uma entrada de uma porta
bool Circuito::setIdInPort(int IdPort, int I, int IdOrig)
{
  // Chegagem dos parametros
  if (!definedPort(IdPort)) return false;
  if (!ports.at(IdPort-1)->validIndex(I)) return false;
  if (!validIdOrig(IdOrig)) return false;
  // Fixa a origem da entrada
  id_in.at(IdPort-1).at(I) = IdOrig;
  return true;
}

// Altera a origem de uma saida
bool Circuito::setIdOutputCirc(int IdOut, int IdOrig)
{
  if (!validIdOutputCirc(IdOut) || !validIdOrig(IdOrig)) return false;
  id_out.at(IdOut-1) = IdOrig;
  return true;
}

/// ***********************
/// E/S de dados
/// ***********************

// Entrada dos dados de um circuito via arquivo
bool Circuito::ler(const std::string& arq)
{
  // Novo circuito provisorio a ser lido do arquivo
  Circuito prov;
  // A stream do arquivo a ser lido
  std::ifstream myfile(arq);

  try
  {
    if (!myfile.is_open()) throw 1;

    // Variaveis temporarias para leitura
    std::string pS;
    int NI,NO,NP;
    char c;
    std::string Tipo;
    int Nin_port;
    int id_orig;
    int i,id,I;

    // Lendo as dimensoes do circuito
    myfile >> pS >> NI >> NO >> NP;
    if (!myfile.good() || pS!="CIRCUITO" ||
        NI<=0 || NO<=0 || NP<=0) throw 2;
    // Redimensionando o novo circuito
    prov.resize(NI, NO, NP);

    // Lendo as portas do circuito
    myfile >> pS;
    if (!myfile.good() || pS!="PORTAS") throw 3;

    for (i=0; i<prov.getNumPorts(); ++i)
    {
      // Lendo o tipo e o numero de entradas de uma porta
      myfile >> id >> c >> Tipo >> Nin_port;
      if (!myfile.good() || id != i+1 || c!=')' ||
          !prov.setPort(id,Tipo,Nin_port)) throw 4;
    }


    // Lendo a conectividade das portas
    myfile >> pS;
    if (!myfile.good() || pS!="CONEXOES") throw 5;
    for (i=0; i<prov.getNumPorts(); ++i)
    {
        // Lendo a id da porta
        myfile >> id >> c;

        if (!myfile.good() || id != i+1 || c!=')') throw 6;
        // Lendo as ids das entradas da porta
        for (I=0; I<prov.getNumInputsPort(id); ++I)
        {
            myfile >> id_orig;
            if (!myfile.good() ||
                !prov.setIdInPort(id, I, id_orig)) throw 7;
        }
    }

    // Lendo as saidas do circuito
    myfile >> pS;
    if (!myfile.good() || pS!="SAIDAS") throw 8;
    for (i=0; i<prov.getNumOutputs(); ++i)
    {
      // Lendo a id de uma saida do circuito
      myfile >> id >> c >> id_orig;

      if (!myfile.good() || id != i+1 || c!=')' ||
          !prov.setIdOutputCirc(id, id_orig)) throw 9;

    }
  }
  catch (int erro)
  {
    // Mensagem de erro para debug.
    /*
    std::cerr << "ERRO Circuito::ler - arquivo (" << arq
              << ") invalido - erro tipo " << erro << std::endl;
    */
    return false;
  }
  // Leitura OK
  // Faz o circuito assumir as caracteristicas lidas do arquivo
  *this = std::move(prov);
  return true;
}

// Saida dos dados de um circuito
std::ostream& Circuito::escrever(std::ostream& O) const
{
  // Soh imprime se o circuito for valido
  if (!valid()) return O;

  int id,I;

  O << "CIRCUITO "
    << getNumInputs() << ' '
    << getNumOutputs() << ' '
    << getNumPorts() << std::endl;
  O << "PORTAS\n";
  for (id=1; id<=getNumPorts(); ++id)
  {
    O << id << ") " << getNamePort(id) << ' '
      << getNumInputsPort(id) << std::endl;
  }
  O << "CONEXOES\n";
  for (id=1; id<=getNumPorts(); ++id)
  {
    O << id << ')';
    for (I=0; I<getNumInputsPort(id); ++I)
    {
      O << ' ' << getIdInPort(id,I);
    }
    O << std::endl;
  }
  O << "SAIDAS\n";
  for (id=1; id<=getNumOutputs(); ++id)
  {
    O << id << ") " << getIdOutputCirc(id) << std::endl;
  }
  return O;
}

// Salvar circuito em arquivo
bool Circuito::salvar(const std::string& arq) const
{
  if (!valid()) return false;

  std::ofstream myfile(arq);
  if (!myfile.is_open()) return false;
  escrever(myfile);
  return true;
}

/// ***********************
/// SIMULACAO (funcao principal do circuito)
/// ***********************

// Simula o circuito
bool Circuito::simular(const std::vector<bool3S>& in_circ)
{
  // Soh simula se o cicuito e o parametro forem validos
  if (!valid() || int(in_circ.size())!=getNumInputs()) return false;

  bool tudo_def, alguma_def;
  int id_orig;
  std::vector<bool3S> in_port;


  //Settando todas as saidas para UNDEF
  for(auto p : this->ports) p->setOutput(bool3S::UNDEF);

  do{
      tudo_def = true;
      alguma_def = false;

      //Para todas as "id" das portas:
      for(int id = 1; id <= this->getNumPorts(); id++){
          if(this->getOutputPort(id) == bool3S::UNDEF){
              // Para todas as "j" entradas da porta "id";
              for(int j = 0; j < this->getNumInputsPort(id); j++){
                  // De onde vem a entrada?
                  id_orig = this->getIdInPort(id, j);
                  // Valor bool3S da entrada
                  if(id_orig > 0) in_port.push_back(this->getOutputPort(id_orig));
                  else in_port.push_back(in_circ[(id_orig*-1)-1]);
              }
              // Simula a porta "id" com
              // entradas in_port
              this->ports[id-1]->simular(in_port);
              ///Confirmar utilidade
              in_port.clear();

              // Calcula os critérios
              // de parada do algoritmo
              if(this->getOutputPort(id) == bool3S::UNDEF) tudo_def = false;
              else alguma_def = true;
          }
      }
  }while(!tudo_def && alguma_def);

  // DETERMINAÇÃO DAS SAÍDAS
  for(int id = 1; id <= this->getNumOutputs(); id++){
      // De onde vem a saída?
      id_orig = this->getIdOutputCirc(id);
      // Valor bool3S da saída
      if(id_orig > 0) out_circ[id-1] = this->getOutputPort(id_orig);
      else out_circ[id-1] = in_circ[(id_orig*-1)-1];
  }

  // Tudo OK com a simulacao
  return true;
}
