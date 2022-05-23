#ifndef MOVIE_H
#define MOVIE_H
#include <thread>
#include <QWidget>
#include <QNetworkReply>
#include <QPushButton>
#include "config.h"
class Config;
class QGridLayout;
class PlayerVideo;
class QMouseEvent;
class QNetworkAccessManager;
namespace Ui {
	class Movie;
}


struct _MV {
	long long id{};
	QString name{};
	QString cover{};
	QString artistName{};
	long long playCount{};
	long long artistId{};

};

//��ƵUI��widget
struct VideoWidget
{
	QPushButton* pic{};
	QPushButton* title{};
	QPushButton* artName{};
};

struct VideoTagId
{
	//�ֳ�
	int scene{};
	//����
	int cover{};
	//�赸
	int dance{};
	//��BGM
	int BGM{};
	//MV
	int MV{};
	//����
	int live{};
	//��Ϸ
	int game{};
	//ACG����
	int ACG{};
	//��ѷ���
	int fanzhi{};
};

//��Ƶ��
struct _VideoGroud
{
	long long userId{};
	QString nickname{};
	QString coverUrl{};
	QString title{};
	QString description{};
	QString vid{};
	QString durationms{};
	QString playTime{};
};

class Movie : public QWidget
{
	Q_OBJECT

public:
	explicit Movie(QWidget* parent = nullptr);
	virtual  ~Movie();

	//��ȡ��Ƶ��ǩ
	void getVideoTag();
	//��Ӧ��ǩ����Ƶ��Ĭ���ֳ���
	void videoGroupId(const int id = 58100);
	void getNewMV(const int limit);
	void getWyMv(const int limit);
	//����MV��Ƶ����Ϣ
	void parseNewMV(const QJsonObject& rot, const QString);
	void addNewMV(const QPixmap& map, int index);
	//����MV�Ŀؼ�����
	void addNewMvList();
	//���׳�ƷMV�ؼ�����
	void addWyMv();
	//������Ƶ�����Ƶ
	void ParseVideoGroud(QJsonValue& val);

	//��ƵUI�ؼ�
	void CreatorVideoWidget(const int index, const QPixmap& pic);
	PlayerVideo* getPlayerVideo();

signals:
	void HideTheSidebar();
	void ShowTheSidebar();
public slots:
	//����MV�����Ϣ
	void RequestMvMessage();
	void clicked();
protected slots:

	void mousePressEvent(QMouseEvent* event);
	void on_tabclicked(int index);
	void on_getpic();
	void on_finshedNewMv();
	void on_finshedVideoGroud();

	//����MV����Ŀؼ������
	void on_MVClicked();

	//���׳�ƷMV����Ŀؼ������
	void on_WyMVClicked();

private:
	Ui::Movie* ui;
	Config conf{};
	int videwidgetindex{};
	std::thread thread{};

	int NewMVindex{};
	int wy_index{};

	VideoTagId tagId{};

	PlayerVideo* playervideo;
	QList<VideoWidget> videwidget{};

	//����MV
	QList<_MV> mvmsg{};
	QList<QPushButton*> mv_btn{};
	QList<QPushButton*> mv_artName{};
	QList<QPushButton*>mv_songName{};

	//���׳�Ʒ
	QList<_MV> wy;
	QList<QPushButton*> wy_mv_pic{};
	QList<QPushButton*> wy_title{};

	//��Ƶ��
	QList<_VideoGroud>vglist{};

	QNetworkAccessManager* manger;
	QNetworkRequest* request{};
	//��ȡ��Ƶ��ǩ
	QNetworkReply* taglist{};
	QNetworkReply* NewMv{};
	QNetworkReply* WyMv{};
	QNetworkReply* pic{};
	QNetworkReply* VideoGroud{};
	QNetworkReply* getpic{};
};

#endif // MOVIE_H
