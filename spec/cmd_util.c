#include <stdio.h>
#include "spec_helper.h"
#include "cmd_util.h"

/*** Types ***/

struct orig_env {
  char *home, *xdg_config_home, *punch_home;
};

/***  helper prototypes  ***/

struct orig_env stub_env(char *home, char *xdg_config_home, char* punch_home);
void restore_env(struct orig_env env);
void set_env_val(char *name, char *val);

/***  Tests  ***/

// default -- no env set
void spec_test_get_log_file_path_1(void) {
  struct orig_env old_env = stub_env("/home/abbynormal", NULL, NULL);

  char buf[FILENAME_MAX];
  get_log_file_path(buf);

  sp_assert_equal_str("/home/abbynormal/.config/punch/entries.log", buf);

  restore_env(old_env);
}

// XDG_CONFIG_HOME set
void spec_test_get_log_file_path_2(void) {
  struct orig_env old_env = stub_env("/home/abbynormal", "/foo/config", NULL);

  char buf[FILENAME_MAX];
  get_log_file_path(buf);

  sp_assert_equal_str("/foo/config/punch/entries.log", buf);

  restore_env(old_env);
}

// PUNCH_HOME set
void spec_test_get_log_file_path_3(void) {
  struct orig_env old_env = stub_env("/home/abbynormal", NULL, "/tmp/punch_home");

  char buf[FILENAME_MAX];
  get_log_file_path(buf);

  sp_assert_equal_str("/tmp/punch_home/entries.log", buf);

  restore_env(old_env);
}

/***  helper implementations  ***/

struct orig_env stub_env(char *home, char *xdg_config_home, char* punch_home) {
  struct orig_env old_vals = {
    .home = getenv("HOME"),
    .xdg_config_home = getenv("XDG_CONFIG_HOME"),
    .punch_home = getenv("PUNCH_HOME"),
  };

  set_env_val("HOME", home);
  set_env_val("XDG_CONFIG_HOME", xdg_config_home);
  set_env_val("PUNCH_HOME", punch_home);

  return old_vals;
}

void restore_env(struct orig_env env) {
  set_env_val("HOME", env.home);
  set_env_val("XDG_CONFIG_HOME", env.xdg_config_home);
  set_env_val("PUNCH_HOME", env.punch_home);
}

void set_env_val(char *name, char *val) {
  if (NULL == val) {
    unsetenv(name);
  } else {
    setenv(name, val, 1);
  }
}

