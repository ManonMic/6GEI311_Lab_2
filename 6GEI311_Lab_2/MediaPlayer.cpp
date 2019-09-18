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

bool MediaPlayer::ReadKey(char *c)
{
	bool valid = false;
	if (_kbhit())
	{
		*c = toupper(_getch());
		switch (*c)
		{
			case 'P':
				if (state == pause)
				{
					PlayVideo();
				}
				else
				{
					PauseVideo();
				}
				valid = true;
				break;
			case 'A':
				FastForwardVideo();
				valid = true;
				break;
			case 'R':
				RestartVideo();
				valid = true;
				break;
			case 'Q':
				valid = true;
				break;
		}
	}
	return valid;
}

HRESULT MediaPlayer::PlayVideo()
{
	HRESULT hresult = Control->Run();
	if (SUCCEEDED(hresult))
	{
		state = play;
		long evCode;
		Event->WaitForCompletion(1, &evCode);
	}
	else
	{
		printf("ERROR - Could not play the video.");
	}
	return hresult;
}

HRESULT MediaPlayer::PauseVideo()
{
	HRESULT hresult = Control->Pause();
	if (SUCCEEDED(hresult))
	{
		state = pause;
	}
	else
	{
		printf("ERROR - Could not pause the video.");
	}
	return hresult;
}

HRESULT MediaPlayer::FastForwardVideo()
{
	double rate;
	HRESULT hresult = NULL;
	Seek->GetRate(&rate);
	if (rate == 1.0)
	{
		hresult = Seek->SetRate(2.0);
		if (FAILED(hresult))
		{
			printf("ERROR - Could not set rate to 2.0.");
		}
	}
	else
	{
		hresult = Seek->SetRate(1.0);
		if (FAILED(hresult))
		{
			printf("ERROR - Could not set rate to 1.0.");
		}
	}
	return hresult;
}

HRESULT MediaPlayer::RestartVideo()
{
	REFERENCE_TIME start = 0;
	HRESULT hresult = NULL;
	hresult = Seek->SetPositions(
		&start, AM_SEEKING_AbsolutePositioning,
		NULL, AM_SEEKING_NoPositioning
	);
	if (FAILED(hresult))
	{
		printf("ERROR - Could not set positions.");
	}
	if (state == pause)
	{
		PauseVideo();
	}
	else
	{
		PlayVideo();
	}
	return hresult;
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
	PauseVideo();
}

MediaPlayer::~MediaPlayer() 
{
	delete Graph;
	delete Control;
	delete Event;
	delete Seek;
}

static PyObject* player(PyObject* self, PyObject* args)
{
	MediaPlayer *player = new MediaPlayer();

	char key = ' ';

	while (key != 'Q')
	{
		while (!player->ReadKey(&key)) {}
	}
	std::cout << "End of program." << std::endl;
	PyObject * python_val = Py_BuildValue("");
	return python_val;
}

static PyMethodDef methods[] = {
	   { "player", player, METH_NOARGS,
	   "Play a video" },
	   { NULL, NULL, 0, NULL }
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
