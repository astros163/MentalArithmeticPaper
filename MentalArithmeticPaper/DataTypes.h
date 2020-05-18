#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtCore/QString>
#include <QtCore/QList>

enum TArithmeticType
{
    atAdd = 0,
    atSub,
    atMul,
    atDiv,
    atRem
};

enum TValueScope
{
    vs0_9,
    vs10_99,
    vs100_999,
    vs1000_9999,
};

struct TQuestionItemType
{
    TArithmeticType     Ari;
    TValueScope         Scope;
};

struct TQuestionItemData
{
    TQuestionItemType   Type;
    float               Param1;
    float               Param2;
    float               Param3;
};

struct TQuestionSection
{
    QString                     Title;
    QList<TQuestionItemData>    Questions;
};


#endif // !DATATYPES_H
