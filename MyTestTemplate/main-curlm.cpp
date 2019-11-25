#include "Curl.h"
#include "muduo/net/EventLoop.h"
#include <stdio.h>

using namespace muduo::net;
EventLoop *g_loop;

void onData(const char *data, int len)
{
	printf("len %d\n", len);
}
void done(curl::Request* c, int code)
{
	printf("done %p %s %d\n", c, c->getEffectiveUrl(), code);
}
void done2(curl::Request* c, int code)
{
	printf("done %p %s %d\n", c, c->getEffectiveUrl(), code);
}
int main(int argc, char *argv[])
{
	EventLoop loop;
	g_loop = &loop;
	loop.runAfter(10.0, std::bind(&EventLoop::quit, &loop));
	curl::Curl::initialize(curl::Curl::Option::kCURLnossl);
	curl::Curl curl(&loop);

	curl::RequestPtr req = curl.getUrl("http://www.baidu.com");
	req->setDataCallback(onData);
	req->setDoneCallback(done);

	curl::RequestPtr req2 = curl.getUrl("http://www.sina.com");
	req2->setDataCallback(onData);
	req2->setDoneCallback(done2);

	loop.loop();
}
