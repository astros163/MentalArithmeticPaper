#include "Questions.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

//  ȫ�ֱ������ڲ�ʹ�ã��������������
std::default_random_engine eng(time(nullptr));
std::uniform_real_distribution<> dr(0.0, 1.0);

//! @brief  ���������
//! @return double  ���������������Χ[0.0, 1.0)
double Rand()
{
    return dr(eng);
}

//! @brief  ����ָ����Χ�ڵ������
//! @param  low     int     [in]�����������
//! @param  high    int     [in]�����������
//! @return int             ���������������Χ[low, high)
//! ������Ӧ��֤high>low��������ܷ��طǷ����
int RangeRand(int low, int high)
{
    return (int)(Rand() * (high - low) + low);
}

//! @brief  �����޶������Χ�ļӷ���ʽ����
//! @param  result  int&    [in,out]����ǰһ������Ľ����������㱾�μӷ���Ľ��
//! @param  low     int     [in]�¼���������
//! @param  high    int     [in]�¼���������
//! @param  high_r  int     [in]�¼ӷ����������
//! @return QString         �����ļӷ���ʽ���֣���ʽ"+x"
//! ������Ӧ��֤����ʱhigh>result+low����high_r>result+low��������ܷ��ز�ǡ���Ľ��
QString LimitAdd(int& result, int low, int high, int high_r)
{
    int b = result + high < high_r ?
        RangeRand(low, high) :
        RangeRand(low, high_r - result);
    result += b;
    return QString::fromLocal8Bit("��")
        + QString::number(b);
}

//! @brief  �����޶������Χ�ļ�����ʽ����
//! @param  result  int&    [in,out]����ǰһ������Ľ����������㱾�μ�����Ľ��
//! @param  low     int     [in]�¼���������
//! @param  high    int     [in]�¼���������
//! @param  low_r   int     [in]�������������
//! @return QString         �����ļ�����ʽ���֣���ʽ"-x"
//! ������Ӧ��֤����ʱhigh>low+low_r����result>low+low_r��������ܷ��ز�ǡ���Ľ��
QString LimitSub(int& result, int low, int high, int low_r)
{
    int b = result > high ?
        RangeRand(low, high - low_r) :
        RangeRand(low, result - low_r);
    result -= b;
    return QString::fromLocal8Bit("��")
        + QString::number(b);
}

//! @brief  �������޶������Χ�ļӷ���ʽ�ͽ��
//! @param  low     int     [in]�¼���������
//! @param  high    int     [in]�¼���������
//! @return QString         �����ļӷ���ʽ����ʽ"+x"
//! ������Ӧ��֤����ʱhigh>low��������ܷ��ز�ǡ���Ľ��
QString UnlimitAdd(int low, int high)
{
    int b = RangeRand(low, high);
    return QString::fromLocal8Bit("��")
        + QString::number(b);
}

//! @brief  �������޶���Χ�ļ�����ʽ�ͽ��
//! @param  low     int     [in]�¼���������
//! @param  high    int     [in]�¼���������
//! @return QString         �����ļ�����ʽ����ʽ"-x"
//! ������Ӧ��֤����ʱhigh>low��������ܷ��ز�ǡ���Ľ��
QString UnlimitSub(int low, int high)
{
    int b = RangeRand(low, high);
    return QString::fromLocal8Bit("��")
        + QString::number(b);
}

//! @brief  �����������ӷ�����ʽ
//! @param  low1    int     [in]����1������
//! @param  high1   int     [in]����1������
//! @param  low2    int     [in]����2������
//! @param  high2   int     [in]����2������
//! @return QString         �����ļӷ���ʽ����ʽ"a+b="
//! ������Ӧ��֤����ʱhigh1>low1����high2>low2��������ܷ��ز�ǡ���Ľ��
QString Add(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    return QString::number(a)
        + UnlimitAdd(low2, high2)
        + QString::fromLocal8Bit("��");
}

//! @brief  �����������ӷ�����ʽ
//! @param  low1    int     [in]����1������
//! @param  high1   int     [in]����1������
//! @param  low2    int     [in]����2������
//! @return QString         �����ļӷ���ʽ����ʽ"a-b="
//! ������Ӧ��֤����ʱhigh1>low1����low1>=low2��������ܷ��ز�ǡ���Ľ��
QString Sub(int low1, int high1, int low2)
{
    int a = RangeRand(low1, high1);
    return QString::number(a)
        + UnlimitSub(low2, a)
        + QString::fromLocal8Bit("��");
}

//! @brief  ���������ӷ�����ʽ
//! @param  low     int     [in]���ֵ�����
//! @param  high    int     [in]���ֵ�����
//! @param  count   size_t  [in]��������
//! @return QString         ��������ʽ
//! ������Ӧ��֤����ʱhigh>low����count>1��������ܷ��ز�ǡ���Ľ��
//! ***** �����ӷ�����һ�������ɽ���������ض���Χ����ʽ��ʹ��ʱӦ����
QString Add(int low, int high, size_t count)
{
    int a0 = RangeRand(low, high);
    int res = a0;
    QString ques = QString::number(a0);
    for(size_t i = 1; i < count; ++i)
    {
        ques += UnlimitAdd(low, high);
    }
    return ques + QString::fromLocal8Bit("��");
}

//! @brief  ����������������ʽ
//! @param  low0    int     [in]������������
//! @param  high0   int     [in]������������
//! @param  low1    int     [in]����������
//! @param  high1   int     [in]����������
//! @param  count   size_t  [in]����������
//! @return QString         ��������ʽ
//! ������Ӧ��֤����ʱhigh0>low0����high1>low1����count>1��
//! ��high0��high1��count�����ñ�ִ֤��count��֮������Ԥ�Ƶķ�Χ�ڣ�������ܷ��ز�ǡ���Ľ��
//! ***** ������������һ�������ɽ���������ض���Χ����ʽ��ʹ��ʱӦ����
QString Sub(int low0, int high0, int low1, int high1, size_t count)
{
    int a0 = RangeRand(low0, high0);
    int res = a0;
    QString ques = QString::number(a0);
    for(size_t i = 0; i < count; ++i)
    {
        ques += UnlimitSub(low1, high1);
    }
    return ques + QString::fromLocal8Bit("��");
}

//! @brief  ���������Ӽ�������ʽ
//! @param  low     int     [in]���ֵ�����
//! @param  high    int     [in]���ֵ�����
//! @param  low_r   int     [in]�м��������ս��������
//! @param  high_r  int     [in]�м��������ս��������
//! @param  count   size_t  [in]��������
//! @return QString         ��������ʽ
//! ������Ӧ��֤����ʱ��
//!     high > low��
//!     ��high_r > low_r��
//!     ��hgih_r >= high��
//!     ��low_r <= low��
//!     ��count > 1��
//!     ������ܷ��ز�ǡ���Ľ��
//! �����Ӽ�����ʽ��֤��
//!     ��ʽ�е����ֶ���[low,high)��Χ�ڣ�
//!     �����(low_r,high_r)��Χ��
//!     �м�����(low_r,high_r)��Χ�ڣ���һ������[low,high)�ķ�Χ�������������
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
    return ques + QString::fromLocal8Bit("��");
}

//! @brief  �����˷���ʽ
//! @param  low1    int     [in]����1������
//! @param  high1   int     [in]����1������
//! @param  low2    int     [in]����2������
//! @param  high2   int     [in]����2������
//! @return QString         �����ļӷ���ʽ����ʽ"a��b="
//! ������Ӧ��֤����ʱhigh1>low1����high2>low2��������ܷ��ز�ǡ���Ľ��
QString Mul(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    int b = RangeRand(low2, high2);
    return QString::number(a)
        + QString::fromLocal8Bit("��")
        + QString::number(b)
        + QString::fromLocal8Bit("��");
}

//! @brief  ��������������ʽ
//! @param  low1    int     [in]����������
//! @param  high1   int     [in]����������
//! @param  low2    int     [in]�̵�����
//! @param  high2   int     [in]�̵�����
//! @return QString         �����ļӷ���ʽ����ʽ"a��b="
//! ������Ӧ��֤����ʱhigh1>low1����high2>low2����low1>0��������ܷ��ز�ǡ���Ľ��
QString Div(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    int b = RangeRand(low2, high2);
    int c = a * b;
    return QString::number(c)
        + QString::fromLocal8Bit("��")
        + QString::number(a)
        + QString::fromLocal8Bit("��");
}

//! @brief  ����������������ʽ
//! @param  low1    int     [in]����������
//! @param  high1   int     [in]����������
//! @param  low2    int     [in]�̵�����
//! @param  high2   int     [in]�̵�����
//! @return QString         �����ļӷ���ʽ����ʽ"a��b="
//! ������Ӧ��֤����ʱhigh1>low1����high2>low2����low1>1��������ܷ��ز�ǡ���Ľ��
//! ***** ��������һ������0����С�ڳ�������һ��Ҫ����low1>1 *****
QString Div2(int low1, int high1, int low2, int high2)
{
    int a = RangeRand(low1, high1);
    int b = RangeRand(low2, high2);
    int c = RangeRand(1, a);
    int d = a * b + c;
    return QString::number(d)
        + QString::fromLocal8Bit("��")
        + QString::number(a)
        + QString::fromLocal8Bit("��");
}

//---------------------------------------------------------
//! @brief  ��ʽ������
//---------------------------------------------------------
CQuestionTypes::CQuestionTypes()
{
    srand(time(nullptr));
    _GenMap.insert(QString::fromLocal8Bit("��λ���ӷ�"),
                   [=]()
                   {
                       return Add(10, 100, 10, 100);
                   });
    _GenMap.insert(QString::fromLocal8Bit("��λ������"),
                   [=]()
                   {
                       return Sub(10, 100, 10);
                   });
    _GenMap.insert(QString::fromLocal8Bit("��λ���Ӽ���"),
                   [=]()
                   {
                       if(Rand() < 0.5)
                           return Sub(10, 100, 10);
                       else
                           return Add(10, 100, 10, 100);
                   });
    _GenMap.insert(QString::fromLocal8Bit("��λ���ӷ�"),
                   [=]()
                   {
                       return Add(100, 1000, 100, 1000);
                   });
    _GenMap.insert(QString::fromLocal8Bit("��λ������"),
                   [=]()
                   {
                       return Sub(100, 1000, 100);
                   });
    _GenMap.insert(QString::fromLocal8Bit("��λ���Ӽ���"),
                   [=]()
                   {
                       if(Rand() < 0.5)
                           return Sub(100, 1000, 100);
                       else
                           return Add(100, 1000, 100, 1000);
                   });
    _GenMap.insert(QString::fromLocal8Bit("ǧ���ڼӼ���"),
                   [=]()
                   {
                       if(Rand() < 0.5)
                           return Sub(1, 1000, 1);
                       else
                           return Add(1, 1000, 1, 1000);
                   });
    _GenMap.insert(QString::fromLocal8Bit("��������������"),
                   [=]()
                   {
                       return AddSub(10, 100, 0, 200, 3);
                   });
    _GenMap.insert(QString::fromLocal8Bit("���ڳ˷�"),
                   [=]()
                   {
                       return Mul(1, 10, 1, 10);
                   });
    _GenMap.insert(QString::fromLocal8Bit("���ڳ���"),
                   [=]()
                   {
                       return Div(1, 10, 1, 10);
                   });
    _GenMap.insert(QString::fromLocal8Bit("���ڴ���������"),
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
//! @brief  ��ʽ������
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
//! @brief  ��ʽ������
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
    //  ����ƽ�ֵļ�����
    size_t avg_set = getZeroSetCount();
    //  ����ƽ�ֵ���Ŀ��
    size_t avg_total = _Count - getFixedCount();
    //  ƽ����������������
    size_t avg = (size_t)((double)avg_total / (double)avg_set + 0.5);
    //  ���һ������ƽ�ֵļ����ܹ��õ�������
    size_t avg_left = avg_set > 0
        ? avg_total - (avg_set - 1) * avg
        : avg_total;
    //  ѭ��ÿ�����ϣ�������ʽ
    size_t left = avg_total;
    QList<QString> sl;
    for(const CQuestionSet& qi : _Sets)
    {
        if(qi.Count == 0)
        {
            //  ƽ��
            if(left > avg_left)
            {
                //  ��ʣ�೬�����һ�����ϵ�����������ƽ��������Ŀ
                sl.append(CQuestionTypes::instance().generate(qi.Type, avg));
                left -= avg;
            }
            else if(left == avg_left)
            {
                //  ʣ�����һ�����ϵ��������Ǿ������һ��������
                sl.append(CQuestionTypes::instance().generate(qi.Type, avg_left));
                left -= avg_left;
            }
            else
            {
                //  ����������Ǿ��ǳ����߼�������
                throw;
            }
        }
        else
        {
            //  ��������������趨����Ŀ��������
            sl.append(CQuestionTypes::instance().generate(qi.Type, qi.Count));
        }
    }
    //  �����Ҫ����˳��
    if(_Disorder)
    {
        std::random_shuffle(sl.begin(), sl.end(),
                            [=](ptrdiff_t max)
                            {
                                return (ptrdiff_t)(Rand() * max);
                            });
    }
    //  ����
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
    //  �����õ�����С���Ѿ�ȷ���ĸ����⼯���������ĳ�0
    if(count < getFixedCount())
        _Count = 0;
    //  �����õ����������Ѿ�ȷ���ĸ����⼯��������
    //  �����ֲ�����δ�趨���������⼯�ϣ��ĳ�0
    else if(count > getFixedCount()
            && std::count_if(_Sets.begin(),
                             _Sets.end(),
                             [](const CQuestionSet& s)
                             {
                                 return s.Count == 0;
                             }) == 0)
        _Count = 0;
    //  ���������ֱ������
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
//! @brief  ��ʽ�Ծ���
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
