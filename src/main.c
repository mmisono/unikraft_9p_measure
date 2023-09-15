#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>

#if 0
#include <vfscore/mount.h>

#include <uk/fuse.h>
#include <uk/fusedev.h>
#include <uk/vfdev.h>
#endif

#include "measurement_scenarios.h"
#include "helper_functions.h"
#include "scenario_runners.h"

/* returns base to the power of exp */
BYTES bpow(BYTES base, BYTES exp)
{
    BYTES result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

int main_old(int argc, char *argv[])
{

#if 0
	struct uk_fusedev_trans *trans;
	struct uk_fuse_dev *dev;
    int rc;
	trans = uk_fusedev_trans_get_default();
	dev = uk_fusedev_connect(trans, "fs0", NULL);
	if ((rc = uk_fuse_request_init(dev))) {
		uk_pr_err("uk_fuse_init has failed \n");
		return 0;
	}

	fuse_file_context dc = { .is_dir = true, .name = "create_files",
		.mode = 0777, .parent_nodeid = 1
	};
	rc = uk_fuse_request_mkdir(dev, dc.parent_nodeid,
		dc.name, 0777, &dc.nodeid,
		&dc.nlookup);
	if (rc) {
		uk_pr_err("uk_fuse_request_mkdir has failed \n");
	}

	return 0;

	const char *rootfs = "virtiofs";
	int rootflags = 0;
	char *path = "/";
	const char *rootopts = "";
    // char *fsdev = "fs0";
    char *fsdev = "";
	int ret;
	ret =  mount(fsdev, path, rootfs, rootflags, rootopts);
	if (ret != 0) {
		printf("Failed to mount virtiofs: %d\n", errno);
		// return 0;
	}
	FILE *fp = fopen("/a.txt", "w");
	if (fp == NULL) {
		printf("Failed to open file\n");
		return 0;
	}
	char *buf="hello\n";
	fwrite(buf, 1, 6, fp);
	fclose(fp);

    return 0;
#endif

	#ifdef __linux__
	char platform[6] = "Linux";
    chdir("guest_fs");
	#elif __Unikraft__
	char platform[9] = "Unikraft";
	#endif
	DEBUG_PRINT("__________________________\n");
	DEBUG_PRINT("Running in DEBUGMODE\n");
	DEBUG_PRINT("Running on %s\n", platform);
	DEBUG_PRINT("__________________________\n");

	// create_file_of_size("data_100M", MB(100));

	int max_pow = 17;
	FILES amount[max_pow];
	puts("amount");
	for (int i = 0; i < max_pow; i++) { // buffer_sizes = {16, 32, 64, ..., 2^max_pow}
		amount[i] = bpow(2, i + 1);
		printf("%lu\n", amount[i]);
	}

#if 0 // create files, listdir
	create_files_runner(amount, 17, 10);
	// remove_files_runner(amount, 17, 10);
	list_dir_runner(amount, 17, 10);
#endif

	// int max_pow2 = 20;
	int max_pow2 = 18;
	int min_pow2 = 4;
	int arr_size = max_pow2 - min_pow2 + 1;

	BYTES bytes_arr_FUSE[arr_size];
	BYTES buffer_size_arr[arr_size];
	BYTES interval_len_arr[arr_size];

	BYTES bytes_arr_DAX[arr_size];

	printf("buffer sizes\n");
	for (int i = min_pow2; i < max_pow2 + 1; i++) { // buffer_sizes = {16, 32, 64, ..., 2^max_pow}
		buffer_size_arr[i-min_pow2] = bpow(2, i);
		bytes_arr_FUSE[i-min_pow2] = buffer_size_arr[i-min_pow2] * bpow(2, 13);
		bytes_arr_DAX[i-min_pow2] = bpow(2, 20 + 13);
		// bytes_arr_DAX[i-min_pow2] = 100 * bpow(2, 20);
		interval_len_arr[i-min_pow2] = buffer_size_arr[i-min_pow2];
		printf("buffer_size: %llu\n\
			bytes: %llu\n\
			interval_len: %llu\n\
			---------------------\n",
			buffer_size_arr[i - min_pow2],
			bytes_arr_FUSE[i-min_pow2],
			interval_len_arr[i-min_pow2]);
	}
	int measurements = 3;

#ifdef DEBUGMODE
	measurements = 1;
#endif

	#ifdef __linux__

	write_seq_runner("1G_file", bytes_arr_DAX,
			buffer_size_arr, arr_size, measurements);
	write_randomly_runner("1G_file", bytes_arr_DAX,
			buffer_size_arr, interval_len_arr, arr_size,
			measurements);

	read_seq_runner("1G_file", bytes_arr_DAX,
			buffer_size_arr, arr_size, measurements);
	read_randomly_runner("1G_file", bytes_arr_DAX,
			buffer_size_arr, interval_len_arr, arr_size,
			measurements);

	#elif __Unikraft__

	write_seq_runner("/1G_file", bytes_arr_FUSE, buffer_size_arr, arr_size, measurements);
	write_randomly_runner("/1G_file", bytes_arr_FUSE,
			buffer_size_arr, interval_len_arr, arr_size,
			measurements);

	read_seq_runner("/1G_file", bytes_arr_FUSE,
			buffer_size_arr, arr_size, measurements);
			read_randomly_runner("/1G_file", bytes_arr_FUSE,
			buffer_size_arr, interval_len_arr, arr_size,
			measurements);

	#endif

	return 0;
}

int main(int argc, char *argv[])
{
	#ifdef __linux__
	char platform[6] = "Linux";
	char *filename = "./file";
	chdir("fs_linux");
	#elif __Unikraft__
	char platform[9] = "Unikraft";
	char *filename = "/file";
	#endif
	DEBUG_PRINT("__________________________\n");
	DEBUG_PRINT("Running in DEBUGMODE\n");
	DEBUG_PRINT("Running on %s\n", platform);
	DEBUG_PRINT("__________________________\n");

#ifdef DEBUGMODE
	int measurements = 1;
#else
	int measurements = 5;
#endif

#ifdef DEBUGMODE
	BYTES buffer_size_arr[] = {4096};
#else
	BYTES buffer_size_arr[] = {4096, 8192, 16384, 32768, 65536};
#endif
	int arr_size = sizeof(buffer_size_arr) / sizeof(buffer_size_arr[0]);
	BYTES bytes_arr[arr_size];
	BYTES interval_len_arr[arr_size];
	int i;

	#ifdef DEBUGMODE
	BYTES array_size = bpow(2, 28);
	BYTES interval_len_size = bpow(2, 24);
	#else
	// BYTES array_size = bpow(2, 30);
	// BYTES interval_len_size = bpow(2, 26);
	BYTES array_size = bpow(2, 31);
	BYTES interval_len_size = bpow(2, 27);
	#endif
	for (i = 0; i < arr_size; i++) {
		bytes_arr[i] = array_size;
		interval_len_arr[i] = interval_len_size;
	}

	write_seq_runner(filename, bytes_arr, buffer_size_arr, arr_size, measurements);
	read_seq_runner(filename, bytes_arr, buffer_size_arr, arr_size, measurements);

#if 0
	write_randomly_runner(filename, bytes_arr,
			buffer_size_arr, interval_len_arr, arr_size,
			measurements);
	read_randomly_runner(filename, bytes_arr, buffer_size_arr, interval_len_arr, arr_size, measurements);
#endif

	return 0;
}
