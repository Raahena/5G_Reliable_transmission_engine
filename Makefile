
CC = gcc

CFLAGS = -std=c11 \
	-Wall -Wextra -Wpedantic -Wshadow \
	-Wconversion -Wsign-conversion -Wcast-qual \
	-Wstrict-prototypes -Wmissing-prototypes \
	-Wwrite-strings -Wundef -Wpointer-arith \
	-Wswitch-default -Wswitch-enum \
	-Wuninitialized -Wmissing-declarations

TARGET = gbn
TEST_TARGET = gbn_tests
CUNIT_TARGET = gbn_cunit

SRC_FILES = \
	ack.c \
	adaptivewindow.c \
	buffer.c \
	channel.c \
	config.c \
	logging.c \
	packet.c \
	receiver.c \
	sender.c \
	statistics.c \
	thread.c \
	throughput.c \
	timer.c \
	window.c

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) \
		main.c $(SRC_FILES) \
		-o $(TARGET) -lpthread

run: $(TARGET)
	./$(TARGET)

test:
	$(CC) $(CFLAGS) \
		tests/test_main.c \
		tests/test_ack.c \
		tests/test_adaptivewindow.c \
		tests/test_buffer.c \
		tests/test_channel.c \
		tests/test_integration.c \
		tests/test_logging.c \
		tests/test_packet.c \
		tests/test_receiver.c \
		tests/test_sender.c \
		tests/test_statistics.c \
		tests/test_throughput.c \
		tests/test_timer.c \
		tests/test_window.c \
		$(SRC_FILES) \
		-o $(TEST_TARGET) -lpthread

run-test: test
	./$(TEST_TARGET)

cunit:
	$(CC) $(CFLAGS) \
		cunit_testing_files/cunit_all_tests.c \
		$(SRC_FILES) \
		-o $(CUNIT_TARGET) \
		-lcunit \
		-lpthread

run-cunit: cunit
	./$(CUNIT_TARGET)

misra:
	$(CC) $(CFLAGS) \
		main.c $(SRC_FILES) \
		-o gbn_misra -lpthread \
		2> misra_report.txt

cppcheck:
	cppcheck \
		--enable=all \
		--std=c11 \
		--suppress=missingIncludeSystem \
		. 2> cppcheck_report.txt

valgrind: $(TARGET)
	valgrind \
		--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		./$(TARGET) \
		2> valgrind_report.txt

gcov-build:
	$(CC) $(CFLAGS) \
		-fprofile-arcs -ftest-coverage \
		main.c $(SRC_FILES) \
		-o gbn_cov -lpthread

gcov-run: gcov-build
	./gbn_cov

gcov-report:
	gcov *.c

coverage: gcov-build gcov-run gcov-report

quality: misra cppcheck valgrind

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TEST_TARGET)
	rm -f $(CUNIT_TARGET)
	rm -f gbn_misra
	rm -f gbn_cov
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f misra_report.txt
	rm -f cppcheck_report.txt
	rm -f valgrind_report.txt

rebuild: clean all

help:
	@echo "========================================"
	@echo "Available Targets"
	@echo "========================================"
	@echo "make - Build Application"
	@echo "make run - Run Application"
	@echo "make test - Build Unit Tests"
	@echo "make run-test - Run Unit Tests"
	@echo "make cunit - Build CUnit Tests"
	@echo "make run-cunit - Run CUnit Tests"
	@echo "make misra - Generate MISRA Report"
	@echo "make cppcheck - Generate Cppcheck Report"
	@echo "make valgrind - Generate Valgrind Report"
	@echo "make gcov-build - Build Coverage Version"
	@echo "make gcov-run - Run Coverage Version"
	@echo "make gcov-report - Generate GCOV Report"
	@echo "make coverage - Full Coverage Analysis"
	@echo "make quality - Run All Quality Checks"
	@echo "make clean - Clean Project"
	@echo "make rebuild - Rebuild Project"
	@echo "========================================"

.PHONY: all run test run-test cunit run-cunit \
	misra cppcheck valgrind quality \
	gcov-build gcov-run gcov-report coverage \
	clean rebuild help

