#include <string>
#include <dshow.h>
#include <iostream>
#include <Python.h>
#include <utility>

class MediaPlayer
{
private:
	HRESULT InitCOMLib();
	HRESULT RenderFile(LPCWSTR filePath);
	HRESULT SetControl();
	HRESULT SetEvent();
	HRESULT SetSeek();
	HRESULT InitFilterGraphManager();
	HRESULT BuildGraph();
public:
	MediaPlayer();
	~MediaPlayer();
	enum class States {
		play,
		pause
	};
	States state;
	IGraphBuilder *Graph;
	IMediaControl *Control;
	IMediaEvent *Event;
	IMediaSeeking *Seek;
	void PlayPauseVideo();
	void FastForwardVideo();
	void RewindVideo();
};

