#ifndef QSALAT_H
#define QSALAT_H
//
#include <QMainWindow>
#include <QtGui>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include "ui_salat.h"
#include "qpray.h"
#include "qhijri.h"
#include "qlocation.h"
//
class Qsalat : public QMainWindow, public Ui::Salat
{
Q_OBJECT
public:
	Qsalat( QWidget * parent = 0, Qt::WFlags f = 0 );
	void getSalats();
	void getHijri();		
	Qpray *prayers;
	Qhijri *hijri;
	Qlocation location;
	QDate date;
protected:
	int year;
	int month;
	int day;
	double latitude;
	double longitude;
	double timezone;
	void closeEvent(QCloseEvent *);
	void adjustWindow();
	
private:    
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    //QAction *action_Close;
    //QAction *action_Hide;
	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	void createActions();
	void createTrayIcon();
	void setVisible(bool);		
	bool Gfirst; 
	bool locationFirst;
	
private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason);
	void editLocation();
};
#endif




