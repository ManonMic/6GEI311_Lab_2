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

void MediaPlayer::PlayPauseVideo()
{
	HRESULT hresult = NULL;
	if (this->state == States::pause)
	{
		hresult = this->Control->Run();
		if (SUCCEEDED(hresult))
		{
			state = States::play;
			long evCode;
			this->Event->WaitForCompletion(1, &evCode);
		}
		else
		{
			std::cout << "ERROR - Could not play the video." << std::endl;
		}
	}
	else if (this->state == States::play)
	{
		hresult = this->Control->Pause();
		if (SUCCEEDED(hresult))
		{
			this->state = States::pause;
		}
		else
		{
			std::cout << "ERROR - Could not pause the video." << std::endl;
		}
	}
}

void MediaPlayer::FastForwardVideo()
{
	double rate;
	HRESULT hresult = NULL;
	hresult = this->Seek->GetRate(&rate);
	if (SUCCEEDED(hresult))
	{
		if (rate == 1.0)
		{
			hresult = this->Seek->SetRate(2.0);
			if (FAILED(hresult))
			{
				std::cout << "ERROR - Could not set rate to 2.0." << std::endl;
			}
		}
		else
		{
			hresult = this->Seek->SetRate(1.0);
			if (FAILED(hresult))
			{
				std::cout << "ERROR - Could not set rate to 1.0." << std::endl;
			}
		}
	}
	else
	{
		std::cout << "ERROR - Could get the rate of the video." << std::endl;
	}
}

void MediaPlayer::RewindVideo()
{
	REFERENCE_TIME start = 0;
	HRESULT hresult = NULL;
	hresult = this->Seek->SetPositions(
		&start, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning
	);

	if (FAILED(hresult))
	{
		printf("ERROR - Could not set position.");
	}
}

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

static PyObject* vp_module(PyObject* self, PyObject* args)
{
	static MediaPlayer *vp = new MediaPlayer();

	char *choice;
	PyArg_ParseTuple(args, "z", &choice);

	if(choice[0] == 'P')
	{
		vp->PlayPauseVideo();
	}
	else if (choice[0] == 'A')
	{
		vp->FastForwardVideo();
	}
	else if (choice[0] == 'R')
	{
		vp->RewindVideo();
	}
	PyObject *pythonVal = Py_BuildValue("");
	return pythonVal;
}

static PyMethodDef methods[] = {
	   { "vp_module", vp_module, METH_VARARGS, "VideoPlayer module" },
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
