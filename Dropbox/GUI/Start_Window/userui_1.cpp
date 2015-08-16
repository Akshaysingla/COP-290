#include "userui_1.h"
#include <string.h>
#include <iostream>
#include <unistd.h>
#include "ui_userui_1.h"
#include <QFileInfo>
#include <QDateTime>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include "error_window.h"

UserUI_1::UserUI_1(int sock, SSL *sslt, QString userID, SSL_CTX *ctxt, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserUI_1)
{
    ssl = sslt;
    ctx = ctxt;
    client_socket = sock;
    homeqpath = getenv("HOME");
    homeqpath += "/MyDropBox_";
    homeqpath += userID;
    user_name = userID;

    QFileInfo Mydropbox(homeqpath);
    if (!Mydropbox.exists()){
        QDir dir = QDir::root();
        dir.mkdir(homeqpath);
    }

    QByteArray temp_id = homeqpath.toLocal8Bit();
    char *temp_id1 = temp_id.data();
    strcpy(homepath,temp_id1);
    std::cout << homepath << std::endl;
    Error_window error("Initial Sync in order. Please wait.");
    error.show();
    SyncAll(userID);
    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center()- this->rect().center());


    model = new QStringListModel(this);
    char fname[256], lname[256], welcome[256];
    strcpy ( welcome, "Welcome ");
    int status;
    do {
        status = SSL_read ( ssl, &fname, 256 );
    } while ( status < 0 );
    do {
        status = SSL_read ( ssl, &lname, 256 );
    } while ( status < 0 );
    strcat ( welcome, fname );
    strcat ( welcome, " ");
    strcat ( welcome, lname );
    strcat ( welcome, " !" );
    QString Welcome = QString::fromLocal8Bit(welcome);
    ui->label->setAlignment(Qt::AlignHCenter);
    ui->label->setText(Welcome);
    while(1){
        do {
            status = SSL_read ( ssl, &fname, 256 );
        } while ( status < 0 );
        std::cout << fname << std::endl;
        if (strcmp(fname,"")==0){
           break;
        }
        data << fname ;
   }
    ServerList();
    SharedByList();
    SharedWithList();

    model_2 = new QStringListModel(this);
    model_2->setStringList(browser_files);
    ui->listView_2->setModel(model_2);

    model_1 = new QDirModel(this);
    model_1->setReadOnly(false);
    model_1->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
    ui->treeView->setModel(model_1);
    QModelIndex index = model_1->index(homeqpath);
    ui->treeView->setRootIndex(index);
    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
    ui->treeView->resizeColumnToContents(0);
    error.close();
    }

UserUI_1::~UserUI_1()
{
    delete ui;
}

void UserUI_1::on_pushButton_clicked()
{
    int instruct = 21;
    char temp[255];
    int status;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    client_up_down = new Client_up_down(client_socket, ssl);
    for (int i = 0; i < browser_files.size() ; i++){
        QFileInfo curr_file( browser_files.at(i)) ;
        QString qfilename1 = curr_file.canonicalFilePath(); //maybe absolute
        std::cout << qfilename1.toStdString() << std::endl;
        QString qfilename = homeqpath + "/" + curr_file.fileName();
        QFile fi(qfilename);
        if (fi.exists() ){
            if (strcmp(client_up_down->create_sha1(qfilename.toStdString().c_str()).c_str(),client_up_down->create_sha1(qfilename1.toStdString().c_str()).c_str())!= 0){
            qfilename = homeqpath + "/Copy_" + curr_file.fileName();
            }
        }
        QFile::copy(qfilename1,qfilename);
        std::string filename = qfilename.toStdString();
        std::string *filepath = new std::string[2];
        client_up_down->SplitFilename (filename,filepath,homepath);
        strcpy(temp, filepath[0].c_str());
        strcat(temp, filepath[1].c_str());
        char path[510];
        const char* filename_1 = filepath[1].c_str();
        do {
            status = SSL_write( ssl, filename_1 , 256 );
        } while ( status < 0 );
        std::cout << "FILE NAME " << filename_1 << std::endl;
        int instruct;
        const char* filepath_2 = filepath[0].c_str();
        do {
            status = SSL_write( ssl, filepath_2, 256 );
        } while ( status < 0 );
        std::cout << "FILE PATH " << filepath_2 << std::endl;
        QString time_curr = curr_file.lastModified().toString("yyyyMMddhhmm.ss");
        const char* datetime = time_curr.toStdString().c_str();
        std::cout << "DATE " << datetime << std::endl;
        do {
            status = SSL_write( ssl, datetime, 256 );
        } while ( status < 0 );
        //bzero(&instruct,sizeof(&instruct));
        do {
            status = SSL_read( ssl, &instruct, 4 );
        } while ( status < 0 );
        std::cout << "RECEIVED INSTRUCTION " << instruct << std::endl;

        if (instruct == 15){
            std::cout << datetime << std::endl;
            const char* sdatetime = time_curr.toStdString().c_str();
            client_up_down->UploadFile_2(filename,sdatetime,homepath);
        }
    }
    std::cout << "END OF LOOP INSTRUCTION "  << std::endl;
    char* cancel = "";
    do {
        status = SSL_write( ssl, cancel, 256 );
    } while ( status < 0 );
    ServerList();
 }


void UserUI_1::on_pushButton_3_clicked()
{
    browser_files  = QFileDialog::getOpenFileNames();
    refresh_data();
}

void UserUI_1::refresh_data(){
    delete model_2;
    model_2 = new QStringListModel(this);
    model_2->setStringList(browser_files);
    ui->listView_2->setModel(model_2);
}


void UserUI_1::SyncAll(QString userID) {
    char fname[255]; //Receiving file name
    char path[510];
    char pathname[255];
    char lastedit[255];
    int status;
    int option = 1 ;
    char lsfilepath[500];
    strcpy(lsfilepath,"../../Client/.Config_");
    strcat(lsfilepath,userID.toStdString().c_str());
    strcat(lsfilepath,".txt");
    std::cout << lsfilepath << std::endl;
        client_up_down = new Client_up_down(client_socket, ssl);
    char* ls_time_local = client_up_down->check_ls_file(lsfilepath);
    std::cout << ls_time_local << std::endl;
    if (strcmp(ls_time_local , "0") == 0 ){
        option = 0;
    }
    else{
        option = 1;
    }
    do{
        status = SSL_write (ssl, &option, 4);
    } while (status < 0);

    std::cout << "SENT OPTION " << option << std::endl;
    if (option==0){
        QDir dir = QDir(homeqpath);
        while(1) {
            //bzero ( pathname, sizeof(pathname));
            do{
                status = SSL_read (ssl, &pathname, 256);
            } while (status < 0);
            if ( strcmp ( pathname, "" ) == 0 ){
                std::cout << "hgg" << pathname << std::endl;
                break;
            }
            QString qpathname;
            qpathname = homeqpath;
            qpathname += QString::fromUtf8(pathname);
            dir.mkpath(qpathname);
            do {
                status = SSL_read ( ssl, &fname, 256 );
            } while ( status < 0 );
            dir.mkpath(qpathname);
            strcpy ( path, homepath );
            strcat (path ,pathname);
            strcat (path, fname ) ;
            do {
                status = SSL_read ( ssl, &lastedit, 256 );
            } while ( status < 0 );
            std::cout << lastedit << std::endl;
            client_up_down->DownloadFile(path,lastedit);
        }
    }
    else {
        int status;
        char lastsync[255];
        do{
            status = SSL_read (ssl, &lastsync, 256);
        } while (status < 0);
        ls_time_local = client_up_down->check_ls_file(lsfilepath);
        int diff = strcmp ( lastsync, ls_time_local );
        std::cout << ls_time_local << std::endl;
        if ( diff <= 0 ) {
            std::cout << "option0 " << std::endl;
            int option = 0;
            do{
                status = SSL_write (ssl, &option, 4);
            } while (status < 0 );
                    SyncDirectory(homeqpath);
        }
        else if ( diff > 0 ) {
                        std::cout << "sdfsdfs" << std::endl;
            int option = 1;
            int status;
            do{
                status = SSL_write (ssl, &option, 4);
            } while (status < 0 );
            std::cout << option << std::endl;
            SyncServerClient(homeqpath);
            }
    }



    char* datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmm.ss").toLocal8Bit().data();
    char dtime[255];
    strcpy(dtime,datetime);
    do{
        status = SSL_write (ssl, &dtime, 256);
    } while (status < 0);
    client_up_down->write_ls_file(lsfilepath,dtime);
}



void UserUI_1:: SyncDirectory(QString directory_path){
    int status;
    QDirIterator directory_walker(directory_path, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while(directory_walker.hasNext())
    {
        directory_walker.next();
        QFileInfo curr_file = directory_walker.fileInfo() ;
        QString qfilename = curr_file.canonicalFilePath(); //maybe absolute
        std::cout << qfilename.toStdString() << std::endl;
        std::string filename = qfilename.toStdString();
        std::string *filepath = new std::string[2];
        client_up_down->SplitFilename (filename,filepath,homepath);
        char path[510];
        const char* filename_1 = filepath[1].c_str();
        do {
            status = SSL_write( ssl, filename_1 , 256 );
        } while ( status < 0 );
        std::cout << "FILE NAME " << filename_1 << std::endl;
        int instruct;
        const char* filepath_2 = filepath[0].c_str();
        do {
            status = SSL_write( ssl, filepath_2, 256 );
        } while ( status < 0 );
        std::cout << "FILE PATH " << filepath_2 << std::endl;
        QString time_curr = curr_file.lastModified().toString("yyyyMMddhhmm.ss");
        const char* datetime = time_curr.toStdString().c_str();
        std::cout << "DATE " << datetime << std::endl;
        do {
            status = SSL_write( ssl, datetime, 256 );
        } while ( status < 0 );
        //bzero(&instruct,sizeof(&instruct));
        do {
            status = SSL_read( ssl, &instruct, 4 );
        } while ( status < 0 );
        std::cout << "RECEIVED INSTRUCTION " << instruct << std::endl;

        if (instruct == 15){
            std::cout << datetime << std::endl;
            const char* sdatetime = time_curr.toStdString().c_str();
            client_up_down->UploadFile_2(filename,sdatetime,homepath);
        }
    }
    std::cout << "END OF LOOP INSTRUCTION "  << std::endl;
    char* cancel = "";
    do {
        status = SSL_write( ssl, cancel, 256 );
    } while ( status < 0 );
}

void UserUI_1::SyncServerClient(QString directory_path) {
    char fname[255]; //Receiving file name
    char path[510];
    char pathname[255];
    char lastedit[255];
    int status;
    QDir dir = QDir(homeqpath);
    QDirIterator directory_walker(directory_path, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while(directory_walker.hasNext())
    {

        directory_walker.next();
        QFileInfo curr_file = directory_walker.fileInfo() ;
        QString qfilename = curr_file.canonicalFilePath(); //maybe absolute
        std::cout << qfilename.toStdString() << std::endl;
        std::string filename = qfilename.toStdString();
        std::string *filepath = new std::string[2];
        client_up_down->SplitFilename (filename,filepath,homepath);
        const char* filename_1 = filepath[1].c_str();
        do {
            status = SSL_write( ssl, filename_1 , 256 );
        } while ( status < 0 );
        std::cout << "FILE NAME " << filename_1 << std::endl;
        int instruct;
        const char* filepath_2 = filepath[0].c_str();
        do {
            status = SSL_write( ssl, filepath_2, 256 );
        } while ( status < 0 );
        std::cout << "FILE PATH " << filepath_2 << std::endl;
        QString time_curr = curr_file.lastModified().toString("yyyyMMddhhmm.ss");
        const char* datetime = time_curr.toStdString().c_str();
        std::cout << "DATE " << datetime << std::endl;
        do {    char path[510];
            status = SSL_write( ssl, datetime, 256 );
        } while ( status < 0 );

        do {
            status = SSL_read( ssl, &instruct, 4 );
        } while ( status < 0 );
                std::cout << "Instruct " << instruct << std::endl;
        if (instruct == 15){
            const char* sdatetime = time_curr.toStdString().c_str();
            client_up_down->UploadFile_2(filename,sdatetime,homepath);
        }
        else if(instruct == 16){
            //bzero ( pathname, sizeof(pathname));
            do{
                status = SSL_read (ssl, &pathname, 256);
            } while (status < 0);
            if ( strcmp ( pathname, "" ) == 0 ){
                break;
            }

            QString qpathname;
            qpathname = homeqpath;
            qpathname += QString::fromUtf8(pathname);
            dir.mkpath(qpathname);
            do {
                status = SSL_read ( ssl, &fname, 256 );
            } while ( status < 0 );
            dir.mkpath(qpathname);
            strcpy ( path, homepath );
            strcat (path ,pathname);
            strcat (path, fname ) ;
            do {
                status = SSL_read ( ssl, &lastedit, 256 );
            } while ( status < 0 );
            std::cout << lastedit << std::endl;
            client_up_down->DownloadFile(path,lastedit);
        }
        //else if(instruct == 17){
         //   strcpy (path, "rm -f ");
         //   strcat ( path, homepath );
         //   strcat (path ,pathname);
         //   strcat (path, fname ) ;
         //   const char* rmv_dir = path;
         //   system(rmv_dir);
        //}
    }
    char* cancel = "";
    do {
        status = SSL_write( ssl, cancel, 256 );
    } while ( status < 0 );
    while(1) {
        //bzero ( pathname, sizeof(pathname));
        do{
            status = SSL_read (ssl, &pathname, 256);
        } while (status < 0);
        if ( strcmp ( pathname, "" ) == 0 ){
            break;
        }

        QString qpathname;
        qpathname = homeqpath;
        qpathname += QString::fromUtf8(pathname);
        dir.mkpath(qpathname);
        do {
            status = SSL_read ( ssl, &fname, 256 );
        } while ( status < 0 );
        dir.mkpath(qpathname);
        strcpy ( path, homepath );
        strcat (path ,pathname);
        strcat (path, fname ) ;
        do {
            status = SSL_read ( ssl, &lastedit, 256 );
        } while ( status < 0 );
        std::cout << lastedit << std::endl;
        client_up_down->DownloadFile(path,lastedit);
    }
}

void UserUI_1::SendFileInfo(QString filepath){
    int status;
    client_up_down = new Client_up_down(client_socket, ssl);
    QFileInfo curr_file( filepath) ;
    QString qfilename = curr_file.canonicalFilePath(); //maybe absolute
    std::string filename = qfilename.toStdString();
    std::string *filepath1 = new std::string[2];
    client_up_down->SplitFilename (filename,filepath1,homepath);
    const char* filename_1 = filepath1[1].c_str();
    do {
        status = SSL_write( ssl, filename_1 , 256 );
    } while ( status < 0 );
    std::cout << "FILE NAME " << filename_1 << std::endl;
    int instruct;
    const char* filepath_2 = filepath1[0].c_str();
    do {
        status = SSL_write( ssl, filepath_2, 256 );
    } while ( status < 0 );
    std::cout << "FILE PATH " << filepath_2 << std::endl;
    QString time_curr = curr_file.lastModified().toString("yyyyMMddhhmm.ss");
    const char* datetime = time_curr.toStdString().c_str();
    std::cout << "DATE " << datetime << std::endl;
    do {
        status = SSL_write( ssl, datetime, 256 );
    } while ( status < 0 );
}



void UserUI_1::ChangePriveledgeShare(QString file, QString Sharee,int priv){
    int status;
    const char* file_name = file.toStdString().c_str();
    do {
        status = SSL_write( ssl, file_name, 256 );
    } while ( status < 0 );
    std::cout << file_name << std::endl;
    const char* sharee = Sharee.toStdString().c_str();
    do {
        status = SSL_write( ssl, sharee, 256 );
    } while ( status < 0 );
    std::cout << sharee << std::endl;
    do {
        status = SSL_write( ssl, &priv, 4 );
    } while ( status < 0 );
    std::cout << priv << std::endl;
}


void UserUI_1::Unshare(QString file, QString Sharee, int priv){
    int status;
    const char* file_name = file.toStdString().c_str();
    do {
        status = SSL_write( ssl, file_name, 256 );
    } while ( status < 0 );
    std::cout << file_name << std::endl;
    const char* sharee = Sharee.toStdString().c_str();
    do {
        status = SSL_write( ssl, sharee, 256 );
    } while ( status < 0 );
    std::cout << sharee << std::endl;
    do {
        status = SSL_write( ssl, &priv, 4 );
    } while ( status < 0 );
    std::cout << priv << std::endl;
}


void UserUI_1::on_pushButton_5_clicked()
{
    Error_window error("Sync in order. Please wait.");
    error.show();
    int instruct = 20;
    int status;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    SyncAll(user_name);
    ServerList();
    SharedByList();
    SharedWithList();
    error.close();
}

void UserUI_1::on_pushButton_6_clicked()
{
    this->close();
    int instruct = 40;
    int status;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    ::close (client_socket);
    SSL_free (ssl);
    SSL_CTX_free ( ctx );
}

void UserUI_1::on_pushButton_20_clicked()
{
    this->lower();
    QDesktopServices::openUrl( QUrl::fromLocalFile( homeqpath ) );
}

void UserUI_1::on_pushButton_10_clicked()
{
    this->lower();
    QDesktopServices::openUrl( QUrl::fromLocalFile( homeqpath ) );
}

void UserUI_1::ServerList(){
    int status;
    char file[255], path[255];
    ServerRow = 0;
    int ServerCol = 0;
    while (1) {
        do {

            status = SSL_read ( ssl, &file, 256 );
        } while ( status < 0 );
        if ( strcmp ( file, "" ) == 0 )
            break;
        do {
            status = SSL_read ( ssl, &path, 256 );
        } while ( status < 0 );
        QString File = QString::fromLocal8Bit(file);
        QString Path = QString::fromLocal8Bit(path);
        ServerRow++;
        ui->ServerList->setRowCount(ServerRow);
        ui->ServerList->setItem(0, ServerCol, new QTableWidgetItem(File));
        (ServerCol+=1)%2;
        ui->ServerList->setItem(0, ServerCol, new QTableWidgetItem(Path));
        (ServerCol+=1)%2;
    }
}


void UserUI_1::SharedByList(){
    int status;
    SharedRow1 = 0;
    int SharedCol1 = 0;
    char namerec[255], sharedby[255];
    QString NameRec, Perm, SharedBy;
    int perm;
    std::cout<<"Ready to receive Shared Data 1!"<<std::endl;
    while (1) {
        do {
            status = SSL_read ( ssl, &namerec, 256 );
        } while ( status < 0 );
        std::cout<<"Got First write"<<std::endl;
        if ( strcmp ( namerec, "" ) == 0 )
            break;
        do {
            status = SSL_read ( ssl, &sharedby, 256 );
        } while ( status < 0 );
        std::cout<<"Got Second Write"<<std::endl;
        do {
            status = SSL_read ( ssl, &perm, 4 );
        } while ( status < 0 );
        NameRec = QString::fromLocal8Bit(namerec);
        if ( perm == 1 ) {
            Perm = "Transfer";
        }
        else if ( perm == 0) {
            Perm = "Collaborate";
        }
        SharedBy = QString::fromLocal8Bit(sharedby);
        std::cout<<namerec<<" "<<SharedRow1<<std::endl;

        SharedRow1++;
        ui->SharedList1->setRowCount(SharedRow1);
        ui->SharedList1->setItem(0, SharedCol1, new QTableWidgetItem(NameRec));
        (SharedCol1+=1)%3;
        ui->SharedList1->setItem(0, SharedCol1, new QTableWidgetItem(SharedBy));
        (SharedCol1+=1)%3;
        ui->SharedList1->setItem(0, SharedCol1, new QTableWidgetItem(Perm));
        (SharedCol1+=1)%3;
    }
}

void UserUI_1::SharedWithList(){
    int status;
    char namesend[255], sharee[255];
    QString NameSend, Sharee;
    int perm;
    QString SetPerm;
    SharedRow2 = 0;
    int SharedCol2 = 0;
    std::cout<<"Ready to receive Shared Data 2!"<<std::endl;
    while (1) {
        do {
            status = SSL_read ( ssl, &namesend, 256 );
        } while ( status < 0 );
        if ( strcmp ( namesend, "" ) == 0 )
            break;
        do {
            status = SSL_read ( ssl, &sharee, 256 );
        } while ( status < 0 );
        do {
            status = SSL_read ( ssl, &perm, 4 );
        } while ( status < 0 );
        NameSend = QString::fromLocal8Bit(namesend);
        //perm = atoi (setperm);
        if ( perm == 1 ) {
            SetPerm = "Transfer";
        }
        else if ( perm == 0) {
            SetPerm = "Collaborate";
        }
        Sharee = QString::fromLocal8Bit(sharee);
        SharedRow2++;
        ui->SharedList2->setRowCount(SharedRow2);
        ui->SharedList2->setItem(0, SharedCol2, new QTableWidgetItem(NameSend));
        (SharedCol2+=1)%3;
        ui->SharedList2->setItem(0, SharedCol2, new QTableWidgetItem(Sharee));
        (SharedCol2+=1)%3;
        ui->SharedList2->setItem(0, SharedCol2, new QTableWidgetItem(SetPerm));
        (SharedCol2+=1)%3;
    }
}

void UserUI_1::on_pushButton_9_clicked()
{
    QString filname,filpath;
    QModelIndexList shareserver = ui->ServerList->selectionModel()->selectedRows();
    if (shareserver.count() !=1 ){
        QMessageBox::information(this,"","Select Exactly 1 from the server list");
    }
    else{
        filname = ui->ServerList->item(shareserver.at(0).row(),0)->text();
        filpath = ui->ServerList->item(shareserver.at(0).row(),1)->text();
        std::cout << filname.toStdString() << filpath.toStdString() << std::endl;
        sharewin = new ShareSearch(client_socket,ssl,ctx,data, filname , filpath);
        sharewin->show();
    }
}

void UserUI_1::on_pushButton_7_clicked()   //Delete Selected Files
{
    int status;
    int instruct = 22;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );

    QString filname;
    QString filpath;
    QModelIndexList shareserver = ui->ServerList->selectionModel()->selectedRows();
    for (int i = 0 ; i < shareserver.count() ; i++){
        filname = ui->ServerList->item(shareserver.at(i).row(),0)->text();
        filpath = ui->ServerList->item(shareserver.at(i).row(),1)->text();
        QString file = homeqpath + filpath + filname;
        QString sys = "rm -f "  + file;
        system(sys.toStdString().c_str());
        char* datetime = QDateTime::currentDateTime().toString("yyyyMMddhhmm.ss").toLocal8Bit().data();
        char dtime[255];
        strcpy(dtime,datetime);
        do {
            status = SSL_write ( ssl, dtime, 250 );
        } while ( status < 0 );
        do {
            status = SSL_write ( ssl, filname.toStdString().c_str(), 256 );
        } while ( status < 0 );
        do {
            status = SSL_write ( ssl, filpath.toStdString().c_str(), 256 );
        } while ( status < 0 );
    }
    char* cancel = "";
    do {
        status = SSL_write( ssl, cancel, 256 );
    } while ( status < 0 );
    ServerList();
}


void UserUI_1::on_commandLinkButton_clicked()
{
    readme1 = new ReadMe("../../Readme.txt");
    readme1->show();
}

void UserUI_1::on_pushButton_16_clicked() // change to transfer
{
    QString filpath;
    QString filname;
    QString priv;
    QModelIndexList shareserver = ui->SharedList1->selectionModel()->selectedRows();
    filname = ui->SharedList1->item(shareserver.at(0).row(),0)->text();
    filpath = ui->SharedList1->item(shareserver.at(0).row(),1)->text();
    priv = ui->SharedList1->item(shareserver.at(0).row(),2)->text();
    if (priv == "Transfer"){
        return;
    }
    else{
        int status;
        int k = 1;
        int instruct = 26;
        do {
            status = SSL_write( ssl, &instruct, 4 );
        } while ( status < 0 );
        ChangePriveledgeShare(filname,filpath,k);
        SharedByList();
        SharedWithList();
    }
}

void UserUI_1::on_pushButton_17_clicked()
{
    QString filpath;
    QString filname;
    QString priv;
    QModelIndexList shareserver = ui->SharedList1->selectionModel()->selectedRows();
    filname = ui->SharedList1->item(shareserver.at(0).row(),0)->text();
    filpath = ui->SharedList1->item(shareserver.at(0).row(),1)->text();
    priv = ui->SharedList1->item(shareserver.at(0).row(),2)->text();
    if (priv == "Collaborate"){
        return;
    }
    else{
        int status;
        int k = 0;
        int instruct = 26;
        do {
            status = SSL_write( ssl, &instruct, 4 );
        } while ( status < 0 );
        ChangePriveledgeShare(filname,filpath,k);
        SharedByList();
        SharedWithList();
    }
}

void UserUI_1::on_pushButton_18_clicked()
{
    QString filpath;
    QString filname;
    QString priv;
    QModelIndexList shareserver = ui->SharedList1->selectionModel()->selectedRows();
    filname = ui->SharedList1->takeItem(shareserver.at(0).row(),0)->text();
    filpath = ui->SharedList1->takeItem(shareserver.at(0).row(),1)->text();
    priv = ui->SharedList1->takeItem(shareserver.at(0).row(),2)->text();
    int status;
    int k ;
    if (priv == "Collaborate"){
        k = 0;
    }
    else{
        k = 1;
    }
    int instruct = 27;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    Unshare( filname,  filpath, k);
    SharedByList();
    SharedWithList();
}

void UserUI_1::on_pushButton_13_clicked()
{
    char fname[255]; //Receiving file name
    char path[510];
    char pathname[255];
    char lastedit[255];
    QString qpathname;
    QDir dir = QDir(homeqpath);
    QString shared;
    QString filname;
    QString priv;
    int instruct = 28;
    int status;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    QModelIndexList shareserver = ui->SharedList2->selectionModel()->selectedRows();
    for (int i = 0 ; i < shareserver.count(); i++){
        filname = ui->SharedList2->item(shareserver.at(i).row(),0)->text();
        shared = ui->SharedList2->item(shareserver.at(i).row(),1)->text();
        priv = ui->SharedList2->item(shareserver.at(i).row(),2)->text();

        int k ;
        if (priv == "Collaborate"){
            k = 0;
        }
        else{
            k = 1;
        }
        const char* file_name = filname.toStdString().c_str();
        do {
            status = SSL_write( ssl, file_name, 256 );
        } while ( status < 0 );
        const char* sharee = shared.toStdString().c_str();
        do {
            status = SSL_write( ssl, sharee, 256 );
        } while ( status < 0 );
        do {
            status = SSL_write( ssl, &k, 4 );
        } while ( status < 0 );

        do{
            status = SSL_read (ssl, &pathname, 256);
        } while (status < 0);

        qpathname = homeqpath;
        qpathname += "/Shared/";
        dir.mkpath(qpathname);
        do {
            status = SSL_read ( ssl, &fname, 256 );
        } while ( status < 0 );
        dir.mkpath(qpathname);
        strcpy ( path, homepath );
        strcat (path ,pathname);
        strcat (path, fname ) ;
        do {
            status = SSL_read ( ssl, &lastedit, 256 );
        } while ( status < 0 );
        std::cout << lastedit << std::endl;
        client_up_down->DownloadFile(path,lastedit);
    }
    const char* cancel = "";
    do {
        status = SSL_write( ssl, cancel, 256 );
    } while ( status < 0 );
}


void UserUI_1::on_pushButton_19_clicked()
{
    this->lower();
    QDir dir = QDir(homeqpath);
    dir.mkpath(homeqpath +"/Shared/");
    QDesktopServices::openUrl( QUrl::fromLocalFile( homeqpath +"/Shared/" ) );
}

void UserUI_1::on_pushButton_21_clicked()
{
    browser_files.clear();
}

void UserUI_1::on_pushButton_4_clicked()
{
    QDir dir = QDir(homeqpath);
    QString temp1 = ui->lineEdit_2->text();
    if ( temp1 == ""){
        QMessageBox::information(this,"","Please enter folder name");
    }

    int instruct = 21;
    char temp[255];
    int status;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    client_up_down = new Client_up_down(client_socket, ssl);
    for (int i = 0; i < browser_files.size() ; i++){
        QFileInfo curr_file( browser_files.at(i)) ;
        QString qfilename1 = curr_file.canonicalFilePath(); //maybe absolute
        std::cout << qfilename1.toStdString() << std::endl;
        QString qfilename = homeqpath + "/" + temp1 + "/";
        dir.mkpath(qfilename);
        qfilename += curr_file.fileName();
        QFile fi(qfilename);
        if (fi.exists() ){
            if (strcmp(client_up_down->create_sha1(qfilename.toStdString().c_str()).c_str(),client_up_down->create_sha1(qfilename1.toStdString().c_str()).c_str())!= 0){
            qfilename = homeqpath + "/Copy_" + curr_file.fileName();
            }
        }
        QFile::copy(qfilename1,qfilename);
        std::string filename = qfilename.toStdString();
        std::string *filepath = new std::string[2];
        client_up_down->SplitFilename (filename,filepath,homepath);
        strcpy(temp, filepath[0].c_str());
        strcat(temp, filepath[1].c_str());
        char path[510];
        const char* filename_1 = filepath[1].c_str();
        do {
            status = SSL_write( ssl, filename_1 , 256 );
        } while ( status < 0 );
        std::cout << "FILE NAME " << filename_1 << std::endl;
        int instruct;
        const char* filepath_2 = filepath[0].c_str();
        do {
            status = SSL_write( ssl, filepath_2, 256 );
        } while ( status < 0 );
        std::cout << "FILE PATH " << filepath_2 << std::endl;
        QString time_curr = curr_file.lastModified().toString("yyyyMMddhhmm.ss");
        const char* datetime = time_curr.toStdString().c_str();
        std::cout << "DATE " << datetime << std::endl;
        do {
            status = SSL_write( ssl, datetime, 256 );
        } while ( status < 0 );
        //bzero(&instruct,sizeof(&instruct));
        do {
            status = SSL_read( ssl, &instruct, 4 );
        } while ( status < 0 );
        std::cout << "RECEIVED INSTRUCTION " << instruct << std::endl;

        if (instruct == 15){
            std::cout << datetime << std::endl;
            const char* sdatetime = time_curr.toStdString().c_str();
            client_up_down->UploadFile_2(filename,sdatetime,homepath);
        }
    }
    std::cout << "END OF LOOP INSTRUCTION "  << std::endl;
    char* cancel = "";
    do {
        status = SSL_write( ssl, cancel, 256 );
    } while ( status < 0 );
    ServerList();



}
