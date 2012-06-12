#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHttp>
#include <QProcess>
#include <QFile>
#include <QDir>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(char *appd, QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog  *ui;
    QHttp       *serveur_search, *serveur_dl;
    QProcess    *unzip_process, *del_last_version_process;
    QFile       *zip_out;
    QString     *application_directory;

public slots:
    bool search_updates(bool);
    bool apply_update(int);
    bool verify_update(int);
    bool unzip_update(bool);
};

#endif // DIALOG_H
