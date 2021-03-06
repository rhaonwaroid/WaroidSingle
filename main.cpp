/*
 * main.cpp
 *
 *  Created on: 2015. 2. 12.
 *      Author: mirime
 */

#include "common/GRCSoundWorker.h"

#ifdef __RPI__
#include <wiringPi.h>
#endif
#include <csignal>
#include <cstdlib>

#include "core/GRCCore.h"
#include "core/GRCLogger.h"
#include "Manager.h"

bool initialize();
void cleanup(int s);

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		GRC_WARN("usage: %s <robot type name> <dev 0 or 1>", argv[0]);
		return -1;
	}

	GRCLogger::setDev(atoi(argv[2]) == 1);

	GRC_INFO("[Waroid Single] START");

	GRC_CHECK_FUNC_RETMINUS(initialize(), cleanup(0));

	const char* robotTypeName = argv[1];

	Manager::start(robotTypeName);

	cleanup(0);

	GRC_INFO("[Waroid Single] END");

	return 0;
}

bool initialize()
{
	signal(SIGINT, cleanup);
	signal(SIGTERM, cleanup);
	signal(SIGQUIT, cleanup);
	signal(SIGHUP, cleanup);

#ifdef __RPI__
	GRC_CHECK_RETFALSE(wiringPiSetupGpio() != -1);
	GRC_DEV("setup gpio of wiringPi");
#endif

	GRC_CHECK_RETFALSE(GRCSoundWorker::start(SOUND_DATA_DIR));

	GRC_INFO("initlaized.");

	return true;
}

void cleanup(int s)
{
	Manager::stop();

	GRCSoundWorker::stop();

	if (s != 0)
	{
		GRC_ERR("caught signal %d", s);
		exit(-1);
	}
}
