#ifndef SCENARIO_RUNNERS_H
#define SCENARIO_RUNNERS_H

#include "helper_functions.h"

#include <stdio.h>

FILE *create_file_of_size(const char *filename, BYTES bytes);

void create_files_runner(FILES *amount_arr, size_t arr_size, int measurements);
void remove_files_runner(FILES *amount_arr, size_t arr_size, int measurements);
void list_dir_runner(FILES *amount_arr, size_t arr_size, int measurements);

void write_seq_runner(const char *filename, BYTES *bytes_arr, BYTES *buffer_size_arr,
			size_t arr_size,
			int measurements);
void write_randomly_runner(const char *filename, BYTES *bytes_arr,
	BYTES *buffer_size_arr, BYTES *interval_len_arr, size_t arr_size,
	int measurements);

void read_seq_runner(const char *filename, BYTES *bytes_arr,
    BYTES *buffer_size_arr, size_t arr_size, int measurements);
void read_randomly_runner(const char *filename, BYTES *bytes_arr,
			  BYTES *buffer_size_arr, BYTES *interval_len_arr,
			  size_t arr_size, int measurements);

#endif