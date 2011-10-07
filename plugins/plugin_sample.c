#include <log.h>

#include <stdio.h>

const char * plugin_name = "Plugin Sample";

void plugin_main(void *arg)
{
	say_debug("I am %s\n", plugin_name);
	return;
}
