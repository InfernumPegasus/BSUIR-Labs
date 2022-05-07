#ifndef LAB5_9_TV_H
#define LAB5_9_TV_H

#include <utility>

#include "D:\Development\C++\Projects\Labs\lib\UserInput.h"
#include "D:\Development\C++\Projects\Labs\lib\UserOutput.h"

// Superclass (B)
class TV
{
protected:
    double m_diagonal;
public:
    // конструктор инициализации
    explicit TV(const double& diagonal = 0)
    : m_diagonal(diagonal)
    {
        std::cout << "TV constructor\n";
    }

    ~TV()
    {
        std::cout << "Destructor of TV's member\n";
    }

    virtual void Info()
    {
        std::cout << "#TV\n";
        std::cout << "Showing TV's member:\n"
                     "Model Diagonal: " << m_diagonal << "\n";
    }

    double getDiagonal() const
    { return m_diagonal; }
};

// Subclass B <- P1
class LCD : public virtual TV
{
protected:
    std::string m_tech;
public:
    // конструктор инициализации
    explicit LCD(const double& diagonal, std::string tech = "LCD")
    : TV(diagonal), m_tech(std::move(tech))
    {
        std::cout << "LCD constructor\n";
    }

    ~LCD()
    {
        std::cout << "Destructor of LCD's member\n";
    }

    void Info() override
    {
        std::cout << "#LCD\n";
        std::cout << "Diagonal: " << getDiagonal() << std::endl;
        std::cout << "Tech:     " << getTech() << std::endl;
    }

    std::string getTech() const
    { return m_tech; }
};

// Subclass B <- P1 <- P11
class LG : public LCD
{
protected:
    std::string m_name;
public:
    LG(const double& diagonal, const std::string &tech, std::string   name = "LG")
    : TV(diagonal), LCD(diagonal, tech), m_name(std::move(name))
    {
        std::cout << "LG constructor\n";
    }

    ~LG()
    {
        std::cout << "Destructor of LG's member\n";
    }

    void Info() override
    {
        std::cout << "#LG\n";
        std::cout << "Diagonal: " << getDiagonal() << std::endl;
        std::cout << "Name:     " << getName() << std::endl;
        std::cout << "Tech:     " << getTech() << std::endl;
    }

    std::string getName() const
    { return m_name; }
};

// Subclass B <- P2
class Plasma : public virtual TV
{
protected:
    bool m_hasVR;
public:
    // конструктор инициализации
    explicit Plasma(const double& diagonal, bool hasVR = false)
    : TV(diagonal), m_hasVR(hasVR)
    {
        std::cout << "Plasma constructor\n";
    }

    ~Plasma()
    {
        std::cout << "Destructor of Plasma's member\n";
    }

    void Info() override
    {
        std::cout << "#Plasma\n";
        std::cout << "Diagonal: " << getDiagonal() << std::endl;
        std::cout << "Has VR:     " << Output::printBool(getVR()) << std::endl;
    }
    bool getVR() const
    { return m_hasVR; }
};

// Subclass B <- P2 <- P22
class Philips : public Plasma
{
protected:
    std::string m_userName;
public:
    // конструктор инициализации
    Philips(const double& diagonal, bool hasVR, std::string userName = "Philips User")
    : TV(diagonal), Plasma(diagonal, hasVR), m_userName(std::move(userName))
    {
        std::cout << "Philips constructor\n";
    }

    ~Philips()
    {
        std::cout << "Destructor of Philips's member\n";
    }

    void Info() override
    {
        std::cout << "#Philips\n";
        std::cout << "Diagonal: " << getDiagonal() << std::endl;
        std::cout << "HasVR:    " << Output::printBool(getVR()) << std::endl;
        std::cout << "UserName: " << m_userName << std::endl;
    }
    std::string getUsername() const
    { return m_userName; }
};

class HomeTV : public LG, public Philips
{
private:
    double m_cost;
public:
    HomeTV(const double& diagonal, const std::string &tech, bool hasVR,
            const std::string& name = "HomeTV-mem", std::string userName = "USER", const double& cost = 90)
    : TV(diagonal), LG(diagonal, tech, name), Philips(diagonal, hasVR, std::move(userName)), m_cost(cost)
    {
    }

    ~HomeTV()
    {
        std::cout << "Destructor of HomeTV's member\n";
    }

    void Info() override
    {
        std::cout << "###HomeTV\n";
        std::cout << "Diagonal: " << getDiagonal() << std::endl;
        std::cout << "Name:     " << getName() << std::endl;
        std::cout << "Tech:     " << getTech() << std::endl;
        std::cout << "Has VR:   " << Output::printBool(getVR()) << std::endl;
        std::cout << "UserName: " << getUsername() << std::endl;
        std::cout << "Cost:     " << getCost() << std::endl;
    }

    double getCost() const
    { return m_cost; }
};

#endif //LAB5_9_TV_H







