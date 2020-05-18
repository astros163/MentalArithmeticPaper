#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QDataStream>
#include <functional>

//  算式生成函数，生成算式字符串
typedef std::function<QString()> TQuestionGenerator;

//---------------------------------------------------------
//! @brief  算式类型类
//! 采用类型字符串-生成函数关联的方式
//! 采用单例模式，只维持一个实例
//---------------------------------------------------------
class CQuestionTypes
{
private:
    //! @brief  构造函数，构造时添加各种算式类型
    CQuestionTypes();
    ~CQuestionTypes(){}
    CQuestionTypes(const CQuestionTypes&) = delete;
    CQuestionTypes& operator= (const CQuestionTypes &) = delete;
public:
    //! @brief  单例模式的获取实例的函数
    //! @return CQuestionTypes  单一的实例
    //! 采用函数内静态变量的方法，在C++11环境下保证线程安全，并自动析构
    static CQuestionTypes& instance()
    {
        static CQuestionTypes obj;
        return obj;
    }
private:
    //  用QMap进行关联
    QMap<QString, TQuestionGenerator> _GenMap;
public:
    //! @brief  返回所有类型字符串，以供选择
    //! @return QList<QString>  所有类型字符串，可直接设置到QComboBox中
    QList<QString> getTypes() const;
    //! @brief  产生指定类型指定数量的算式
    //! @param  type    const QString&  [in]指定的算式类型
    //! @param  count   size_t          [in]指定的算式数量
    //! @return QList<QString>          产生的算式
    QList<QString> generate(const QString& type, 
                            size_t count) const;
};

//---------------------------------------------------------
//! @brief  算式集合类
//! 一个算式集合是多个某一种算式的聚集
//---------------------------------------------------------
class CQuestionSet
{
public:
    CQuestionSet();
    CQuestionSet(const CQuestionSet& another);
    CQuestionSet(const QString& type, size_t count);
public:
    QString Type;
    size_t  Count;
public:
    //  流读写
    void Load(QDataStream& stream);
    void Save(QDataStream& stream) const;
};
//  将算式集合声明为自定义元类型，以便保存到QVariant中
Q_DECLARE_METATYPE(CQuestionSet);

//---------------------------------------------------------
//! @brief  算式区域类
//! 算式区域是多个算式集合的聚集，且带有分栏数量、是否打乱算式等参数
//! 算式区域内的算式集合的数量可为0，表示将参与平分未指定的算式数量
//---------------------------------------------------------
class CQuestionSection
{
public:
    CQuestionSection();
    CQuestionSection(const CQuestionSection& another);
public:
    QList<CQuestionSet> _Sets;
    bool                _Disorder;
    QList<QString>      _LastQuestions;
private:
    size_t              _Columns;
    size_t              _Count;
public:
    inline  size_t  getColumns()    const { return _Columns; }
    inline  size_t  getCount()      const { return _Count; }
    //! @brief  按照区域的参数产生算式
    //! @return QList<QString>          产生的算式
    QList<QString>  generate()      const;
    //! @brief  取得区域内指定了数量的算式集合的算式数量之和
    //! @return size_t  和
    size_t  getFixedCount()        const;
    //! @brief  取得区域内参与平分未指定算式数量的算式集合数
    //! @return size_t  算式数量未0的算式集合数
    size_t  getZeroSetCount()  const;
    //! @brief  设置区域的分栏数
    //! @param  cols    size_t          [in]分栏数量
    //! @return void
    //! 分栏数量最小为1，如果指定为0，则实际分栏数为1
    void    setColumns(size_t cols);
    //! @brief  设置区域的算式总数
    //! @param  count   size_t          [in]算式总数
    //! @return void
    //! 算式总数不应小于区域内各算式集合的算式数量之和
    //! *** 如果设定的算式总数小于区域内各算式集合的算式数量之和，
    //!     则实际设置为0，生成时按照区域内各算式集合的实际算式数量执行；
    //! *** 如果设定的算式总数大于区域内各算式集合的算式数量之和，
    //!     且区域内各算式集合的算式数量均不为0，
    //!     则实际设置为0，生成时按照区域内各算式集合的实际算式数量执行；
    //! *** 如果设定的算式总数大于区域内各算式集合的算式数量之和，
    //!     且区域内有算式数量为0的算式集合，
    //!     则设置为指定总数，生成时多余数量由算式数量为0的算式集合平分。
    void    setCount(size_t count);
public:
    //  流读写
    void Load(QDataStream& stream);
    void Save(QDataStream& stream) const;
};
//  将算式区域声明为自定义元类型，以便保存到QVariant中
Q_DECLARE_METATYPE(CQuestionSection);

//---------------------------------------------------------
//! @brief  算式试卷类
//! 算式试卷是多个算式区域的集合
//---------------------------------------------------------
class CQuestionPaper
{
public:
    CQuestionPaper();
    CQuestionPaper(const CQuestionPaper& another);
public:
    QList<CQuestionSection> _Sections;
public:
    //  流读写
    void Load(QDataStream& stream);
    void Save(QDataStream& stream) const;
};

#endif // !QUESTIONS_H
