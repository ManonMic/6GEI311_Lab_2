#include <string>
#include <dshow.h>
#include <iostream>
#include <Python.h>

class MediaPlayer
{
private:
	IGraphBuilder *Graph;
	IMediaControl *Control;
	IMediaEvent *Event;
	IMediaSeeking *Seek;
	enum STATE {
		play,
		pause
	};

	HRESULT InitCOMLib();
	HRESULT RenderFile(LPCWSTR filePath);
	HRESULT SetControl();
	HRESULT SetEvent();
	HRESULT SetSeek();
	HRESULT InitFilterGraphManager();
	HRESULT BuildGraph();
	HRESULT PlayVideo();
	HRESULT PauseVideo();
	HRESULT FastForwardVideo();
	HRESULT RestartVideo();
public:
	MediaPlayer();
	~MediaPlayer();
	bool ReadKey(char *);
	STATE state;
};

