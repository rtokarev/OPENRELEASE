/*
 * Copyright (c) 2011 Roman Tokarev <roman.s.tokarev@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *        may be used to endorse or promote products derived from this software
 *        without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <mem_patch.h>

#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


#define MAPS_SIZE 256

#define PAGE_SIZE 4096


struct map {
	void *begin_addr;
	void *end_addr;
	int perms;
	off_t offset;
};


static struct map maps[MAPS_SIZE];


void mmaps_init(void)
{
	pid_t pid;
	char maps_filename[PATH_MAX + 1];
	char line[256];
	FILE *maps_file;
	struct map *map = maps;

	memset(maps, 0, sizeof(maps));

	pid = getpid();
	snprintf(maps_filename, sizeof(maps_filename), "/proc/%u/maps", pid);
	maps_file = fopen(maps_filename, "r");

	while (fgets(line, sizeof(line), maps_file) != NULL) {
		char perms[5];
		unsigned begin_addr, end_addr, offset;
		char *p;

		sscanf(line, "%x-%x %s %x", &begin_addr, &end_addr, perms, &offset);

		map->begin_addr = (void *)begin_addr;
		map->end_addr = (void *)end_addr;
		map->perms = PROT_NONE;
		map->offset = offset;
		p = perms;
		while (*p != '\0') {
			switch (*p) {
			case 'r':
				map->perms |= PROT_READ;
				break;
			case 'w':
				map->perms |= PROT_WRITE;
				break;
			case 'x':
				map->perms |= PROT_EXEC;
				break;
			default:
				break;

			}

			++p;
		}

		++map;

		assert(map != &maps[MAPS_SIZE]);
	}

	fclose(maps_file);
}

static struct map *search_map(void *vaddr)
{
	for (struct map *map = maps; map != &maps[MAPS_SIZE]; ++map) {
		if (vaddr >= map->begin_addr && vaddr < map->end_addr)
			return map;
	}

	return NULL;
}

void vmem_patch1(void *vaddr, int value)
{
	struct map *map = search_map(vaddr);

	if (!(map->perms & PROT_WRITE))
		mprotect(map->begin_addr, map->end_addr - map->begin_addr,
			 map->perms | PROT_WRITE);

	*(int *)vaddr = value;

	if (!(map->perms & PROT_WRITE))
		mprotect(map->begin_addr, map->end_addr - map->begin_addr,
			 map->perms);
}

void vmem_patch2(void *vaddr, int value)
{
	struct map *map = search_map(vaddr);
	void *raddr;

	if (map->offset == 0) {
		/* It's not a XIPed region. Patch it using virtual address */
		vmem_patch1(vaddr, value);

		return;
	}

	raddr = (void *)map->offset + (vaddr - map->begin_addr);

	rmem_patch(raddr, value);
}

void rmem_patch(void *raddr, int value)
{
	static int fd = -1;
	static void *map_base = NULL;
	static void *devmem_base = NULL;
	void *vaddr;

	if (raddr == NULL) {
		if (map_base != NULL)
			munmap(map_base, PAGE_SIZE);

		return;
	}

	if (devmem_base == NULL ||
	    raddr < devmem_base || raddr > (devmem_base + PAGE_SIZE)) {
		if (devmem_base != NULL) {
			assert(map_base != NULL);

			munmap(map_base, PAGE_SIZE);
			map_base = NULL;
			devmem_base = NULL;
		}

		if (fd == -1) {
			fd = open("/dev/mem", O_RDWR);
			assert(fd != -1);
		}

		devmem_base = (void *)((off_t)raddr & ~(PAGE_SIZE - 1));
		map_base = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
				fd, (off_t)devmem_base);
		assert(map_base != NULL);
	}

	vaddr = map_base + ((off_t)raddr & (PAGE_SIZE - 1));

	*(int *)vaddr = value;
}
