#include <gint/config.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/clock.h>
#include <gint/gint.h>
#include <gint/bfile.h>

#if GINT_RENDER_MONO
#include "images-fx.h"
#else
#include <fcntl.h>
#include <unistd.h>
extern bopti_image_t img_cat;
extern bopti_image_t img_cat_r;
extern bopti_image_t img_cat_l;
extern bopti_image_t img_cat_lr;
#endif

static int count;
#define SAVE_SIZE 4

static int sw_save(int value)
{
	uint32_t v = (uint32_t)value;

#if GINT_RENDER_MONO
	uint16_t const *path = u"\\\\fls0\\bongocount";
	BFile_Remove(path);
	int size = SAVE_SIZE;
	if(BFile_Create(path, BFile_File, &size) < 0)
		return -1;
	int fd = BFile_Open(path, BFile_ReadWrite);
	if(fd >= 0) {
		BFile_Write(fd, &v, SAVE_SIZE);
		BFile_Close(fd);
	}
#else
	int fd = open("/fls0/bongocount", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd >= 0) {
		write(fd, &v, SAVE_SIZE);
		close(fd);
	}
#endif
	return 0;
}

static int sw_load(void)
{
	uint32_t v = 0;

#if GINT_RENDER_MONO
	uint16_t const *path = u"\\\\fls0\\bongocount";
	int fd = BFile_Open(path, BFile_ReadOnly);
	if(fd >= 0) {
		BFile_Read(fd, &v, SAVE_SIZE, 0);
		BFile_Close(fd);
	}
#else
	int fd = open("/fls0/bongocount", O_RDONLY);
	if(fd >= 0) {
		read(fd, &v, SAVE_SIZE);
		close(fd);
	}
#endif
	if(v & 0x80000000)
		return 0;
	return (int)v;
}

static void save_count(void)
{
	gint_world_switch(GINT_CALL(sw_save, count));
}

static void draw_centered(bopti_image_t const *img)
{
	dclear(C_WHITE);
	dimage((DWIDTH - img->width) / 2, (DHEIGHT - img->height) / 2, img);
	dprint(2, 2, C_BLACK, "%d", count);
	dupdate();
}

static void show_paw(bopti_image_t const *img)
{
	draw_centered(img);
	sleep_ms(100);
}

int main(void)
{
	int show_left = 0;
	int pressed = 0;
	count = gint_world_switch(GINT_CALL(sw_load));

	for (;;) {
		key_event_t ev = pollevent();

		if (ev.type == KEYEV_NONE) {
			draw_centered(&img_cat);
			sleep_ms(15);
			continue;
		}

		if (ev.type == KEYEV_DOWN) {
			if (ev.key == KEY_EXIT) {
				save_count();
				return 0;
			}
			if (ev.key == KEY_MENU) {
				save_count();
				gint_osmenu();
				continue;
			}
			pressed++;
			count++;
		}
		else if (ev.type == KEYEV_UP) {
			if (pressed > 0)
				pressed--;
			continue;
		}
		else
			continue;
		if (pressed >= 2)
			show_paw(&img_cat_lr);
		else if (pressed == 1) {
			show_paw(show_left ? &img_cat_l : &img_cat_r);
			show_left = !show_left;
		}
	}

	return 0;
}