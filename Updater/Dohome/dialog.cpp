#include <QDesktopServices>
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(char *appd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    QFile version_txt("dohome/version.txt");
    QString current_version;
    if (version_txt.open(QIODevice::ReadOnly)){
        current_version = version_txt.readLine();
    } else {
        current_version = "0.0.0";
    }
    version_txt.close();

    application_directory = new QString(appd);

    zip_out = new QFile(QString("temp/new.zip"));

    unzip_process = new QProcess();

    del_last_version_process = new QProcess();

    ui->setupUi(this);
    ui->debugDisplay->append(QString("Recherche de nouvelles versions, Actuelle : " + current_version));

    serveur_search = new QHttp("mkookies.hebergratuit.com");
    serveur_search->setHost("mkookies.hebergratuit.com");
    serveur_dl = new QHttp("mkookies.hebergratuit.com");
    serveur_dl->setHost("mkookies.hebergratuit.com");

    serveur_search->get(QString("/dohome/update.php?v=") + current_version);
    QObject::connect(serveur_search, SIGNAL(done(bool)), this, SLOT(search_updates(bool)));
}

bool Dialog::search_updates(bool error){
    if(!(error)){
        ui->debugDisplay->append(QString("Réponse reçue"));
        QString result = serveur_search->readAll();

        if (result != QString("-1")){
            if(result != QString("0")){
                ui->debugDisplay->append(QString("Une nouvelle version est disponible, celle-ci va maintenant etre telechargee"));
                serveur_dl->get(result);
                ui->debugDisplay->append(QString("Telechargement en cours"));

                QObject::connect(serveur_dl, SIGNAL(done(bool)), this, SLOT(unzip_update(bool)));
            } else {
                ui->debugDisplay->append(QString("Vous avez la derniere version, lancement de l'application : ") + QString("file:///") + application_directory->replace(QLatin1Char('\\'), QLatin1Char('/')) + QString("dohome/index.html"));
                QDesktopServices::openUrl(QString("file:///") + application_directory->replace(QLatin1Char('\\'), QLatin1Char('/')) + QString("dohome/index.html"));
            }
        } else {
            ui->debugDisplay->append(QString("La reponse indique une erreur"));
        }
        
        return true;
    } else {
        ui->debugDisplay->append(QString("Echec de la requete, le serveur de mise a jour est probablement invalide"));
        
        return false;
    }
}

bool Dialog::unzip_update(bool error){
    if(!(error)){
        QStringList arg_del_last_version;

        ui->debugDisplay->append(QString("Telechargement termine"));

        zip_out->open(QIODevice::ReadWrite);
        zip_out->write(serveur_dl->readAll());
        zip_out->close();

        ui->debugDisplay->append(QString("Suppression de l'ancienne version"));

        del_last_version_process->start(QString("cmd.exe"));
        del_last_version_process->write("rmdir dohome /S /Q\n");
        del_last_version_process->write("exit\n");

        QObject::connect(del_last_version_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(apply_update(int)));


        return true;
    } else {
        ui->debugDisplay->append(QString("La mise a jour n'a pas pu être telechargee"));
        
        return false;
    }
}

bool Dialog::apply_update(int exit_code){
    if (exit_code == EXIT_SUCCESS){
        QStringList arg_unzip;

        ui->debugDisplay->append(QString("Décompression de la mise a jour en cours"));

        arg_unzip << "-o" << "-u" << "temp/new.zip" << "-d" << "dohome";
        unzip_process->start(QString("unzip.exe"), arg_unzip);

        QObject::connect(unzip_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(verify_update(int)));

        return true;
    } else {

        return false;
    }
}

bool Dialog::verify_update(int exit_code){
    if (exit_code == EXIT_SUCCESS){
        ui->debugDisplay->append(QString("Décompression terminée\nSuppression du fichier compréssé"));
        if (zip_out->remove()){
            ui->debugDisplay->append(QString("Mise a jour terminée avec succes, lancement de l'application"));
            ui->debugDisplay->append(QString("Vous avez la derniere version, lancement de l'application : ") + QString("file:///") + application_directory->replace(QLatin1Char('\\'), QLatin1Char('/')) + QString("dohome/index.html"));
            QDesktopServices::openUrl(QString("file:///") + application_directory->replace(QLatin1Char('\\'), QLatin1Char('/')) + QString("dohome/index.html"));

            return true;
        } else {
            ui->debugDisplay->append(QString("Erreur lors de la supression du fichier compréssé"));

            return false;
        }
    } else {
        ui->debugDisplay->append(QString("Erreur lors de la decompression de la nouvelle mise a jour"));        

        return false;
    }
}

Dialog::~Dialog()
{
    delete ui;
    delete serveur_dl;
    delete serveur_search;
    delete unzip_process;
    delete del_last_version_process;
    delete zip_out;
}
