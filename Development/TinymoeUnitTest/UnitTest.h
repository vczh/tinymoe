#ifndef VCZH_TINYMOE_UNITTEST
#define VCZH_TINYMOE_UNITTEST

#include <iostream>
#include <string>

#define TEST_CASE(NAME)								\
	extern void TEST_CASE_FUNCTION_##NAME(void);	\
	class TEST_CASE_CLASS_##Name					\
	{												\
	public:											\
		TEST_CASE_CLASS_##Name()					\
		{											\
			std::cout << #NAME << std::endl;		\
			TEST_CASE_FUNCTION_##NAME();			\
		}											\
	} TEST_CASE_INSTANCE_##NAME;					\
	void TEST_CASE_FUNCTION_##NAME(void)

#define TEST_ASSERT(CONDITION) do{ if (!(CONDITION)) throw 0; } while (0)
#define TEST_PRINT(MESSAGE) std::cout << (MESSAGE) << std::endl;

#endif