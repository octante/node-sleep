
#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif // BUILDING_NODE_EXTENSION

#include <v8.h>
#include <node.h>

#ifdef WIN32
unsigned int sleep(unsigned int seconds)
{
    Sleep(seconds * 1000);
    return 0;
}
int usleep(unsigned __int64 usec)
{
    LARGE_INTEGER li;
    li.QuadPart = -10 * usec;     // negative values for relative time

    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if(!timer) return -1;

    SetWaitableTimer(timer, &li, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return 0;
}
#else
#   include <unistd.h>
#endif //WIN32


using namespace v8;
using namespace node;


Handle<Value> Sleep(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1 || !args[0]->IsUint32()) {
    return ThrowException(Exception::TypeError(String::New("Expected number of seconds")));
  }

  sleep(args[0]->Uint32Value());

  return scope.Close(Undefined());
}

Handle<Value> USleep(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1 || !args[0]->IsUint32()) {
    return ThrowException(Exception::TypeError(String::New("Expected number of micro")));
  }

  usleep(args[0]->Uint32Value());

  return scope.Close(Undefined());
}


extern "C" void init(Handle<Object> target) {
  NODE_SET_METHOD(target, "sleep", Sleep);
  NODE_SET_METHOD(target, "usleep", USleep);
}


#ifdef NODE_MODULE
NODE_MODULE(node_sleep, init)
#endif

