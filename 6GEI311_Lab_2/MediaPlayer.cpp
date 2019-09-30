#include <conio.h>
#include "MediaPlayer.h"

HRESULT MediaPlayer::InitCOMLib()
{
	HRESULT hresult = CoInitialize(NULL);
	if (FAILED(hresult))
	{
		printf("ERROR - Could not initialize COM library.");
	}
	return hresult;
}

HRESULT MediaPlayer::InitFilterGraphManager()
{
	HRESULT hresult = CoCreateInstance(
		CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void **)&Graph
	);
	if (FAILED(hresult))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
	}
	return hresult;
}

HRESULT MediaPlayer::BuildGraph()
{
	HRESULT hresult = NULL;
		
	hresult = RenderFile(L"C:\\Example.avi");
	if (FAILED(hresult))
	{
		printf("ERROR - Could not render the file.");
		return hresult;
	}

	hresult = SetControl();
	if (FAILED(hresult))
	{
		printf("ERROR - Could not query the control.");
		return hresult;
	}

	hresult = SetEvent();
	if (FAILED(hresult))
	{
		printf("ERROR - Could not query the event.");
		return hresult;
	}

	hresult = SetSeek();
	if (FAILED(hresult))
	{
		printf("ERROR - Could not query the seek.");
		return hresult;
	}

	return hresult;
}

HRESULT MediaPlayer::RenderFile(LPCWSTR filePath)
{
	return Graph->RenderFile(filePath, NULL);
}

HRESULT MediaPlayer::SetControl()
{
	return Graph->QueryInterface(IID_IMediaControl, (void **)&Control);
}

HRESULT MediaPlayer::SetEvent()
{
	return Graph->QueryInterface(IID_IMediaEvent, (void **)&Event);
}

HRESULT MediaPlayer::SetSeek()
{
	return Graph->QueryInterface(IID_IMediaSeeking, (void**)&Seek);
}

PyObject* MediaPlayer::play_video(PyObject* self, PyObject* args)
{
	MediaPlayer *mediaPlayer;
	PyArg_ParseTuple(args, "O", &mediaPlayer);

	HRESULT hresult = NULL;
	if (mediaPlayer->state == States::pause)
	{
		hresult = mediaPlayer->Control->Run();
		if (SUCCEEDED(hresult))
		{
			mediaPlayer->state = States::play;
			long evCode;
			mediaPlayer->Event->WaitForCompletion(1, &evCode);
		}
	}

	PyObject *pythonVal = Py_BuildValue("i", hresult);
	return pythonVal;
}

//HRESULT MediaPlayer::FastForwardVideo(PyObject* self, PyObject* args)
//{
//	double rate;
//	HRESULT hresult = NULL;
//	Seek->GetRate(&rate);
//	if (rate == 1.0)
//	{
//		hresult = Seek->SetRate(2.0);
//		if (FAILED(hresult))
//		{
//			printf("ERROR - Could not set rate to 2.0.");
//		}
//	}
//	else
//	{
//		hresult = Seek->SetRate(1.0);
//		if (FAILED(hresult))
//		{
//			printf("ERROR - Could not set rate to 1.0.");
//		}
//	}
//	return hresult;
//}
//
//HRESULT MediaPlayer::RestartVideo(PyObject* self, PyObject* args)
//{
//	REFERENCE_TIME start = 0;
//	HRESULT hresult = NULL;
//	hresult = Seek->SetPositions(
//		&start, AM_SEEKING_AbsolutePositioning,
//		NULL, AM_SEEKING_NoPositioning
//	);
//	if (FAILED(hresult))
//	{
//		printf("ERROR - Could not set positions.");
//	}
//	if (state == pause)
//	{
//		PauseVideo();
//	}
//	else
//	{
//		PlayVideo();
//	}
//	return hresult;
//}

MediaPlayer::MediaPlayer()
{
	Graph = NULL;
	Control = NULL;
	Event = NULL;
	Seek = NULL;
	InitCOMLib();
	InitFilterGraphManager();
	BuildGraph();
	state = States::pause;
}

MediaPlayer::~MediaPlayer() 
{
	delete Graph;
	delete Control;
	delete Event;
	delete Seek;
}

static PyObject* init(PyObject* self, PyObject* args)
{
	MediaPlayer *mediaPlayer = new MediaPlayer;

	PyObject *pythonVal = Py_BuildValue("O", mediaPlayer);
	return pythonVal;
}

static PyMethodDef methods[] = {
	   { "init", init, METH_NOARGS, "Initialize a mediaPlayer c++ object on the memory stack." },
	   { "play_video", MediaPlayer::play_video, METH_VARARGS, "Play a video." },
	   { NULL, NULL }
};

static struct PyModuleDef cModPyDem =
{
	PyModuleDef_HEAD_INIT,
	"videoplayer",
	"NULL",
	-1,
	methods
};

PyMODINIT_FUNC PyInit_videoplayer(void)
{
	return PyModule_Create(&cModPyDem);
}
