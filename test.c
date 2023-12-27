#include "mtnlog.h"
#include "mtnlogversion.h"

static void _someFunction(void)
{
    mtnlogMessageC(LOG_INFO, "Message with context in another function");
    mtnlogMessageC(LOG_INFO, "Ctx message formatting test: %x", 0xdeadbeef);
}

int main(void)
{
    // initialize log
    mtnlogInit(LOG_WARNING, "test.log");

    // simple test
    mtnlogMessage(LOG_INFO, "Information");
    mtnlogMessage(LOG_WARNING, "Warning!");
    mtnlogMessage(LOG_ERROR, "ERROR!!!");

    mtnlogSetLevel(LOG_INFO);
    mtnlogColor(true);
    
    // color test
    mtnlogMessage(LOG_INFO, "Color test: info");
    mtnlogMessage(LOG_ERROR, "Color test: error");
    mtnlogMessage(LOG_WARNING, "Color test: warning");

    // console output enable/disable test
    mtnlogConsoleOutput(false);
    mtnlogMessage(LOG_INFO, "Console output has been disabled!");
    mtnlogConsoleOutput(true);
    mtnlogMessage(LOG_INFO, "Cosole output has been enabled!");

    // tag test
    mtnlogMessageTag(LOG_INFO, "THIS IS A TAG", "Message with tag");

    // context logging test
    mtnlogMessageC(LOG_INFO, "Message with context");
    _someFunction();
}
