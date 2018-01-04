#include "MaClasse.h"
#include <QtGlobal>
#include <QDateTime>
#include <QDebug>

static const char *strRef = "Ceci est une chaine pour remplir une variable membre du type const char*, ce qui nous permettra de vérifier la performance lors de la copie des données de cette chaine\0";

MaClasse::MaClasse()
{
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    i_ = qrand();
    str_ = new char[strlen(strRef)+1];
    memcpy(str_, strRef, strlen(strRef));
    str_[strlen(strRef)] = '\0';
    f_ = i_/1.2348766f;
    d_ = f_*0.883773;
}

MaClasse::~MaClasse()
{
    if(str_) {
        delete[] str_;
    }
}

MaClasse::MaClasse(const MaClasse &orig)
    : i_(orig.i_), f_(orig.f_), d_(orig.d_)
{
    str_ = new char[strlen(orig.str_)+1];
    memcpy(str_, strRef, strlen(orig.str_));
    str_[strlen(orig.str_)] = '\0';
}

MaClasse& MaClasse::operator =(const MaClasse &orig)
{
    i_ = orig.i_;
    str_ = new char[strlen(orig.str_)+1];
    memcpy(str_, strRef, strlen(orig.str_));
    str_[strlen(orig.str_)] = '\0';
    f_ = orig.f_;
    d_ = orig.d_;

    return *this;
}
