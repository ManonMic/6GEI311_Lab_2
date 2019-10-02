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
	States GetState();
	void ChangeState(States state);
	IGraphBuilder *Graph;
	IMediaControl *Control;
	IMediaEvent *Event;
	IMediaSeeking *Seek;
	static PyObject* play_video(PyObject* self, PyObject* args);
	static PyObject* fastforward_video(PyObject* self, PyObject* args);
	static PyObject* restart_video(PyObject* self, PyObject* args);
};

