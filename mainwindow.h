#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadFromFile(QString fileName){
        loadFromFile(fileName,*EditCopy);
    }

private:
    Ui::MainWindow *ui;
    QString FileNameS;
    QString LastDir;
    QPlainTextEdit * EditCopy;
    bool isSaved;
    int ShowRate;

    void loadSettings();
    void saveSettings();
    void slotExit();
    void setActionState();
    void setFileName(QString fileName,QPlainTextEdit &edit);
    void loadFromFile(QString fileName,QPlainTextEdit &edit);
    bool saveFile(QString fileName,QPlainTextEdit &edit);
    bool saveFileAs(QPlainTextEdit &edit);
    bool confirmSave(QPlainTextEdit &edit);
    void closeEvent(QCloseEvent *event);
    bool findTxt(QPlainTextEdit &edit);
    //void contextMenuEvent(QContextMenuEvent *event);
};
#endif // MAINWINDOW_H
