#ifndef LAB6_9_TV_H
#define LAB6_9_TV_H

#include "UserInput.h"
#include "UserOutput.h"

// Superclass (B)
class TV
{
protected:
    double m_diagonal;
    // параметр, вычисляемый по формуле
    float m_costCoefficient{};
public:
    // конструктор инициализации
    explicit TV(const double& diagonal)
    : m_diagonal(diagonal)
    {
        std::cout << "TV constructor\n";
    }

    TV() : m_diagonal(0)
    {}

    virtual ~TV()
    {
        std::cout << "Destructor of TV's member\n";
    }

    // чисто виртуальная функция
    virtual void Info() = 0;

    virtual void calculateCostCoefficient()
    {
        int k = (int )m_diagonal / 10;
        if (k > 1 && k < 10) m_costCoefficient += 1;
        if (k > 10) m_costCoefficient += 2;
    }

    double getDiagonal() const
    { return m_diagonal; }

    float getCostCoefficient() const
    { return m_costCoefficient; }

    virtual void fill()
    {
        std::cout << "Input diagonal!" << std::endl;
        Input::getValue(m_diagonal);
    }
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

    LCD() : TV(0), m_tech("")
    {}

    ~LCD() override
    {
        std::cout << "Destructor of LCD's member\n";
    }

    void Info() override
    {
        calculateCostCoefficient();
        std::cout << "#LCD\n";
        std::cout << "Diagonal:   " << getDiagonal() << std::endl;
        std::cout << "Tech:       " << getTech() << std::endl;
        std::cout << "Cost Multi: " << getCostCoefficient() << std::endl;
    }

    void calculateCostCoefficient() override
    {
        m_costCoefficient += 1;
        int k = (int )m_diagonal / 10;
        if (k > 1 && k < 10) m_costCoefficient += 1;
        if (k > 10) m_costCoefficient += 2;
        if (m_tech.substr(0, m_tech.size()) == "LCD") m_costCoefficient += 1.3f;
    }

    std::string getTech() const
    { return m_tech; }

    void fill() override
    {
        std::cout << "Input diagonal!" << std::endl;
        Input::getValue(m_diagonal);
        std::cout << "Enter technology! (f.e. LCD)!" << std::endl;
        m_tech = Input::getStringNoDigs();
    }
};

// Subclass B <- P1 <- P11
class LG : public LCD
{
protected:
    std::string m_matrix_type;
public:
    LG(const double& diagonal, const std::string &tech, std::string matrixType = "LG")
    : TV(diagonal), LCD(diagonal, tech), m_matrix_type(std::move(matrixType))
    {
        std::cout << "LG constructor\n";
    }

    LG () : LCD() {}

    ~LG() override
    {
        std::cout << "Destructor of LG's member\n";
    }

    void Info() override
    {
        calculateCostCoefficient();
        std::cout << "#LG\n";
        std::cout << "Diagonal:    " << getDiagonal() << std::endl;
        std::cout << "Matrix type: " << getMatrixType() << std::endl;
        std::cout << "Tech:        " << getTech() << std::endl;
        std::cout << "Cost Multi:  " << getCostCoefficient() << std::endl;
    }

    void calculateCostCoefficient() override
    {
        m_costCoefficient += 1;
        int k = (int )m_diagonal / 10;
        if (k > 1 && k < 10) m_costCoefficient += 1;
        if (k > 10) m_costCoefficient += 2;
        if (m_tech.substr(0, m_tech.size()) == "LCD") m_costCoefficient += 1.3f;
        if (m_matrix_type == "QLED" || m_matrix_type == "IPS" || m_matrix_type == "VA") m_costCoefficient += 1.2f;
    }

    std::string getMatrixType() const
    { return m_matrix_type; }

    void fill() override
    {
        std::cout << "Input diagonal!" << std::endl;
        Input::getValue(m_diagonal);
        std::cout << "Enter technology! (f.e. LCD)!" << std::endl;
        m_tech = Input::getStringNoDigs();
        std::cout << "Enter matrix type! (f.e. QLED, IPS, VA)!" << std::endl;
        m_matrix_type = Input::getStringNoDigs();
    }
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

    Plasma() : TV(0), m_hasVR(false) {}

    ~Plasma() override
    {
        std::cout << "Destructor of Plasma's member\n";
    }

    void Info() override
    {
        calculateCostCoefficient();
        std::cout << "#Plasma\n";
        std::cout << "Diagonal:   " << getDiagonal() << std::endl;
        std::cout << "Has VR:     " << Output::printBool(getVR()) << std::endl;
        std::cout << "Cost Multi: " << getCostCoefficient() << std::endl;
    }

    void calculateCostCoefficient() override
    {
        m_costCoefficient += 1;
        m_hasVR ? m_costCoefficient += 1 : m_costCoefficient += 0;
        int k = (int )m_diagonal / 10;
        if (k > 1 && k < 10) m_costCoefficient += 1;
        if (k > 10) m_costCoefficient += 2;
    }

    bool getVR() const
    { return m_hasVR; }

    void fill() override
    {
        std::cout << "Input diagonal!" << std::endl;
        Input::getValue(m_diagonal);
        std::cout << "Has VR?" << std::endl;
        m_hasVR = Input::getBoolean();
    }
};

#endif //LAB6_9_TV_H







