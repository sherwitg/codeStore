#include"Widget.h"
#ifdef _MSC_VER
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
using namespace std;

int main()
{
	Widget widget(800, 600);
	widget.init();
	widget.run();
	widget.close();
	return 0;
}