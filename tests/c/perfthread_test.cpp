#include "csound.hpp"
#include "csPerfThread.hpp"
#include <stdio.h>
#include <CUnit/Basic.h>

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

void test_perfthread(void)
{
    const char  *instrument =
            "instr 1 \n"
            "k1 expon p4, p3, p4*0.001 \n"
            "a1 randi  k1, p5   \n"
            "out  a1   \n"
            "endin \n";

    Csound csound;
    csound.SetOption((char*)"-odac");
    csound.CompileOrc(instrument);
    csound.ReadScore((char*)"i 1 0  3 10000 5000\n");
    csound.Start();
    CsoundPerformanceThread performanceThread1(csound.GetCsound());
    performanceThread1.Play();
    performanceThread1.Join();
    csound.Cleanup();
    csound.Reset();
    CsoundPerformanceThread performanceThread2(csound.GetCsound());
    csound.SetOption((char*)"-odac");
    csound.CompileOrc(instrument);
    csound.ReadScore((char*)"i 1 0  3 10000 5000\n");
    csound.Start();
    performanceThread2.Play();
    performanceThread2.Join();
    csound.Cleanup();
    csound.Reset();
}


int main()
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("perfthread tests", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "Test Performance Thread", test_perfthread))
//            || (NULL == CU_add_test(pSuite, "Test reuse", test_reuse))
        )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "Test Performance Thread second run", test_perfthread))
//            || (NULL == CU_add_test(pSuite, "Test reuse", test_reuse))
        )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

