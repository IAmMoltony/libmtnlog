#include "mtnlog.h"
#include "mtnlogversion.h"

static void _someFunction(void)
{
    /* context logging text in another function */
    mtnlogMessageC(MTNLOG_INFO, "Message with context in another function");
    mtnlogMessageC(MTNLOG_INFO, "Ctx message formatting test: %x", 0xdeadbeef);
}

int main(void)
{
    /* initialize log */
    mtnlogInit(MTNLOG_WARNING, "test.log");

    /* simple test */
    mtnlogMessage(MTNLOG_INFO, "Information");
    mtnlogMessage(MTNLOG_WARNING, "Warning!");
    mtnlogMessage(MTNLOG_ERROR, "ERROR!!!");

    mtnlogSetLevel(MTNLOG_INFO);

    /* set color */
    if (mtnlogCheckColor())
        mtnlogColor(true);
    else
    {
        mtnlogColor(false);
        mtnlogMessage(MTNLOG_WARNING, "Terminal doesn't seem to support color :(");
    }
    
    /* color test */
    mtnlogMessage(MTNLOG_INFO, "Color test: info");
    mtnlogMessage(MTNLOG_ERROR, "Color test: error");
    mtnlogMessage(MTNLOG_WARNING, "Color test: warning");

    /* console output enable/disable test */
    mtnlogConsoleOutput(false);
    mtnlogMessage(MTNLOG_INFO, "Console output has been disabled!");
    mtnlogConsoleOutput(true);
    mtnlogMessage(MTNLOG_INFO, "Cosole output has been enabled!");

    /* tag test */
    mtnlogMessageTag(MTNLOG_INFO, "THIS IS A TAG", "Message with tag");

    /* context logging test */
    mtnlogMessageC(MTNLOG_INFO, "Message with context");
    _someFunction();

    /* timestamp enable/disable test */
    mtnlogTimestamps(false);
    mtnlogMessage(MTNLOG_INFO, "Timestamps have been disabled");
    mtnlogTimestamps(true);
    mtnlogMessage(MTNLOG_INFO, "Timestamps have been enabled");
}
