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
	//视频相关ID
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
	//获取 mv 播放地址
	void getMovieUrl(const long long& id);
	void getMovieUrl(const QString& id);
	void setMessage(const long long artid,const QString &cover,const QString &art,const QString &title);

	//获取视频评论
	void getVideoComment();
	//获取相关视频
	void getRelatedVideo(const QString &str);
	void addBtn_Title();

	//评论
	void CommentWidget(long long id);

signals:
	//返回上一级
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
	//评论UI
	Remark* remark;

	QList<Related_Video>ListRealte{};

	//相关推荐title集合
	QList<QPushButton*> btn_Title{};
	QList<QPushButton*> btn_RelatedPic{};
	QList<QPushButton*> btn_UserName{};

	QMediaPlayer* player;
	QVideoWidget* videowidget;

	QNetworkAccessManager* manger;
	//MV ID为long long 的类型时
	QNetworkReply* VideoURL{};
	//视频评论
	QNetworkReply* VideoComment{};
	QNetworkReply* RelatedVideo{};
	QNetworkReply* artpix{};
};


#endif // PLAYERVIDEO_H
