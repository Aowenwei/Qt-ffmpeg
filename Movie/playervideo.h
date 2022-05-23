#ifndef PLAYERVIDEO_H
#define PLAYERVIDEO_H

#include <QPushButton>
#include <QWidget>
namespace Ui {
	class PlayerVideo;
}

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>

class Remark;
class QPushButton;

struct Related_Video {
	QString userName{};
	QString title{};
	//��Ƶ���ID
	QString MvId{};
	QString cover{};
	long long userId{};
	long long playCount{};

};


class PlayerVideo : public QWidget
{
	Q_OBJECT
public:
	explicit PlayerVideo(QWidget* parent = nullptr);
	~PlayerVideo();
	//��ȡ mv ���ŵ�ַ
	void getMovieUrl(const long long& id);
	void getMovieUrl(const QString& id);
	void setMessage(const long long artid,const QString &cover,const QString &art,const QString &title);

	//��ȡ��Ƶ����
	void getVideoComment();
	//��ȡ�����Ƶ
	void getRelatedVideo(const QString &str);
	void addBtn_Title();

	//����
	void CommentWidget(long long id);

signals:
	//������һ��
	void back();
protected slots:
	void on_PicClicked();
	void on_TitleClicked();
	void on_UserNameClicked();

	void on_finshedVideoComment();
	void on_finshedRelatedVideo();
	void on_btn_back_clicked();
private:
	Ui::PlayerVideo* ui;
	QString MVID{};
	//����UI
	Remark* remark;

	QList<Related_Video>ListRealte{};

	//����Ƽ�title����
	QList<QPushButton*> btn_Title{};
	QList<QPushButton*> btn_RelatedPic{};
	QList<QPushButton*> btn_UserName{};

	QMediaPlayer* player;
	QVideoWidget* videowidget;

	QNetworkAccessManager* manger;
	//MV IDΪlong long ������ʱ
	QNetworkReply* VideoURL{};
	//��Ƶ����
	QNetworkReply* VideoComment{};
	QNetworkReply* RelatedVideo{};
	QNetworkReply* artpix{};
};


#endif // PLAYERVIDEO_H
