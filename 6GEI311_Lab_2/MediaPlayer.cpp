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
		std::cout << "ERROR - Could not render the file." << std::endl;
		return hresult;
	}

	hresult = SetControl();
	if (FAILED(hresult))
	{
		std::cout << "ERROR - Could not query the control." << std::endl;
		return hresult;
	}

	hresult = SetEvent();
	if (FAILED(hresult))
	{
		std::cout << "ERROR - Could not query the event." << std::endl;
		return hresult;
	}

	hresult = SetSeek();
	if (FAILED(hresult))
	{
		std::cout << "ERROR - Could not query the seek." << std::endl;
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

MediaPlayer::States MediaPlayer::GetState()
{
	int state = static_cast<int>(this->state);
	if (state == 0 || state == 3)
	{
		return States::play;
	}
	else if (state == 1 || state == 4)
	{
		return States::pause;
	}
}

void MediaPlayer::ChangeState(States state)
{
	this->state = state;
}

PyObject* MediaPlayer::play_video(PyObject* self, PyObject* args)
{
	MediaPlayer *mediaPlayer;
	PyArg_ParseTuple(args, "O", &mediaPlayer);

	HRESULT hresult = NULL;
	States state = mediaPlayer->GetState();
	if (state == States::pause)
	{
		hresult = mediaPlayer->Control->Run();
		if (SUCCEEDED(hresult))
		{
			std::cout << "play_video before: " << static_cast<int>(state) << std::endl;
			mediaPlayer->state = States::play;
			std::cout << "play_video after: " << static_cast<int>(state) << std::endl;
			long evCode;
			mediaPlayer->Event->WaitForCompletion(1, &evCode);
		}
	}
	else if (state == States::play)
	{
		hresult = mediaPlayer->Control->Pause();
		if (SUCCEEDED(hresult))
		{
			std::cout << "play_video state before: " << static_cast<int>(state) << std::endl;
			mediaPlayer->state = States::pause;
			std::cout << "play_video state after: " << static_cast<int>(state) << std::endl;
		}
	}

	PyObject *pythonVal = Py_BuildValue("i", hresult);
	std::cout << "obj state val: " << static_cast<int>(mediaPlayer->state) << std::endl;
	return pythonVal;
}

PyObject* MediaPlayer::fastforward_video(PyObject* self, PyObject* args)
{
	MediaPlayer *mediaPlayer;
	PyArg_ParseTuple(args, "O", &mediaPlayer);
	double rate;
	HRESULT hresult = NULL;
	mediaPlayer->Seek->GetRate(&rate);
	if (rate == 1.0)
	{
		hresult = mediaPlayer->Seek->SetRate(2.0);
		if (FAILED(hresult))
		{
			std::cout << "ERROR - Could not set rate to 2.0." << std::endl;
		}
	}
	else
	{
		hresult = mediaPlayer->Seek->SetRate(1.0);
		if (FAILED(hresult))
		{
			std::cout << "ERROR - Could not set rate to 1.0." << std::endl;
		}
	}
	PyObject *pythonVal = Py_BuildValue("i", hresult);
	return pythonVal;
}

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
	   { "fastforward_video", MediaPlayer::fastforward_video, METH_VARARGS, "Fastforward a video." },
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
