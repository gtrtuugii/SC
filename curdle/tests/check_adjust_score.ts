//////////////////////////
// DO NOT SUBMIT THIS FILE
//////////////////////////

#include "adjust_score.h"
#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// useful if you want to coredump
// yourself -- see `man raise`
//#include <signal.h>

// used for lseek
#include <unistd.h>
#include <sys/types.h>

// an (unshared) "fixture" -- a mock file containing 2 records,
// with some specified "filename"
int mk_two_record_file(const char * filename) {
    const size_t FILE_SIZE = REC_SIZE*2;
    char file_conts[FILE_SIZE];
    memset(file_conts, 0, FILE_SIZE);

    const char *names[]   = { "doug", "eve" };
    const char *scores[]  = { "64",   "73" };

    size_t num_recs = sizeof(scores)/sizeof(scores[0]);

    for(size_t i=0; i < num_recs; i++) {
      size_t start_pos = i * REC_SIZE;

      // names
      strncpy(file_conts + start_pos, names[i], FIELD_SIZE);
      (file_conts + start_pos)[FIELD_SIZE-1] = '\0';

      // scores
      strncpy(file_conts + start_pos + FIELD_SIZE, scores[i], FIELD_SIZE);
      (file_conts + start_pos + FIELD_SIZE)[FIELD_SIZE-1] = '\0';
    }

    int fd = mock_file(filename, file_conts, FILE_SIZE);
    return fd;
}


#suite adjust_score_tests

#tcase arithmetic_testcase

#test arithmetic_works
    int m = 3;
    int n = 4;
    int expected = 7;
    int actual = m + n;
    ck_assert_int_eq(actual, expected);

//////////////////////////////////////////////////
// Uncomment the following to see AddressSanitizer
// in action:
//////////////////////////////////////////////////

//#tcase deliberately_bad_testcase
//
//#test go_out_of_bounds
//    char buf[]            = "supercallifragilistic";
//    const size_t buf_size = sizeof(buf)/sizeof(buf[0]);
//    buf[buf_size-1]       = 'X';
//    size_t len            = strlen(buf);
//    // we expect the strlen-calculated size
//    // to be greater than 0.
//    ck_assert_uint_gt(len, 0);

#tcase filesize_works_testcase

#test filesize_small_works
    const size_t expected_size  = 100;
    void * conts_buf            = random_contents(expected_size);
    int fd                      = mock_file("mock", conts_buf, expected_size); 
    free(conts_buf);
    size_t actual_size          = file_size("mock", fd);
    ck_assert_uint_eq(actual_size, expected_size);

#test filesize_medium_works
    const size_t expected_size  = 500;
    void * conts_buf            = random_contents(expected_size);
    int fd                      = mock_file("mock", conts_buf, expected_size); 
    free(conts_buf);
    size_t actual_size          = file_size("mock", fd);
    ck_assert_uint_eq(actual_size, expected_size);

#test filesize_large_works
    const size_t expected_size  = 4096;
    void * conts_buf            = random_contents(expected_size);
    int fd                      = mock_file("mock", conts_buf, expected_size);
    free(conts_buf);
    size_t actual_size          = file_size("mock", fd);
    ck_assert_uint_eq(actual_size, expected_size);

///////////////////////////////////////////////////////
// Uncomment the following if you have a parse_record()
// implementation to test:
///////////////////////////////////////////////////////

//#tcase parse_record_testcase
//
//#test parse_record_works
//    char rec_buf[REC_SIZE];
//    memset(rec_buf, 0, REC_SIZE);
//    const char *expected_name = "bob";
//    // set name
//    strncpy(rec_buf, expected_name, FIELD_SIZE);
//    // set score
//    strncpy(rec_buf + FIELD_SIZE, "42", FIELD_SIZE);
//
//    struct score_record myrec = parse_record(rec_buf);
//    ck_assert_str_eq(myrec.name, expected_name);
//    ck_assert_int_eq(myrec.score, 42);


///////////////////////////////////////////////////////
// Uncomment the following if you have a store_record()
// implementation to test:
///////////////////////////////////////////////////////

//#tcase store_record_testcase
//
//#test store_record_works
//    const char *expected_name = "alice";
//    int expected_score = 999;
//
//    struct score_record rec;
//    score_record_init(&rec, expected_name, expected_score);
//    char rec_buf[REC_SIZE];
//    store_record(rec_buf, &rec);
//
//    ck_assert_str_eq(rec_buf, expected_name);
//    ck_assert_str_eq(rec_buf + FIELD_SIZE, "999");

//////////////////////////////////////////////////////
// Uncomment the following if you have a find_record()
// implementation to test.
//
// Note that a *full* set of "find_record" tests would include the
// situations where:
//  - the record being searched for isn't in the file at all
//  - it is, and is at the start
//  - it is, and is at the end (the `find_record_works` test)
//  - it is, and is somewhere in the middle
//
// Here, only one is provided: you should write additional tests
// to cover the other scenarios.
//////////////////////////////////////////////////////

//#tcase find_record_testcase
//
//#test find_record_works
//    int fd = mk_two_record_file("find_record_file");
//    off_t res = find_record("mock", fd, "eve");
//    // we expect to find eve 1 record into
//    // the file
//    off_t expected_res = REC_SIZE;
//    ck_assert_int_eq(res, expected_res);

////////////////////////////////////////////////////////////
// Uncomment the following if you have a adjust_score_file()
// implementation to test:
////////////////////////////////////////////////////////////

//#tcase increment_score_testcase
//
//// Test whether we can we amend an existing score
//#test increment_score_overwrites
//    int fd = mk_two_record_file("find_record_file");
//    adjust_score_file("find_record_file", fd, "eve", 13);
//
//    // read whole file into an array for easy
//    // testing
//    lseek(fd, 0, SEEK_SET);
//    char incremented_conts[REC_SIZE*2];
//    ssize_t res = read(fd, incremented_conts, REC_SIZE*2);
//
//    // old score was 73
//    // new should be 86
//    const char * expected_score = "86";
//    ck_assert_str_eq(incremented_conts + REC_SIZE + FIELD_SIZE, expected_score);
//
//
//// Test whether we can append a new record, if no
//// existing record is found
//#test increment_score_appends
//    int fd = mk_two_record_file("find_record_file");
//    // search for a player not in the file
//    const char *expected_name = "carlos";
//    adjust_score_file("find_record_file", fd, expected_name, 13);
//
//    size_t actual_size = file_size("find_record_file", fd);
//    // new file size should be 3 records
//    size_t expected_size = REC_SIZE * 3;
//
//    ck_assert_uint_eq(actual_size, expected_size);
//
//    // read whole file into an array for easy
//    // testing
//    lseek(fd, 0, SEEK_SET);
//    char appended_conts[expected_size];
//    ssize_t res = read(fd, appended_conts, expected_size);
//
//    // carlos is at 3rd record
//    ck_assert_str_eq(appended_conts + (REC_SIZE*2), expected_name);
//    const char *expected_score = "13";
//    ck_assert_str_eq(appended_conts + (REC_SIZE*2) + FIELD_SIZE, expected_score);


// vim: syntax=c :
