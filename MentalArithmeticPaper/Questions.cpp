#include "Questions.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

//  全局变量，内部使用，用于随机数生成
std::default_random_engine eng(time(nullptr));
std::uniform_real_distribution<> dr(0.0, 1.0);

//! @brief  产生随机数
//! @return double  产生的随机数，范围[0.0, 1.0)
double Rand()
{
    return dr(eng);
}

//! @brief  产生指定范围内的随机数
//! @param  low     int     [in]随机数的下限
//! @param  high    int     [in]随机数的上限
//! @return int             产生的随机数，范围[low, high)
//! 调用者应保证high>low，否则可能返回非法结果
int RangeRand(int low, int high)
{
    return (int)(Rand() * (high - low) + low);
}

//! @brief  产生限定结果范围的加法算式部分
//! @param  result  int&    [in,out]输入前一个计算的结果，输出计算本次加法后的结果
//! @param  low     int     [in]新加数的下限
//! @param  high    int     [in]新加数的上限
//! @param  high_r  int     [in]新加法结果的上限
//! @return QString         产生的加法算式部分，格式"+x"
//! 调用者应保证输入时high>result+low，且high_r>result+low，否则可能返回不恰当的结果
QString LimitAdd(int& result, int low, int high, int high_r)
{
    int b = result + high < high_r ?
        RangeRand(low, high) :
        RangeRand(low, high_r - result);
    result += b;
    return QString::fromLocal8Bit("＋")
        + QString::number(b);
}

//! @brief  产生限定结果范围的减法算式部分
//! @param  result  int&    [in,out]输入前一个计算的结果，输出计算本次减法后的结果
//! @param  low     int     [in]新减数的下限
//! @param  high    int     [in]新减数的上限
//! @param  low_r   int     [in]减法结果的下限
//! @return QString         产生的减法算式部分，格式"-x"
//! 调用者应保证输入时high>low+low_r，且result>low+low_r，否则可能返回不恰当的结果
QString LimitSub(int& result, int low, int high, int low_r)
{
    int b = result > high ?
        RangeRand(low, high - low_r) :
        RangeRand(low, result - low_r);
    result -= b;
    return QString::fromLocal8Bit("－")
        + QString::number(b);
}

//! @brief  产生不限定结果范围的加法算式和结果
//! @param  low     int     [in]新加数的下限
//! @param  high    int     [in]新加数的上限
//! @return QString         产生的加法算式，格式"+x"
//! 调用者应保证输入时high>low，否则可能返回不恰当的结果
QString UnlimitAdd(int low, int high)
{
    int b = RangeRand(low, high);
    return QString::fromLocal8Bit("＋")
        + QString::number(b);
}

//! @brief  产生不限定范围的减法算式和结果
//! @param  low     int     [in]新减数的下限
//! @param  high    int     [in]新减数的上限
//! @return QString         产生的减法算式，格式"-x"
//! 调用者应保证输入时high>low，否则可能返回不恰当的结果
QString UnlimitSub(int low, int high)
{
    int b = RangeRand(low, high);
    return QString::fromLocal8Bit("－")
        + QString::number(b);
}

//! @brief  产生两个数加法的算式
//! @param  low1    int     [in]加数1的下限
//! @param  high1   int     [in]加数1的上限
//! @param  low2    int     [in]加数2的下限
//! @param  high2   int     [in]加数2的上限
//! @return QString         产生的加法算式，格式"a+b="
//! 调用者应保证输入时high1>low1，且high2>low2，否则可能返回不恰当的结果
QString Add(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    return QString::number(a)
        + UnlimitAdd(low2, high2)
        + QString::fromLocal8Bit("＝");
}

//! @brief  产生两个数加法的算式
//! @param  low1    int     [in]加数1的下限
//! @param  high1   int     [in]加数1的上限
//! @param  low2    int     [in]加数2的下限
//! @return QString         产生的加法算式，格式"a-b="
//! 调用者应保证输入时high1>low1，且low1>=low2，否则可能返回不恰当的结果
QString Sub(int low1, int high1, int low2)
{
    int a = RangeRand(low1, high1);
    return QString::number(a)
        + UnlimitSub(low2, a)
        + QString::fromLocal8Bit("＝");
}

//! @brief  产生连续加法的算式
//! @param  low     int     [in]数字的下限
//! @param  high    int     [in]数字的上限
//! @param  count   size_t  [in]数字数量
//! @return QString         产生的算式
//! 调用者应保证输入时high>low，且count>1，否则可能返回不恰当的结果
//! ***** 连续加法很难一次性生成结果不超过特定范围的算式，使用时应慎重
QString Add(int low, int high, size_t count)
{
    int a0 = RangeRand(low, high);
    int res = a0;
    QString ques = QString::number(a0);
    for(size_t i = 1; i < count; ++i)
    {
        ques += UnlimitAdd(low, high);
    }
    return ques + QString::fromLocal8Bit("＝");
}

//! @brief  产生连续减法的算式
//! @param  low0    int     [in]被减数的下限
//! @param  high0   int     [in]被减数的上限
//! @param  low1    int     [in]减数的下限
//! @param  high1   int     [in]减数的上限
//! @param  count   size_t  [in]减数的数量
//! @return QString         产生的算式
//! 调用者应保证输入时high0>low0，且high1>low1，且count>1，
//! 且high0、high1和count的设置保证执行count次之后结果在预计的范围内，否则可能返回不恰当的结果
//! ***** 连续减法很难一次性生成结果不超过特定范围的算式，使用时应慎重
QString Sub(int low0, int high0, int low1, int high1, size_t count)
{
    int a0 = RangeRand(low0, high0);
    int res = a0;
    QString ques = QString::number(a0);
    for(size_t i = 0; i < count; ++i)
    {
        ques += UnlimitSub(low1, high1);
    }
    return ques + QString::fromLocal8Bit("＝");
}

//! @brief  产生连续加减法的算式
//! @param  low     int     [in]数字的下限
//! @param  high    int     [in]数字的上限
//! @param  low_r   int     [in]中间结果和最终结果的下限
//! @param  high_r  int     [in]中间结果和最终结果的上限
//! @param  count   size_t  [in]数字数量
//! @return QString         产生的算式
//! 调用者应保证输入时：
//!     high > low，
//!     且high_r > low_r，
//!     且hgih_r >= high，
//!     且low_r <= low，
//!     且count > 1，
//!     否则可能返回不恰当的结果
//! 连续加减法算式保证：
//!     算式中的数字都在[low,high)范围内；
//!     结果在(low_r,high_r)范围内
//!     中间结果在(low_r,high_r)范围内，且一旦超过[low,high)的范围，立即向回修正
QString AddSub(int low, int high, int low_r, int high_r, size_t count)
{
    int a0 = RangeRand(low, high);
    int res = a0;
    QString ques = QString::number(a0);
    for(size_t i = 1; i < count; ++i)
    {
        if(res >= high
           || (res > low && Rand() < 0.5))
        {
            ques += LimitSub(res, low, high, low_r);
        }
        else
        {
            ques += LimitAdd(res, low, high, high_r);
        }
    }
    return ques + QString::fromLocal8Bit("＝");
}

//! @brief  产生乘法算式
//! @param  low1    int     [in]乘数1的下限
//! @param  high1   int     [in]乘数1的上限
//! @param  low2    int     [in]乘数2的下限
//! @param  high2   int     [in]乘数2的上限
//! @return QString         产生的加法算式，格式"a×b="
//! 调用者应保证输入时high1>low1，且high2>low2，否则可能返回不恰当的结果
QString Mul(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    int b = RangeRand(low2, high2);
    return QString::number(a)
        + QString::fromLocal8Bit("×")
        + QString::number(b)
        + QString::fromLocal8Bit("＝");
}

//! @brief  产生整除除法算式
//! @param  low1    int     [in]除数的下限
//! @param  high1   int     [in]除数的上限
//! @param  low2    int     [in]商的下限
//! @param  high2   int     [in]商的上限
//! @return QString         产生的加法算式，格式"a÷b="
//! 调用者应保证输入时high1>low1，且high2>low2，且low1>0，否则可能返回不恰当的结果
QString Div(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    int b = RangeRand(low2, high2);
    int c = a * b;
    return QString::number(c)
        + QString::fromLocal8Bit("÷")
        + QString::number(a)
        + QString::fromLocal8Bit("＝");
}

//! @brief  产生带余数除法算式
//! @param  low1    int     [in]除数的下限
//! @param  high1   int     [in]除数的上限
//! @param  low2    int     [in]商的下限
//! @param  high2   int     [in]商的上限
//! @return QString         产生的加法算式，格式"a÷b="
//! 调用者应保证输入时high1>low1，且high2>low2，且low1>1，否则可能返回不恰当的结果
//! ***** 由于余数一定大于0，且小于除数，故一定要设置low1>1 *****
QString Div2(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    int b = RangeRand(low2, high2);
    int c = RangeRand(1, a);
    int d = a * b + c;
    return QString::number(d)
        + QString::fromLocal8Bit("÷")
        + QString::number(a)
        + QString::fromLocal8Bit("＝");
}

//---------------------------------------------------------
//! @brief  算式类型类
//---------------------------------------------------------
CQuestionTypes::CQuestionTypes()
{
    srand(time(nullptr));
    _GenMap.insert(QString::fromLocal8Bit("两位数加法"),
                   [=]()
                   {
                       return Add(10, 100, 10, 100);
                   });
    _GenMap.insert(QString::fromLocal8Bit("两位数减法"),
                   [=]()
                   {
                       return Sub(10, 100, 10);
                   });
    _GenMap.insert(QString::fromLocal8Bit("两位数加减法"),
                   [=]()
                   {
                       if(Rand() < 0.5)
                           return Sub(10, 100, 10);
                       else
                           return Add(10, 100, 10, 100);
                   });
    _GenMap.insert(QString::fromLocal8Bit("三位数加法"),
                   [=]()
                   {
                       return Add(100, 1000, 100, 1000);
                   });
    _GenMap.insert(QString::fromLocal8Bit("三位数减法"),
                   [=]()
                   {
                       return Sub(100, 1000, 100);
                   });
    _GenMap.insert(QString::fromLocal8Bit("三位数加减法"),
                   [=]()
                   {
                       if(Rand() < 0.5)
                           return Sub(100, 1000, 100);
                       else
                           return Add(100, 1000, 100, 1000);
                   });
    _GenMap.insert(QString::fromLocal8Bit("千以内加减法"),
                   [=]()
                   {
                       if(Rand() < 0.5)
                           return Sub(1, 1000, 1);
                       else
                           return Add(1, 1000, 1, 1000);
                   });
    _GenMap.insert(QString::fromLocal8Bit("百以内连加连减"),
                   [=]()
                   {
                       return AddSub(10, 100, 0, 200, 3);
                   });
    _GenMap.insert(QString::fromLocal8Bit("表内乘法"),
                   [=]()
                   {
                       return Mul(1, 10, 1, 10);
                   });
    _GenMap.insert(QString::fromLocal8Bit("表内除法"),
                   [=]()
                   {
                       return Div(1, 10, 1, 10);
                   });
    _GenMap.insert(QString::fromLocal8Bit("表内带余数除法"),
                   [=]()
                   {
                       return Div2(2, 10, 1, 10);
                   });
}

QList<QString> CQuestionTypes::getTypes() const
{
    return _GenMap.keys();
}

QList<QString> CQuestionTypes::generate(const QString & type, size_t count) const
{
    QList<QString> ret;
    auto it = _GenMap.find(type);
    if(it != _GenMap.end())
    {
        for(size_t i = 0; i < count; ++i)
        {
            ret.push_back(it.value()());
        }
    }
    return ret;
}

//---------------------------------------------------------
//! @brief  算式集合类
//---------------------------------------------------------
CQuestionSet::CQuestionSet()
    : CQuestionSet(QString(), 0)
{
}

CQuestionSet::CQuestionSet(const CQuestionSet& another)
    : CQuestionSet(another.Type, another.Count)
{
}

CQuestionSet::CQuestionSet(const QString& type, size_t count)
    : Type(type),
    Count(count)
{
}

void CQuestionSet::Load(QDataStream & stream)
{
    stream >> Type;
    stream >> Count;
}

void CQuestionSet::Save(QDataStream & stream) const
{
    stream << Type;
    stream << Count;
}

QDataStream& operator >> (QDataStream& stream, CQuestionSet& s)
{
    s.Load(stream);
    return stream;
}

QDataStream& operator << (QDataStream& stream, const CQuestionSet& s)
{
    s.Save(stream);
    return stream;
}

//---------------------------------------------------------
//! @brief  算式区域类
//---------------------------------------------------------
CQuestionSection::CQuestionSection()
    : _Sets(),
    _Disorder(false),
    _LastQuestions(),
    _Columns(1),
    _Count(0)
{
}

CQuestionSection::CQuestionSection(const CQuestionSection & another)
    : _Sets(another._Sets),
    _Disorder(another._Disorder),
    _LastQuestions(),
    _Columns(another._Columns),
    _Count(another._Count)
{
}

QList<QString> CQuestionSection::generate() const
{
    //  参与平分的集合数
    size_t avg_set = getZeroSetCount();
    //  用于平分的题目数
    size_t avg_total = _Count - getFixedCount();
    //  平分数量，四舍五入
    size_t avg = (size_t)((double)avg_total / (double)avg_set + 0.5);
    //  最后一个参与平分的集合能够得到的数量
    size_t avg_left = avg_set > 0
        ? avg_total - (avg_set - 1) * avg
        : avg_total;
    //  循环每个集合，生成算式
    size_t left = avg_total;
    QList<QString> sl;
    for(const CQuestionSet& qi : _Sets)
    {
        if(qi.Count == 0)
        {
            //  平分
            if(left > avg_left)
            {
                //  还剩余超过最后一个集合的数量，产生平分数的题目
                sl.append(CQuestionTypes::instance().generate(qi.Type, avg));
                left -= avg;
            }
            else if(left == avg_left)
            {
                //  剩余最后一个集合的数量，那就是最后一个集合了
                sl.append(CQuestionTypes::instance().generate(qi.Type, avg_left));
                left -= avg_left;
            }
            else
            {
                //  其他情况，那就是程序逻辑出错了
                throw;
            }
        }
        else
        {
            //  其他情况，按照设定的题目数量生成
            sl.append(CQuestionTypes::instance().generate(qi.Type, qi.Count));
        }
    }
    //  如果需要打乱顺序
    if(_Disorder)
    {
        std::random_shuffle(sl.begin(), sl.end(),
                            [=](ptrdiff_t max)
                            {
                                return (ptrdiff_t)(Rand() * max);
                            });
    }
    //  返回
    return sl;
}

size_t CQuestionSection::getFixedCount() const
{
    size_t count = 0;
    for(const CQuestionSet& s : _Sets)
    {
        count += s.Count;
    }
    return count;
}

size_t CQuestionSection::getZeroSetCount() const
{
    return std::count_if(_Sets.begin(),
                         _Sets.end(),
                         [](const CQuestionSet& s)
                         {
                             return s.Count == 0;
                         });
}

void CQuestionSection::setColumns(size_t cols)
{
    if(cols == 0)
        cols = 1;
    _Columns = cols;
}

void CQuestionSection::setCount(size_t count)
{
    //  若设置的总数小于已经确定的各试题集合总数，改成0
    if(count < getFixedCount())
        _Count = 0;
    //  若设置的总数大于已经确定的各试题集合总数，
    //  但是又不存在未设定数量的试题集合，改成0
    else if(count > getFixedCount()
            && std::count_if(_Sets.begin(),
                             _Sets.end(),
                             [](const CQuestionSet& s)
                             {
                                 return s.Count == 0;
                             }) == 0)
        _Count = 0;
    //  其它情况，直接设置
    else
        _Count = count;
}

void CQuestionSection::Load(QDataStream & stream)
{
    stream >> _Sets;
    stream >> _Disorder;
    stream >> _Columns;
    stream >> _Count;
    _LastQuestions.clear();
}

void CQuestionSection::Save(QDataStream & stream) const
{
    stream << _Sets;
    stream << _Disorder;
    stream << _Columns;
    stream << _Count;
}

QDataStream& operator >> (QDataStream& stream, CQuestionSection& s)
{
    s.Load(stream);
    return stream;
}

QDataStream& operator << (QDataStream& stream, const CQuestionSection& s)
{
    s.Save(stream);
    return stream;
}

//---------------------------------------------------------
//! @brief  算式试卷类
//---------------------------------------------------------
CQuestionPaper::CQuestionPaper()
    : _Sections()
{
}

CQuestionPaper::CQuestionPaper(const CQuestionPaper & another)
    : _Sections(another._Sections)
{
}

void CQuestionPaper::Load(QDataStream & stream)
{
    stream >> _Sections;
}

void CQuestionPaper::Save(QDataStream & stream) const
{
    stream << _Sections;
}
