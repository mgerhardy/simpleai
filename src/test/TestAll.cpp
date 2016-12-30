#include "TestShared.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>

#if (defined(linux) || defined(__linux) || defined(__linux__))
#undef __LINUX__
#define __LINUX__ 1
#endif

class LocalEnv: public ::testing::Environment {
public:
	virtual ~LocalEnv() {
	}
	virtual void SetUp() override {
	}
	virtual void TearDown() override {
	}
};

#if __LINUX__
// Shamelessly ripped off of SDL
static char * readSymLink(const char *path) {
	char *retval = nullptr;
	ssize_t len = 64;
	ssize_t rc = -1;

	for(;;) {
		char *ptr = (char *) realloc(retval, (size_t) len);
		if (ptr == nullptr) {
			break;
		}

		retval = ptr;

		rc = readlink(path, retval, len);
		if (rc == -1) {
			break;
		} else if (rc < len) {
			retval[rc] = '\0';
			return retval;
		}

		len *= 2;
	}

	free(retval);
	return nullptr;
}
#endif

static void changeCWD() {
#if __LINUX__
	if (access("/proc", F_OK) == 0) {
		char *self = readSymLink("/proc/self/exe");
		if (self != nullptr) {
			char *ptr = strrchr(self, '/') + 1;
			*ptr = '\0';
			chdir(self);
			free(self);
		}
	}
#endif
}

extern "C" int main (int argc, char **argv)
{
	changeCWD();
	::testing::AddGlobalTestEnvironment(new LocalEnv);
	//::testing::GTEST_FLAG(throw_on_failure) = true;
	::testing::InitGoogleTest(&argc, argv);

#if AI_EXCEPTIONS
	try {
#endif
		return RUN_ALL_TESTS();

#if AI_EXCEPTIONS
	} catch (const std::exception& e) {
		ai_log_error("%s", e.what());
		return EXIT_FAILURE;
	}
#endif
}
