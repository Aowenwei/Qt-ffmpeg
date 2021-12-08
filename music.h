﻿
#ifndef MUSIC_H
#define MUSIC_H
#include "Decode.h"
#include "base.h"
#include <QAudioFormat>
#include <QAudioOutput>
#include <QMainWindow>
#include <QMutex>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
	class Music;
}
QT_END_NAMESPACE

enum status { STATE_VOLUME };
class Base;
class lyric;
class Local_and_Download;
using namespace AudioPlayer;





class Music : public QMainWindow {
	Q_OBJECT
private:
	Ui::Music* ui;
	unsigned int CurrVolume{};
	QString pcmdir{};
	bool sliderSeeking = false;
	AudioPlayer::Player* player;
	int CurrentPlayerListIndex{};
	Base* base{};
	lyric* lyr{};

	//解码器
	AudioDeCode* Decode;

	QTableWidget* tableWidget{};
	//检查鼠标是否移动
	bool mMoving = false;
	QPoint mLastMousePosition{};
	Local_and_Download* localMusic;

	//stop按钮的状态
	enum  class State {
		/*暂停*/
		pause,
		/*恢复播放*/
		resume,
		/*第一次启动*/
		none
	};
	State state;
	enum class  PlayMode {
		/*顺序播放 */
		Order,
		/*单曲循环 */
		Single,
		/*随机播放 */
		Random
	}; 
	PlayMode Mode;


public:
	explicit Music(QWidget* parent = nullptr);
	virtual ~Music();
	void init();
	void initWidget();
	void initTableWidget();
	//悬停提示
	void HoverTip();
	//播放歌曲
	void setBottomInformation(Mp3tag* tag);
	void PlayerMode();
	void Previous();
	void Next();
protected:
	//重写键盘事件
	// void keyPressEvent(QKeyEvent *event);
	//重写鼠标操作,移动，按下，松开
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void Presskey(QKeyEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void closeEvent(QCloseEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);

private slots:
	void onSeekOk();
	void onDuration(int, int);	
	void setPlayMode(PlayMode& Mode);
	void on_btn_login_clicked();
	void on_btn_pictrue_clicked();
	void on_btn_stop_clicked();
	void on_btn_mode_clicked();
	void on_btn_next_clicked();
	void on_btn_max_clicked();
	void on_btn_prev_clicked();
	void on_btn_volum_clicked();

	void on_Sli_volum_valueChanged(int value);
	//进度条按压处理
	void on_playslider_sliderPressed();
	//进度条释放处理
	void on_playslider_sliderReleased();
};

#endif // MUSIC_H