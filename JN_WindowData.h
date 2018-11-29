#ifndef JN_WINDOW_SIZE_H
#define JN_WINDOW_SIZE_H

// This will be passed to the user so the user can also be resized and moved
struct JN_WindowData
{
public:
	JN_WindowData()
	{

	}

	JN_WindowData(int x, int y, int w, int h)
	{
		xOffset = x;
		yOffset = y;
		windowWidth = w;
		windowHeight = h;
	}

	int windowWidth;
	int windowHeight;
	int xOffset;
	int yOffset;
};

#endif // !JN_WINDOW_SIZE_H
