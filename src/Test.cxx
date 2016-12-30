#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sysexits.h>
#include "lib/backtrace.h"
#include "lib/thread.h"
#include "lib/frame.h"
#include "lib/strbuf.h"
#include "lib/utils.h"
#include "lib/location.h"


using namespace std;


char *read_file(char *filename) {
  return btp_file_to_string(filename);
}

struct btp_backtrace *parse_backtrace(char *filename) {
  char *text = read_file(filename);
  const char *ptr = text;
  struct btp_location location;
  btp_location_init(&location);
  struct btp_backtrace *backtrace = btp_backtrace_parse(&ptr, &location);
  if (!backtrace) {
    char *location_str = btp_location_to_string(&location);
    cerr << text << endl;
    cerr << "Failed to parse the backtrace. " << location_str << endl;
    free(location_str);
    exit(-1);
  }
  free(text);
  return backtrace;
}

struct btp_thread *get_optimized_thread(struct btp_backtrace *backtrace, int max_frames) {
  struct btp_thread *thread = btp_backtrace_get_optimized_thread(backtrace, max_frames);
  if (!thread) {
    cerr << "Failed to find the crash thread." << endl;
    exit(-1);
  }
  return thread;
}

struct btp_frame *walk_frames(struct btp_frame *frm) {
  if (frm) {
    cout << frm->function_name << endl;
    return walk_frames(frm->next);
  }
  else {
    return NULL;
  }
}

struct btp_thread *walk_thread(struct btp_thread* thr) {
  if (thr) {
    walk_frames(thr->frames);
    return walk_thread(thr->next);
  }
  else {
    return NULL;
  }
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
    char* filename = argv[1];
    struct btp_backtrace *backtrace = parse_backtrace(filename);
    cout << backtrace->threads->number << endl;
    std::size_t maxThreads = backtrace->threads->number;
    walk_thread(backtrace->threads);
    //struct btp_thread *crash_thread = get_optimized_thread(backtrace, 4096);
    //char *funs = btp_thread_format_funs(crash_thread);
    //printf("%s", funs);
    //free(funs);
    //btp_thread_free(crash_thread);
  }
  return 0;
}
  
