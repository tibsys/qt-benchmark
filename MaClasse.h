#ifndef MACLASSE_H
#define MACLASSE_H


class MaClasse
{
public:
    MaClasse();
    ~MaClasse();
    MaClasse(const MaClasse&);

    MaClasse& operator=(const MaClasse&);

public:
    int i_ = 0;
    char* str_ = nullptr;
    float f_ = 0.0f;
    double d_ = 0.0;
};

#endif // MACLASSE_H
