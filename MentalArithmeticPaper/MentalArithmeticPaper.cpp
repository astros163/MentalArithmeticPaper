#include "MentalArithmeticPaper.h"
#include <QLabel>
#include <algorithm>
#include <QTextTable>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog> 
#include <QtPrintSupport/QPageSetupDialog> 
#include <QPainter>
#include <QFileDialog>
#include <QDebug>

MentalArithmeticPaper::MentalArithmeticPaper(QWidget *parent)
    : QMainWindow(parent),
    _pSetupWindow(new CSetupPaperWindow)
{
    ui.setupUi(this);
    //  载入上次的试卷设置
    QFile datafile("./paper.dat");
    datafile.open(QIODevice::ReadOnly);
    if(datafile.isOpen())
    {
        QDataStream stream(&datafile);
        _Paper.Load(stream);
    }
    //  工具条事件
    connect(ui.actionSetup,     //  设置
            &QAction::triggered,
            [=]()
            {
                if(_pSetupWindow != nullptr)
                {
                    //  将现有试卷设置复制到试卷设置窗口中
                    _pSetupWindow->setPaper(_Paper);
                    //  试卷设置窗口为了使用QToolBar，不能使用QDialog，只能自己实现模态
                    _pSetupWindow->setWindowModality(Qt::WindowModal);
                    //  显示设置窗口
                    //  注意：show函数会立即返回，因此要通过事件获得对话框操作结果
                    _pSetupWindow->show();
                }
            });
    connect(ui.actionGenerate,  //  生成
            &QAction::triggered,
            [=]()
            {
                //  对每个试题部分，创建试题的字符串列表，并保存在_LastQuestions中
                for(CQuestionSection& s : _Paper._Sections)
                {
                    s._LastQuestions = s.generate();
                }
                //  刷新显示
                refresh();
            });
    connect(ui.actionRefresh,   //  刷新
            &QAction::triggered,
            [=]()
            {
                refresh();
            });
    connect(ui.actionPageSetup, //  页面设置
            &QAction::triggered,
            [=]()
            {
                doPageSetup();
            });
    connect(ui.actionPrintPreview,  //  打印预览
            &QAction::triggered,
            [=]()
            {
                doPrintPreview();
            });
    connect(ui.actionPrint,     //  打印
            &QAction::triggered,
            [=]()
            {
                doPrint();
            });
    connect(ui.actionPDF,       //  打印到PDF
            &QAction::triggered,
            [=]()
            {
                printToPDF();
            });
    connect(ui.actionExit,      //  退出
            &QAction::triggered,
            [=]()
            {
                //
                close();
            });
    //  设置窗口确定产生的自定义事件
    connect(_pSetupWindow,
            &CSetupPaperWindow::signalSetupFinished,
            [=](const CQuestionPaper& paper)
            {
                //  保存设置结果
                _Paper = paper;
            });
}

MentalArithmeticPaper::~MentalArithmeticPaper()
{
    delete _pSetupWindow;
}

//  页面设置
void MentalArithmeticPaper::doPageSetup()
{
    QPageSetupDialog pageSetupDlg(&_Printer, this);
    pageSetupDlg.exec();
}

//  打印预览
void MentalArithmeticPaper::doPrintPreview()
{
    // 创建打印预览对话框
    QPrintPreviewDialog preview(&_Printer, this);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
            this, SLOT(printPreview(QPrinter*)));
    preview.exec();
}

//  打印
void MentalArithmeticPaper::doPrint()
{
    // 创建打印对话框
    QString printerName = _Printer.printerName();
    if(printerName.size() == 0)
        return;
    QPrintDialog dlg(&_Printer, this);
    // 如果在对话框中按下了打印按钮，则执行打印操作
    if(dlg.exec() == QDialog::Accepted)
    {
        ui.edtPaper->print(&_Printer);
    }
}

//  打印预览回调函数
void MentalArithmeticPaper::printPreview(QPrinter *printer)
{
#if 1
    //  正常的打印预览
    //  ***** 经常出现窗口尺寸发生变化后，打印结果也发生偏移，在打印预览中能够看到
    ui.edtPaper->print(printer);
#else
    //
    //以下方法在分页打印图形时可用
    //打印文字时可能将同一行文字分别打印在两页上
    //
    //实测当QTextEdit尺寸改变后，其document()的pageSize也会随之改变
    QTextDocument* doc = ui.edtPaper->document();
    QSizeF szd = doc->size();
    QSizeF szp = printer->pageRect().size();
    qreal scale = szp.width() / szd.width();
    QPainter painter(printer);
    painter.scale(scale, scale);
    //
    qreal print_height = szd.height() * scale;
    qreal print_top = 0;
    while(print_top < print_height)
    {
        painter.save();
        //
        painter.translate(0, -print_top / scale);
        doc->drawContents(&painter,
                          QRectF(0,
                                 print_top / scale,
                                 szp.width() / scale,
                                 szp.height() / scale));
        print_top += szp.height();
        //
        if(print_top < print_height)
            printer->newPage();
        //
        painter.restore();
}
#endif
}

//  打印到PDF文件
void MentalArithmeticPaper::printToPDF()
{
    QString filepath = QFileDialog::getSaveFileName(this,
                                                    QString::fromLocal8Bit("保存为..."),
                                                    QString::fromLocal8Bit("./口算试卷.pdf"),
                                                    QString::fromLocal8Bit("PDF格式(*.pdf)"));
    if(!filepath.isEmpty())
    {
        _Printer.setOutputFileName(filepath);
        ui.edtPaper->print(&_Printer);
    }
}

//  更新文档
void MentalArithmeticPaper::refresh()
{
    //  清空文档内容
    ui.edtPaper->clear();
    //  若试卷中不包含试题，直接退出
    if(_Paper._Sections.size() == 0) return;
    //  取得打印机的页面尺寸
    QSizeF pageSize = _Printer.pageRect().size();
    qDebug() << "LAYOUT: " << _Printer.pageLayout();
    //  设置为文档的纸张尺寸
    //  ***** 实测当窗口发生变化时，文档尺寸也发生变化
    ui.edtPaper->document()->setPageSize(pageSize);
    //  设置文档字体
    QFont font("Times New Roman");
    font.setPointSizeF(15);
    ui.edtPaper->document()->setDefaultFont(font);
    //  设置文档边界
    //  ***** 实测设置为0对文档偏移现象没有影响
    ui.edtPaper->document()->setDocumentMargin(0);
    //  循环每一个试题部分
    for(const CQuestionSection& s : _Paper._Sections)
    {
        //  若该部分没有试题，继续下一部分
        if(s._LastQuestions.size() == 0)
            continue;
        //  取得文档光标
        //  调用QTextEdit的textCursor，能获得最后一次编辑以后的文档位置
        QTextCursor cursor = ui.edtPaper->textCursor();
        //  按照该试题部分的分栏设置表格列数
        //  同时按照列数和试题总数设置表格行数
        int cols = s.getColumns();
        int rows = s._LastQuestions.size() / cols;
        if((s._LastQuestions.size() % cols) != 0)
            ++rows;
        QTextTable* table = cursor.insertTable(rows, cols);
        //  设置表格格式：居中、边界为0、宽度为页面宽度、无边框
        QTextTableFormat fmt = table->format();
        fmt.setAlignment(Qt::AlignCenter);
        fmt.setMargin(0.0);
        fmt.setWidth(pageSize.width());
        //fmt.setCellSpacing(1.0);
        fmt.setBorderStyle(QTextFrameFormat::BorderStyle_None);
        table->setFormat(fmt);
        //  设置表格内容
        for(int i = 0; i < s._LastQuestions.size(); ++i)
        {
            int row = i / cols;
            int col = i % cols;
            table->cellAt(row, col).firstCursorPosition().insertText(s._LastQuestions[i]);
        }
    }
}

void MentalArithmeticPaper::resizeEvent(QResizeEvent * event)
{
    refresh();
}

void MentalArithmeticPaper::closeEvent(QCloseEvent * event)
{
    //  保存试卷设置
    QFile datafile("./paper.dat");
    if(!datafile.exists())
        datafile.open(QIODevice::NewOnly);
    else
        datafile.open(QIODevice::WriteOnly);
    QDataStream stream(&datafile);
    _Paper.Save(stream);
}
