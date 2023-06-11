/****************************************\
 pl32lib-ng, v1.05
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32.hpp: Base API header (C++ Bindings)
\****************************************/

#define PL32LIBNG_VERSION "1.05"
#define PL32LIBNG_API_VER 1
#define PL32LIBNG_FEATURELVL 5
#define PL32LIBNG_PATCHLVL 0

enum plmtaction {
	PLMT_GET_USEDMEM = 6,
	PLMT_GET_MAXMEM = 7,
	PLMT_SET_MAXMEM = 8,
};


namespace pl32 {
	using plmtaction_t = enum plmtaction;
	using byte_t = uint8_t;
	using string_t = char*;
	using memptr_t = void*;

	struct plmt;
	struct plfatptr {
		memptr_t array;
		size_t size;
		bool isMemAlloc;
		plmt_t* mt;
	};
	struct plchar {
		byte_t bytes[5];
	};

	using plchar_t = struct plchar;
	using plfatptr_t = struct plfatptr;
	using plarray_t = struct plfatptr;

	struct plstring {
		plarray_t data;
		bool isplChar;
	}

	using plstring_t = struct plstring;
}
