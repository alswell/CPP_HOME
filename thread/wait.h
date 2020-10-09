#pragma once

#define SYNC_WAIT(sync_obj, _lock, _timedlock, nTimeout) \
	if (nTimeout == -1)\
	{\
		return _lock(&sync_obj);\
	}\
	else\
	{\
		struct timespec timeout;\
		timeout.tv_sec = nTimeout / 1000;\
		timeout.tv_nsec = nTimeout % 1000 * 1000000;\
		return _timedlock(&sync_obj, &timeout);\
	}

