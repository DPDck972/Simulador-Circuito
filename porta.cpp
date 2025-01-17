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
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = ~(*in_port.begin());
        this->setOutput(saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

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
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = *in_port.begin();
        for (auto it = in_port.begin(); it != in_port.end(); ++it) // Operação com .begin() não provoca erro na porta AND
        {
            saida = saida & (*it);
        }
        this->setOutput(saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

/// Porta NAND

ptr_Porta PortaNAND::clone() const
{
    return new PortaNAND(*this);
}

std::string PortaNAND::getName() const
{
    return "NA";
}

bool PortaNAND::simular(const std::vector<bool3S> &in_port)
{
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = *in_port.begin();
        for (auto it = in_port.begin(); it != in_port.end(); ++it) // Operação com .begin() não provoca erro na porta NAND
        {
            saida = saida & (*it);
        }
        this->setOutput(~saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

/// Porta OR

ptr_Porta PortaOR::clone() const
{
    return new PortaOR(*this);
}

std::string PortaOR::getName() const
{
    return "OR";
}

bool PortaOR::simular(const std::vector<bool3S> &in_port)
{
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = *in_port.begin();
        for (auto it = in_port.begin(); it != in_port.end(); ++it) // Operação com .begin() não provoca erro na porta OR
        {
            saida = saida | (*it);
        }
        this->setOutput(saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

/// Porta NOR

ptr_Porta PortaNOR::clone() const
{
    return new PortaNOR(*this);
}

std::string PortaNOR::getName() const
{
    return "NO";
}

bool PortaNOR::simular(const std::vector<bool3S> &in_port)
{
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = *in_port.begin();
        for (auto it = in_port.begin(); it != in_port.end(); ++it) // Operação com .begin() não provoca erro na porta NOR
        {
            saida = saida | (*it);
        }
        this->setOutput(~saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

/// Porta XOR

ptr_Porta PortaXOR::clone() const
{
    return new PortaXOR(*this);
}

std::string PortaXOR::getName() const
{
    return "XO";
}

bool PortaXOR::simular(const std::vector<bool3S> &in_port)
{
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = *in_port.begin();
        for (auto it = in_port.begin() + 1; it != in_port.end(); ++it) // Usando .begin() + 1 para evitar repetição e erro na simulação da porta XOR
        {
            saida = saida ^ (*it);
        }
        this->setOutput(saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}

/// Porta NXOR

ptr_Porta PortaNXOR::clone() const
{
    return new PortaNXOR(*this);
}

std::string PortaNXOR::getName() const
{
    return "NX";
}

bool PortaNXOR::simular(const std::vector<bool3S> &in_port)
{
    if (this->getNumInputs() == int(in_port.size()) && int(in_port.size()) > 0)
    {
        bool3S saida = *in_port.begin();
        for (auto it = in_port.begin() + 1; it != in_port.end(); ++it) // Usando .begin() + 1 para evitar repetição e erro na simulação da porta NXOR
        {
            saida = saida ^ (*it);
        }
        this->setOutput(~saida);
        return true;
    }
    else
    {
        this->setOutput(bool3S::UNDEF);
        return false;
    }
}
