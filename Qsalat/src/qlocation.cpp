#include "qlocation.h"
//
Qlocation::Qlocation( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	setWindowIcon(QIcon("images/mecque.png"));		
	manager = new QNetworkAccessManager(this);	
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    //connect(this,SIGNAL(loadFinished(ok)),this,SLOT(updateLatLng()));
    connect(this,SIGNAL(reloadMap()),this,SLOT(loadCoordinates()));
    connect(pushButton,SIGNAL(clicked()),this,SLOT(showItem()));  
    connect(pushButton_2,SIGNAL(clicked()),this,SLOT(updateLatLng()));    
}

void Qlocation::geoCode(const QString& address)
{
	adress = address;
    clearCoordinates();
    QString requestStr( tr("http://maps.google.com/maps/geo?q=%1&output=%2&key=%3")
            .arg(address)
            .arg("csv")
            .arg("ABQIAAAADK2Z-2Uhvnv0BtjasvfxpBQ9r-JX5Ew3YiWGH5u4o4FZAQdnQRRB9I6-zAG7Un8nsXxMTDtquMbY2A") );
	//QMessageBox::warning(this, tr("My Application"),requestStr,QMessageBox::Ok);
    manager->get( QNetworkRequest(requestStr) );
    ++pendingRequests;
}

void Qlocation::replyFinished(QNetworkReply *reply)
{
    QString replyStr( reply->readAll() );
    QStringList coordinateStrList = replyStr.split(",");

    if( coordinateStrList.size() == 4) {
        QPointF coordinate( coordinateStrList[2].toFloat(),coordinateStrList[3].toFloat() );
        coordinates << coordinate;        
    }

    --pendingRequests;
    if( pendingRequests<1 )
        emit( reloadMap() );
}

void Qlocation::loadCoordinates()
{
    QStringList scriptStr;    
    foreach( QPointF point, coordinates ) {
        label_2->setText(QString::number(point.x()));
        label_3->setText(QString::number(point.y()));
    }   
   scriptStr << QString("http://www.skanderjabouzi.com/qpray/?adr=%1")                        
                             .arg(adress);
    
    QUrl url(scriptStr.join("\n"));
    this->webView->load(url);
    //scriptStr << "document.getElementById(\"lat\").value;";
    //scriptStr << "alert(123);";
    //QVariant vres = webView->page()->mainFrame()->evaluateJavaScript( scriptStr.join("\n") );
    //QString sres = vres.toString();
    //QMessageBox::warning(this, tr("My Application"),sres,QMessageBox::Ok);
}

void Qlocation::clearCoordinates()
{
    coordinates.clear();   
}

void Qlocation::showItem()
{
    geoCode( lineEdit->text() );    
}

void Qlocation::updateLatLng(){
	QStringList scriptStr1,scriptStr2; 
	scriptStr1 << "document.getElementById(\"lat\").value;";
	QVariant vres1 = webView->page()->mainFrame()->evaluateJavaScript( scriptStr1.join("\n") );
    QString sres1 = vres1.toString();
    scriptStr2 << "document.getElementById(\"lng\").value;";
	QVariant vres2 = webView->page()->mainFrame()->evaluateJavaScript( scriptStr2.join("\n") );
    QString sres2 = vres2.toString();
    label_2->setText(sres1);
    label_3->setText(sres2);
    //QMessageBox::warning(this, tr("My Application"),sres1,QMessageBox::Ok);
}

void Qlocation::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}
//