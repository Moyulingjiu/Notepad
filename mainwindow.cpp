#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QString>
#include <QFont>
#include <QFontDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QIcon>
#include <QToolButton>
#include <QImage>
#include <QPixmap>
#include <QDockWidget>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QPageSetupDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/Book.ico"));
    QFont FontStyle("微软雅黑",12,14,false);
    isSaved = true;
    FileNameS = "";
    LastDir = "C:/";
    ShowRate = 0;

    ui->setupUi(this);

    resize(1200,660);
    setWindowTitle("记事本");


    QMenuBar * bar = menuBar();
    setMenuBar(bar);


    QMenu * filesMenu = bar->addMenu("文件(&F)");
    QMenu * editMenu = bar->addMenu("编辑(&E)");
    QMenu * typeMenu = bar->addMenu("格式(&O)");
    QMenu * viewMenu = bar->addMenu("查看(&V)");
    QMenu * helpMenu = bar->addMenu("帮助(&H)");

    //文件菜单下拉项
    QAction * NewFile = filesMenu->addAction("新建(&N)");
    NewFile->setShortcut(Qt::CTRL + Qt::Key_N);
    QAction * OpenFile = filesMenu->addAction("打开(&O)");
    OpenFile->setShortcut(Qt::CTRL + Qt::Key_O);
    QAction * SaveFile = filesMenu->addAction("保存(&S)");
    SaveFile->setShortcut(Qt::CTRL + Qt::Key_S);
    QAction * AsSaveFile = filesMenu->addAction("另存为(&A)");
    AsSaveFile->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_S);
    filesMenu->addSeparator();
    //QAction * PageSet = filesMenu->addAction("页面设置(&U)");
    QAction * PagePrint = filesMenu->addAction("打印(&P)");
    PagePrint->setShortcut(Qt::CTRL + Qt::Key_P);
    filesMenu->addSeparator();
    QAction * ExitFile = filesMenu->addAction("退出(&X)");

    //编辑菜单下拉项
    QAction * Revoke = editMenu->addAction("撤销(&U)");
    Revoke->setShortcut(Qt::CTRL + Qt::Key_Z);
    editMenu->addSeparator();
    QAction * Cut = editMenu->addAction("剪切(&T)");
    Cut->setShortcut(Qt::CTRL + Qt::Key_X);
    QAction * Copy = editMenu->addAction("复制(&C)");
    Copy->setShortcut(Qt::CTRL + Qt::Key_C);
    QAction * Paste = editMenu->addAction("粘贴(&P)");
    Paste->setShortcut(Qt::CTRL + Qt::Key_V);
    QAction * Delete = editMenu->addAction("删除(&L)");
    Delete->setShortcut(Qt::Key_Delete);
    editMenu->addSeparator();
    QAction * SearchOnLine = editMenu->addAction("使用百度搜索");
    SearchOnLine->setShortcut(Qt::CTRL + Qt::Key_E);
    QAction * FindText = editMenu->addAction("查找(&F)");
    FindText->setShortcut(Qt::CTRL + Qt::Key_F);
    QAction * FindTextNext = editMenu->addAction("查找下一个(&N)");
    FindTextNext->setShortcut(Qt::Key_F3);
    QAction * ReplaceText = editMenu->addAction("替换(&R)");
    ReplaceText->setShortcut(Qt::CTRL + Qt::Key_H);
    //QAction * GoToText = editMenu->addAction("转到(&G)");
    //GoToText->setShortcut(Qt::CTRL + Qt::Key_G);
    editMenu->addSeparator();
    QAction * ChooseAllText = editMenu->addAction("全选(&A)");
    ChooseAllText->setShortcut(Qt::CTRL + Qt::Key_A);
    QAction * InsertData = editMenu->addAction("时间/日期(&D)");
    InsertData->setShortcut(Qt::Key_F5);

    //格式菜单下拉项
    QAction * AutoChangeLine = typeMenu->addAction("自动换行(&W)");
    AutoChangeLine->setIcon(QIcon(":/MenuBarImage/Choose.png"));
    QAction * FontType = typeMenu->addAction("字体(&F)");

    //查看菜单下拉项
    QMenu * Scaling = viewMenu->addMenu("缩放(&Z)");
    QAction * enlarge = Scaling->addAction("放大");
    enlarge->setShortcut(Qt::CTRL + Qt::Key_Equal);
    QAction * narrow = Scaling->addAction("缩小");
    narrow->setShortcut(Qt::CTRL + Qt::Key_Minus);
    QAction * reduction = Scaling->addAction("恢复默认缩放");
    reduction->setShortcut(Qt::CTRL + Qt::Key_0);
    QAction * IsStatusBat = viewMenu->addAction("状态栏(&S)");
    IsStatusBat->setIcon(QIcon(":/MenuBarImage/Choose.png"));

    //帮助菜单下拉项
    QAction * HelpDoc = helpMenu->addAction("查看帮助(&H)");
    QAction * AboutNotepad = helpMenu->addAction("关于记事本(&A)");

    //状态栏
    QStatusBar * StatusBar = statusBar();
    setStatusBar(StatusBar);
    QLabel * TextFormat = new QLabel("CRLF          ",this);
    QLabel * MousePost = new QLabel("第1行,第1列          ",this);
    QLabel * DisplayScale = new QLabel("100%          ",this);
    StatusBar->addPermanentWidget(TextFormat);
    StatusBar->addPermanentWidget(MousePost);
    StatusBar->addPermanentWidget(DisplayScale);

    //查找窗体
    ui->Find->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->label_3->setVisible(false);

    //文本框
    QPlainTextEdit * edit = new QPlainTextEdit(this);
    edit->setFont(FontStyle);
    edit->setStyleSheet("QPlainTextEdit{border-width:0;border-style:outset}"); //取消边框
    setCentralWidget(edit);
    EditCopy = edit;


    //初始化部分
    //---------------------------------------------------
    loadSettings();
    setFileName(FileNameS,*edit);
    Cut->setEnabled(false);
    Copy->setEnabled(false);
    Delete->setEnabled(false);






    //函数链接部分
    //---------------------------------------------------
    connect(NewFile,&QAction::triggered,this,[=](){ //新建文件
        if(isSaved)
            edit->clear();
        else
        {
            if(confirmSave(*edit))
            {
                edit->clear();
            }
        }
    });
    connect(OpenFile,&QAction::triggered,this,[=](){ //打开文件
        if(isSaved)
        {
            QString PathName = QFileDialog::getOpenFileName(this,"打开文件","C:/","(*.txt)");
            loadFromFile(PathName,*edit);
        }
        else
        {
            if(confirmSave(*edit))
            {
                QString PathName = QFileDialog::getOpenFileName(this,"打开文件","C:/","(*.txt)");
                loadFromFile(PathName,*edit);
            }
        }
    });
    connect(SaveFile,&QAction::triggered,this,[=](){ //保存文件
        if(!isSaved)
        {
            if(FileNameS != "")
            {
                saveFile(FileNameS,*edit);
            }
            else
            {
                saveFileAs(*edit);
            }
        }
    });
    connect(AsSaveFile,&QAction::triggered,this,[=](){ //另存为文件
        saveFileAs(*edit);
    });

    /*
    connect(PageSet,&QAction::triggered,this,[=](){
        QPageSetupDialog page(this);
        page.exec();
        qDebug() << page.printer();
    });
    */
    connect(PagePrint,&QAction::triggered,this,[=](){
        QPrintDialog dlg(this);
        if(dlg.exec() == QPrintDialog::Accepted)
        {
            QPrinter* p = dlg.printer();//获取QPrinter数据对象
            QTextDocument td;

            td.setPlainText(edit->toPlainText());//设置打印内容
            QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"),
                LastDir + tr("./未命名.pdf"));
            if (fileName.isEmpty())
                return;
            p->setOutputFileName(fileName);//设置打印到文件名
            td.print(p);//传入参数开始打印
        }
    });

    connect(ExitFile,&QAction::triggered,this,&MainWindow::slotExit);

    //---------------------------------------------------
    connect(Revoke,&QAction::triggered,this,[=](){
        edit->undo();
    });
    connect(Cut,&QAction::triggered,this,[=](){
        edit->cut();
    });
    connect(Copy,&QAction::triggered,this,[=](){
        edit->copy();
    });
    connect(Paste,&QAction::triggered,this,[=](){
        edit->paste();
    });
    connect(Delete,&QAction::triggered,this,[=](){
        edit->textCursor().removeSelectedText();
    });

    connect(SearchOnLine,&QAction::triggered,this,[=](){ //使用百度搜索
        QDesktopServices::openUrl(QUrl(QString("https://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&tn=monline_3_dg&wd=" + edit->textCursor().selectedText() + "&oq=Qt%25E8%258F%259C%25E5%258D%2595%25E6%25A0%258F%25E5%258F%2598%25E7%2581%25B0&rsv_pq=db16e4f80007027c&rsv_t=5d9esAXtgUE7WN%2F8FJfnBLj50hPHypdCBokPO48tWWyAbGmI47ZSspKf4cKlyMnfBuB%2F&rqlang=cn&rsv_enter=1&rsv_dl=tb&rsv_sug3=11&")));
    });
    connect(FindText,&QAction::triggered,this,[=](){ //查找
        ui->Find->setWindowTitle("查找");
        ui->widget_5->setVisible(false);
        ui->widget_4->setVisible(false);
        ui->Find->setVisible(true);
    });
    connect(FindTextNext,&QAction::triggered,this,[=](){ //查找下一个
        if(!ui->Find->isVisible())
        {
            ui->Find->setWindowTitle("查找");
            ui->widget_5->setVisible(false);
            ui->widget_4->setVisible(false);
            ui->Find->setVisible(true);
        }
        emit ui->pushButton->clicked();
    });
    connect(ReplaceText,&QAction::triggered,this,[=](){ //替换
        ui->Find->setWindowTitle("替换");
        ui->widget_5->setVisible(true);
        ui->widget_4->setVisible(true);
        ui->Find->setVisible(true);
    });

    connect(ChooseAllText,&QAction::triggered,this,[=](){
        edit->selectAll();
    });
    connect(InsertData,&QAction::triggered,this,[=](){
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
        edit->insertPlainText(current_date);
    });

    //---------------------------------------------------
    connect(AutoChangeLine,&QAction::triggered,this,[=](){ //设置是否自动换行
        if(edit->wordWrapMode() == 4)
        {
            edit->setWordWrapMode(QTextOption::ManualWrap);
            AutoChangeLine->setIcon(QIcon(""));
        }
        else
        {
            edit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
            AutoChangeLine->setIcon(QIcon(":/MenuBarImage/Choose.png"));
        }
    });
    connect(FontType,&QAction::triggered,this,[=](){ //设置字体
        bool flag;
        QFont FontStyle3(edit->font().family(),edit->font().pointSize() - ShowRate,edit->font().italic());
        QFont FontStyle2 = QFontDialog::getFont(&flag,FontStyle3);
        if (flag)
        {
            QFont FontStyle3(FontStyle2.family(),FontStyle2.pointSize() + ShowRate,FontStyle2.weight(),FontStyle2.italic());
            edit->setFont(FontStyle3);
        }
    });

    //---------------------------------------------------
    connect(enlarge,&QAction::triggered,this,[=](){ //放大
        if(edit->font().pointSize() < 100)
        {
            ShowRate += 1;
            QFont FontStyle2(edit->font().family(),edit->font().pointSize() + 1,edit->font().weight(),edit->font().italic());
            edit->setFont(FontStyle2);
            DisplayScale->setText(tr("%1%          ").arg(edit->font().pointSize() * 100 / (edit->font().pointSize() - ShowRate)));
        }
    });
    connect(narrow,&QAction::triggered,this,[=](){ //缩小
        if(edit->font().pointSize() > 5)
        {
            ShowRate -= 1;
            QFont FontStyle2(edit->font().family(),edit->font().pointSize() - 1,edit->font().weight(),edit->font().italic());
            edit->setFont(FontStyle2);
            DisplayScale->setText(tr("%1%          ").arg(edit->font().pointSize() * 100 / (edit->font().pointSize() - ShowRate)));
        }
    });
    connect(reduction,&QAction::triggered,this,[=](){ //恢复默认
        QFont FontStyle2(edit->font().family(),edit->font().pointSize() - ShowRate,edit->font().weight(),edit->font().italic());
        edit->setFont(FontStyle2);
        ShowRate = 0;
        DisplayScale->setText(tr("%1%          ").arg(edit->font().pointSize() * 100 / (edit->font().pointSize() - ShowRate)));
    });
    connect(IsStatusBat,&QAction::triggered,this,[=](){ //设置状态栏是否可见
        if(StatusBar->isVisible())
        {
            StatusBar->setVisible(false);
            IsStatusBat->setIcon(QIcon(""));
        }
        else
        {
            StatusBar->setVisible(true);
            IsStatusBat->setIcon(QIcon(":/MenuBarImage/Choose.png"));
        }
    });

    //---------------------------------------------------
    connect(HelpDoc,&QAction::triggered,this,[=](){ //查看帮助
        QDesktopServices::openUrl(QUrl(QString("https://cn.bing.com/search?q=%E8%8E%B7%E5%8F%96%E6%9C%89%E5%85%B3+windows+10+%E4%B8%AD%E7%9A%84%E8%AE%B0%E4%BA%8B%E6%9C%AC%E7%9A%84%E5%B8%AE%E5%8A%A9&filters=guid:%224466414-zh-hans-dia%22%20lang:%22zh-hans%22&form=T00032&ocid=HelpPane-BingIA")));
    });
    connect(AboutNotepad,&QAction::triggered,this,[=](){ //查看记事本信息
        QDialog About(this);
        About.resize(200,200);
        About.setMinimumSize(200,200);
        About.setMaximumSize(200,200);
        Qt::WindowFlags flags = Qt::Dialog;
        flags |= Qt::WindowCloseButtonHint;
        About.setWindowFlags(flags);
        About.setWindowTitle("记事本");

        QLabel *LogoIcon=new QLabel(&About);
        //QToolButton * LogoIcon = new QToolButton(&About);
        LogoIcon->resize(64,64);
        LogoIcon->setPixmap(QPixmap(":/Book.ico"));
        LogoIcon->setScaledContents(true);
        LogoIcon->move(68,18);
        //LogoIcon->setDisabled(true);

        QLabel * version = new QLabel(&About);
        version->setText("版本号：v0.0.2");
        version->move(50,100);
        QLabel * author = new QLabel(&About);
        author->setText("制作者：张浩山");
        author->move(50,120);

        QPushButton * AboutSure = new QPushButton(&About);
        AboutSure->setText("确认");
        AboutSure->move(60,160);

        connect(AboutSure,&QPushButton::clicked,&About,&QWidget::close);

        //About.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        About.exec();
    });

    //---------------------------------------------------
    connect(edit,&QPlainTextEdit::selectionChanged,this,[=](){
        QTextCursor tc = edit->textCursor();
        QString info = tr("第%1行，第%2列          ").arg(tc.blockNumber() + 1).arg(tc.columnNumber());
        MousePost->setText(info);

        if(edit->textCursor().selectedText() == "")
        {
            Cut->setEnabled(false);
            Copy->setEnabled(false);
            Delete->setEnabled(false);
        }
        else
        {
            Cut->setEnabled(true);
            Copy->setEnabled(true);
            Delete->setEnabled(true);
        }
    });
    connect(edit,&QPlainTextEdit::textChanged,this,[=](){
        QTextCursor tc = edit->textCursor();
        QString info = tr("第%1行，第%2列          ").arg(tc.blockNumber() + 1).arg(tc.columnNumber());
        MousePost->setText(info);
        isSaved = false;
        //edit->setFont(FontStyle);
    });


    connect(ui->pushButton,&QPushButton::clicked,[=](){ //查找下一个
        ui->label_3->setVisible(false);
        findTxt(*edit);
        edit->setFocus();
    });
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){ //取消
        ui->label_3->setVisible(false);
        ui->Find->setVisible(false);
    });
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){ //替换
        ui->label_3->setVisible(false);
        bool res = findTxt(*edit);
        if(res)
            edit->insertPlainText(ui->lineEdit_2->text());
        edit->setFocus();
    });
    connect(ui->pushButton_4,&QPushButton::clicked,[=](){ //替换全部
        ui->label_3->setVisible(false);
        bool res = findTxt(*edit);
        while(res)
        {
            edit->insertPlainText(ui->lineEdit_2->text());
            res = findTxt(*edit);
        }
        edit->setFocus();
    });
    connect(ui->lineEdit,&QLineEdit::textChanged,[=](){
        if(ui->lineEdit->text() == "")
        {
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
        }
        else
        {
            ui->pushButton->setEnabled(true);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_4->setEnabled(true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotExit()
{
    qDebug("clicked");
    this->close();
}
void MainWindow::loadSettings()
{
    //大小&位置
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Notepad", QSettings::NativeFormat);
    int x = settings.value("iWindowPosX", 0).toInt();
    int y = settings.value("iWindowPosY", 0).toInt();
    int w = settings.value("iWindowPosDX", 800).toInt();
    int h = settings.value("iWindowPosDY", 600).toInt();
    if(x<10) x = 100;
    if(y<10) y = 100;
    this->setGeometry(x, y, w, h);
}
void MainWindow::saveSettings()
{
    //大小&位置
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Notepad", QSettings::NativeFormat);
    settings.setValue("iWindowPosX", this->x());
    settings.setValue("iWindowPosY", this->y());
    settings.setValue("iWindowPosDX", this->width());
    settings.setValue("iWindowPosDY", this->height());
}


void MainWindow::setFileName(QString fileName,QPlainTextEdit &edit)
{
    this->FileNameS = fileName;
    edit.document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = tr("未命名");
    else
        shownName = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1[*] - %2").arg(shownName, tr("记事本")));
    setWindowModified(false);
    LastDir = QFileInfo(fileName).absoluteDir().absolutePath();
}
void MainWindow::loadFromFile(QString fileName,QPlainTextEdit &edit)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.isFile())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("提示"), tr("不能打开此文件！"), tr("确定"));
        return;
    }

    setFileName(fileName,edit);
    QTextStream in(&file);
    edit.setPlainText(in.readAll());
}

bool MainWindow::saveFile(QString fileName,QPlainTextEdit &edit)
{
    if (!edit.document()->isModified())
        return false;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this, tr("提示"), tr("不能写入文件！"), tr("确定"));
        return false;
    }

    QTextStream out(&file);
    out << edit.toPlainText();
    setFileName(fileName,edit);
    FileNameS = fileName;
    return true;
}
bool MainWindow::saveFileAs(QPlainTextEdit &edit)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"),
        LastDir + tr("./未命名.txt"), tr("文本文档(*.txt);;所有文件(*.*)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName,edit);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (confirmSave(*EditCopy))
    {
        saveSettings();
        event->accept();
    }
    else
        event->ignore();
}
bool MainWindow::confirmSave(QPlainTextEdit &edit)
{
    if (!edit.document()->isModified())
        return true;
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, tr("记事本"), tr("是否将更改保存到 %1？").arg(this->windowTitle().replace(tr(" - 记事本"), "")), QMessageBox::Save|QMessageBox::No|QMessageBox::Cancel);

    msgBox->button(QMessageBox::Save)->setText("保存");
    msgBox->button(QMessageBox::No)->setText("否");
    msgBox->button(QMessageBox::Cancel)->setText("取消");

    int choose =  msgBox->exec();
    delete msgBox;
    if(choose == QMessageBox::Save)
    {
        if(FileNameS == "")
        {
            return saveFileAs(edit);
        }
        else
        {
            return saveFile(FileNameS,edit);
        }

    }
    else if(choose == QMessageBox::No)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MainWindow::findTxt(QPlainTextEdit &edit){
    bool res = false;
    if(ui->lineEdit->text() != "")
    {
        if(ui->checkBox->checkState() == 2)
        {
            if(ui->radioButton_2->isChecked())
            {
                res = edit.find(ui->lineEdit->text(),QTextDocument::FindCaseSensitively);
            }
            else
            {
                res = edit.find(ui->lineEdit->text(),QTextDocument::FindCaseSensitively|QTextDocument::FindBackward);
            }
        }
        else
        {
            if(ui->radioButton_2->isChecked())
            {
                res = edit.find(ui->lineEdit->text());
            }
            else
            {
                res = edit.find(ui->lineEdit->text(),QTextDocument::FindBackward);
            }
        }
    }
    if(!res)
    {
        ui->label_3->setVisible(true);
        ui->label_3->setText("找不到\"" + ui->lineEdit->text() + "\" 或已经查找到文末");
    }
    return res;
}
