#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QDataStream>
#include <functional>

//  ��ʽ���ɺ�����������ʽ�ַ���
typedef std::function<QString()> TQuestionGenerator;

//---------------------------------------------------------
//! @brief  ��ʽ������
//! ���������ַ���-���ɺ��������ķ�ʽ
//! ���õ���ģʽ��ֻά��һ��ʵ��
//---------------------------------------------------------
class CQuestionTypes
{
private:
    //! @brief  ���캯��������ʱ��Ӹ�����ʽ����
    CQuestionTypes();
    ~CQuestionTypes(){}
    CQuestionTypes(const CQuestionTypes&) = delete;
    CQuestionTypes& operator= (const CQuestionTypes &) = delete;
public:
    //! @brief  ����ģʽ�Ļ�ȡʵ���ĺ���
    //! @return CQuestionTypes  ��һ��ʵ��
    //! ���ú����ھ�̬�����ķ�������C++11�����±�֤�̰߳�ȫ�����Զ�����
    static CQuestionTypes& instance()
    {
        static CQuestionTypes obj;
        return obj;
    }
private:
    //  ��QMap���й���
    QMap<QString, TQuestionGenerator> _GenMap;
public:
    //! @brief  �������������ַ������Թ�ѡ��
    //! @return QList<QString>  ���������ַ�������ֱ�����õ�QComboBox��
    QList<QString> getTypes() const;
    //! @brief  ����ָ������ָ����������ʽ
    //! @param  type    const QString&  [in]ָ������ʽ����
    //! @param  count   size_t          [in]ָ������ʽ����
    //! @return QList<QString>          ��������ʽ
    QList<QString> generate(const QString& type, 
                            size_t count) const;
};

//---------------------------------------------------------
//! @brief  ��ʽ������
//! һ����ʽ�����Ƕ��ĳһ����ʽ�ľۼ�
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
    //  ����д
    void Load(QDataStream& stream);
    void Save(QDataStream& stream) const;
};
//  ����ʽ��������Ϊ�Զ���Ԫ���ͣ��Ա㱣�浽QVariant��
Q_DECLARE_METATYPE(CQuestionSet);

//---------------------------------------------------------
//! @brief  ��ʽ������
//! ��ʽ�����Ƕ����ʽ���ϵľۼ����Ҵ��з����������Ƿ������ʽ�Ȳ���
//! ��ʽ�����ڵ���ʽ���ϵ�������Ϊ0����ʾ������ƽ��δָ������ʽ����
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
    //! @brief  ��������Ĳ���������ʽ
    //! @return QList<QString>          ��������ʽ
    QList<QString>  generate()      const;
    //! @brief  ȡ��������ָ������������ʽ���ϵ���ʽ����֮��
    //! @return size_t  ��
    size_t  getFixedCount()        const;
    //! @brief  ȡ�������ڲ���ƽ��δָ����ʽ��������ʽ������
    //! @return size_t  ��ʽ����δ0����ʽ������
    size_t  getZeroSetCount()  const;
    //! @brief  ��������ķ�����
    //! @param  cols    size_t          [in]��������
    //! @return void
    //! ����������СΪ1�����ָ��Ϊ0����ʵ�ʷ�����Ϊ1
    void    setColumns(size_t cols);
    //! @brief  �����������ʽ����
    //! @param  count   size_t          [in]��ʽ����
    //! @return void
    //! ��ʽ������ӦС�������ڸ���ʽ���ϵ���ʽ����֮��
    //! *** ����趨����ʽ����С�������ڸ���ʽ���ϵ���ʽ����֮�ͣ�
    //!     ��ʵ������Ϊ0������ʱ���������ڸ���ʽ���ϵ�ʵ����ʽ����ִ�У�
    //! *** ����趨����ʽ�������������ڸ���ʽ���ϵ���ʽ����֮�ͣ�
    //!     �������ڸ���ʽ���ϵ���ʽ��������Ϊ0��
    //!     ��ʵ������Ϊ0������ʱ���������ڸ���ʽ���ϵ�ʵ����ʽ����ִ�У�
    //! *** ����趨����ʽ�������������ڸ���ʽ���ϵ���ʽ����֮�ͣ�
    //!     ������������ʽ����Ϊ0����ʽ���ϣ�
    //!     ������Ϊָ������������ʱ������������ʽ����Ϊ0����ʽ����ƽ�֡�
    void    setCount(size_t count);
public:
    //  ����д
    void Load(QDataStream& stream);
    void Save(QDataStream& stream) const;
};
//  ����ʽ��������Ϊ�Զ���Ԫ���ͣ��Ա㱣�浽QVariant��
Q_DECLARE_METATYPE(CQuestionSection);

//---------------------------------------------------------
//! @brief  ��ʽ�Ծ���
//! ��ʽ�Ծ��Ƕ����ʽ����ļ���
//---------------------------------------------------------
class CQuestionPaper
{
public:
    CQuestionPaper();
    CQuestionPaper(const CQuestionPaper& another);
public:
    QList<CQuestionSection> _Sections;
public:
    //  ����д
    void Load(QDataStream& stream);
    void Save(QDataStream& stream) const;
};

#endif // !QUESTIONS_H
