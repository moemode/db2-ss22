#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* scan the tail file (assuming it is of type int, column name 'a') */
void scan_tail(int32_t *tail, off_t size) {
    for (off_t i = 0; i < size / sizeof(int32_t); i += 1) {
        printf("row #%jd: a = %d\n", i, tail[i]);
    }
}

/* mmap file at ‹path›, return address and ‹size› of memory map */
void *mmap_file(char *path, off_t *size) {
    int fd;
    struct stat status;
    void *map;

    fd = open(path, O_RDONLY);
    assert(fd >= 0);
    assert(stat(path, &status) == 0);
    *size = status.st_size;

    map = mmap(NULL, *size, PROT_READ, MAP_SHARED, fd, 0);
    assert(map != MAP_FAILED);

    close(fd);

    return map;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <tail_file_path>\n", argv[0]);
        return 1;
    }

    off_t tail_size;
    void *tail_map;
    char *tail_path = argv[1];

    /* map tail file into memory */
    tail_map = mmap_file(tail_path, &tail_size);

    scan_tail(tail_map, tail_size);

    return 0;
}
