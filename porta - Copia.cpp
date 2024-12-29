#include "porta.h"

///
/// AS PORTAS
///

/// Porta NOT
ptr_Porta PortaNOT::clone() const
{
    return new PortaNOT(*this);
}

std::string PortaNOT::getName() const
{
    return "NT";
}

bool PortaNOT::simular(const std::vector<bool3S> &in_port)
{
    // Simula uma porta logica.
    // Recebe um vector de bool3S com os valores logicos das entradas da porta
    // com os quais deve ser simulada a funcao logica da porta.
    // Se a dimensao do vetor in_port for adequada (>0 e igual ao numero de entradas
    // da porta), armazena o resultado da simulacao em out_port e retorna true.
    // Se nao for, faz out_port = UNDEF e retorna false.
    if (this->getNumInputs() == in_port.size)
    {
        bool3S saida;
        if (in_port[0] == bool3S::TRUE)
            saida = bool3S::FALSE;
        else if (in_port[0] == bool3S::FALSE)
            saida = bool3S::TRUE;
        this->setOutput(saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

//
// FALTA IMPLEMENTAR
//

/// Porta AND

ptr_Porta PortaAND::clone() const
{
    return new PortaAND(*this);
}

std::string PortaAND::getName() const
{
    return "AN";
}

bool PortaAND::simular(const std::vector<bool3S> &in_port)
{
}

//
// FALTA IMPLEMENTAR
//

/// Porta NAND

//
// FALTA IMPLEMENTAR
//

/// Porta OR

//
// FALTA IMPLEMENTAR
//

/// Porta NOR

//
// FALTA IMPLEMENTAR
//

/// Porta XOR

//
// FALTA IMPLEMENTAR
//

/// Porta NXOR

//
// FALTA IMPLEMENTAR
//
