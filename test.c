#include "mtnlog.h"
#include "mtnlogversion.h"

int main(void)
{
    mtnlogInit(LOG_WARNING, "test.log");
    mtnlogMessage(LOG_INFO, "Hi world, I'm using MtnLog version %d.%d.%d!", MTNLOG_MAJOR, MTNLOG_MINOR, MTNLOG_PATCH);
    mtnlogMessage(LOG_INFO, "Information");
    mtnlogMessage(LOG_WARNING, "Warning!");
    mtnlogMessage(LOG_ERROR, "ERROR!!!");
    mtnlogSetLevel(LOG_INFO);
    mtnlogColor(true);
    mtnlogMessage(LOG_INFO, "Color test: info");
    mtnlogMessage(LOG_ERROR, "Color test: error");
    mtnlogMessage(LOG_WARNING, "Color test: warning");
    mtnlogConsoleOutput(false);
    mtnlogMessage(LOG_INFO, "Console output has been disabled!");
    mtnlogConsoleOutput(true);
    mtnlogMessage(LOG_INFO, "Cosole output has been enabled!");
}
