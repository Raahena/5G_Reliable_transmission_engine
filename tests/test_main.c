#include "test.h"

/*----------------------------------------------------------
    Global Counters
-----------------------------------------------------------*/
int totalTests = 0;
int passedTests = 0;

/*----------------------------------------------------------
    Run All Unit Tests
-----------------------------------------------------------*/
void runAllUnitTests(void)
{
    printf("\n=============================================\n");
    printf("         GO-BACK-N ARQ UNIT TESTS\n");
    printf("=============================================\n");

    printf("\n[PACKET MODULE]\n");
    runPacketTests();

    printf("\n[BUFFER MODULE]\n");
    runBufferTests();

    printf("\n[WINDOW MODULE]\n");
    runWindowTests();

    printf("\n[SENDER MODULE]\n");
    runSenderTests();

    printf("\n[RECEIVER MODULE]\n");
    runReceiverTests();

    printf("\n[ACK MODULE]\n");
    runACKTests();

    printf("\n[TIMER MODULE]\n");
    runTimerTests();

    printf("\n[CHANNEL MODULE]\n");
    runChannelTests();

    printf("\n[ADAPTIVE WINDOW MODULE]\n");
    runAdaptiveWindowTests();

    printf("\n[STATISTICS MODULE]\n");
    runStatisticsTests();

    printf("\n[THROUGHPUT MODULE]\n");
    runThroughputTests();

    printf("\n[LOGGING MODULE]\n");
    runLoggingTests();
}

/*----------------------------------------------------------
    Test Summary
-----------------------------------------------------------*/
void printSummary(void)
{
    printf("\n=============================================\n");
    printf("              TEST SUMMARY\n");
    printf("=============================================\n");

    printf("Total Tests : %d\n", totalTests);
    printf("Passed      : %d\n", passedTests);
    printf("Failed      : %d\n", totalTests - passedTests);

    printf("Success Rate: %.2f%%\n",
           (totalTests == 0) ? 0.0f :
           ((float)passedTests / totalTests) * 100.0f);

    if(totalTests == passedTests)
    {
        printf("\nALL TESTS PASSED SUCCESSFULLY\n");
    }
    else
    {
        printf("\nSOME TESTS FAILED\n");
    }

    printf("=============================================\n");
}

/*----------------------------------------------------------
    Main
-----------------------------------------------------------*/
int main(void)
{
    printf("\n");
    printf("=============================================\n");
    printf("        GO-BACK-N ARQ TEST FRAMEWORK\n");
    printf("=============================================\n");

    runAllUnitTests();

    runAllIntegrationTests();

    printSummary();

    return (passedTests == totalTests) ? 0 : 1;
}