#include "bootanimation.h"
#include "fbviewer.h"
#include "de_display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include <dirent.h>
#include <sys/resource.h>

#include "jpeg_wrapper.h"

#include <sys/ioctl.h>
#include "sunxi_g2d.h"

#define PAN_STEPPING 20


static int opt_alpha = 0;

typedef int64_t nsecs_t;
#define ms2ns(n)		(n*1000000LL)
#define s2ns(n)			(ms2ns(1000))
#define ns2us(n)		(n*1000LL)
#define BOOT_PLAY_RES_PATH	"/usr/res/boot-play/"
#define WEB_LOAD_DONE		"/tmp/web_load_done"

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   600


static bool gExitPending;

void bootanimation_exit()
{
	fprintf(stderr, "bootanimation_exit!\n");
	gExitPending = true;
}

static inline nsecs_t systemTime(void)
{
	struct timeval t = {0};

	gettimeofday(&t, NULL);
	return (nsecs_t)(t.tv_sec)*1000000000LL + (nsecs_t)(t.tv_usec)*1000LL;
}


static int load_image(char *filename, struct Image *i)
{
	int (*load)(char *, unsigned char *, unsigned char **, int, int);
	unsigned char * image = NULL;
	unsigned char * alpha = NULL;

	int x_size, y_size;

#ifdef FBV_SUPPORT_PNG
	if(fh_png_id(filename))
	if(fh_png_getsize(filename, &x_size, &y_size) == FH_ERROR_OK)
	{
		load = fh_png_load;
		goto identified;
	}
#endif

#ifdef FBV_SUPPORT_JPEG
	if(fh_jpeg_id(filename))
	if(fh_jpeg_getsize(filename, &x_size, &y_size) == FH_ERROR_OK)
	{
		load = fh_jpeg_load;
		goto identified;
	}
#endif

#ifdef FBV_SUPPORT_BMP
	if(fh_bmp_id(filename))
	if(fh_bmp_getsize(filename, &x_size, &y_size) == FH_ERROR_OK)
	{
		load = fh_bmp_load;
		goto identified;
	}
#endif
	fprintf(stderr, "%s: Unable to access file or file format unknown.\n", filename);
	return(1);

identified:
	if(!(image = (unsigned char*)malloc(x_size * y_size * 3)))
	{
		fprintf(stderr, "%s: Out of memory.\n", filename);
		return -1;
	}
	if(load(filename, image, &alpha, x_size, y_size) != FH_ERROR_OK)
	{
		fprintf(stderr, "%s: Image data is corrupt?\n", filename);
		free(image);
		return -1;
	}
	if(!opt_alpha)
	{
		free(alpha);
		alpha = NULL;
	}


	i->width = x_size;
	i->height = y_size;
	i->rgb = image;
	i->alpha = alpha;
	i->do_free = 0;


	return 0;
}


void* convertRGB2FB(int fh, unsigned char *rgbbuff, unsigned long count, int bpp, int *cpp);

static int preload_image(char *filename, struct framebuffer *fb, struct Image *i)
{
	int ret = 0;

	if (!i->fbbuffer) {
		if (!i->rgb) {
			ret = load_image(filename, i);
			if (ret != 0) {
				fprintf(stderr, "load_image [%s] failed\n", filename);
				return ret;
			}
		}
		i->fbbuffer = (unsigned char*)convertRGB2FB(0, i->rgb, i->width * i->height, fb->var.bits_per_pixel, &fb->cpp);
		free(i->rgb);
		i->rgb = NULL;
		if(i->width < fb->screen_x)
			i->x_offs = (fb->screen_x - i->width) / 2;
		else
			i->x_offs = 0;

		if(i->height < fb->screen_y)
			i->y_offs = (fb->screen_y - i->height) / 2;
		else
			i->y_offs = 0;
	}
	return ret;
}

static int show_image(char *filename, struct framebuffer *fb, struct Image *i)
{

	int ret = 0;

	ret = preload_image(filename, fb, i);

	fb_show(i, fb);
#ifdef EACHLOAD_IMAGE
	free(i->fbbuffer);
	i->fbbuffer = NULL;
#endif
	return ret;
}

struct Frame {
	char name[32];
	struct Image image;
	struct Frame *next;
};
struct Part {
	char path[64];
	int count;
	int pause;
	int playUntilComplete;
	int frame_num;
	struct Frame *frame;
	struct Part *next;
};
struct Animation {
	int fps;
	int width;
	int height;
	int part_num;
	struct Part *part;
};

static int filter(const struct dirent *dir_ent)
{
	if ('.' == *dir_ent->d_name)
		return 0;
	return 1;
}

FILE *desc_fp;
#if BOOT_PLAY_USE_JPEGDECODER_DE
static struct JpegDecoderWrapper_t DecoderWrapper;
static struct dedisplay de;
struct SunxiMemOpsS *pMemops;
char* pVirBuf;
bool logo_rotate = false;
#else
static struct framebuffer fb;
#endif
static int movie(void)
{
	struct Animation animation;
	FILE *desc_fp;
	struct Part *part_cur = NULL;
	char buf[128];
	char path[128];
	int i,j,r;
	FILE *done_fp = NULL;
	int errnum = 0;

	nsecs_t frameDuration, lastFrame, nowFrame, delay;

	setpriority(PRIO_PROCESS, 0, BOOT_ANIMATION_PRIORITY);

	desc_fp = fopen(BOOT_PLAY_RES_PATH"desc.txt", "r");
	if (!desc_fp) {
		fprintf(stderr, "open desc failed\n");
		return -1;
	}
	printf("%s %d\n", __func__, __LINE__);
#if BOOT_PLAY_USE_JPEGDECODER_DE
	FILE *cmd_fp = NULL;
	char cmd_buf[128] = {0};
	uint32_t len;
	uint8_t *yuv_buf;
	int g2d_ret = -1;

	JpegDecoderInit(&DecoderWrapper);

	DecoderWrapper.ScaleRatio = JPEG_DECODE_SCALE_DOWN_1;
	DecoderWrapper.OutputDataTpe = JpegDecodeOutputDataYU12;

	cmd_fp = popen("/sbin/fw_printenv -n logo_rotate", "r");
	if (NULL == cmd_fp) {
		fprintf(stderr, "popen failed.\n");
	} else {

		while(fgets(cmd_buf, 128, cmd_fp)) {
			fprintf(stdout, "fw_printenv -n logo_rotate: %s", cmd_buf);
		}
		pclose(cmd_fp);
		cmd_fp = NULL;

		if(!strncmp(cmd_buf, "1", 1)) {
			logo_rotate = true;
			g2d_ret = init_config_g2d(SCREEN_WIDTH, SCREEN_HEIGHT, G2D_FORMAT_YUV420_PLANAR);

			pMemops = GetMemAdapterOpsS();
			SunxiMemOpen(pMemops);

			pVirBuf = (char*)SunxiMemPalloc(pMemops, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
			if (NULL == pVirBuf) {
				printf("%s %d: SunxiMemPalloc failed!\n", __func__, __LINE__);
			} else {
				memset((void*)pVirBuf, 0x0, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
			}
		}
	}

	de.width = SCREEN_WIDTH;
	de.height = SCREEN_HEIGHT;

	if(de_display_init(&de) < 0){
		fprintf(stderr, "fb_init failed.\n");
		fclose(desc_fp);
		return -1;
	}
#else
	if (fb_init(&fb) < 0) {
		fprintf(stderr, "fb_init failed.\n");
		fclose(desc_fp);
		return -1;
	}
#endif
	memset(&animation, 0, sizeof(animation));
	while(fgets(buf, sizeof(buf), desc_fp) != NULL) {
		int fps, width, height, count, pause;
		char pathType;
		if (sscanf(buf, "%d %d %d", &width, &height, &fps) == 3) {
			animation.width = width;
			animation.height = height;
			animation.fps = fps;
#ifdef DEBUG
			printf("width=%d, height=%d, fps=%d\n", animation.width, animation.height, animation.fps);
#endif
		} else if (sscanf(buf, " %c %d %d %32s", &pathType, &count, &pause, path) == 4) {
			struct Part *part;

			part = (struct Part *)malloc(sizeof(struct Part));
			memset(part, 0, sizeof(struct Part));
			part->playUntilComplete = pathType == 'c';
			part->count = count;
			part->pause = pause;
			snprintf(part->path, sizeof(part->path), "%s", path);
#ifdef DEBUG
			printf("===============\n");
			printf( "Part:\n"
				"path:%s\n"
				"count:%d\n"
				"pause:%d\n"
				"PlayUNtilComplete:%d\n",
				part->path, part->count, part->pause, part->playUntilComplete);
#endif
			if (!part_cur) {
				part_cur = part;
				animation.part = part;
			} else {
				part_cur->next = part;
				part_cur = part;
			}
			animation.part_num++;
		}
	}

	frameDuration = s2ns(1) / animation.fps;

	part_cur = animation.part;
	for (i=0; (i < animation.part_num) && (part_cur != NULL); i++) {
		struct Frame *frame, *frame_cur = NULL;
		struct dirent **namelist;
		int num;

		snprintf(path, sizeof(path), BOOT_PLAY_RES_PATH"%s", part_cur->path);
#ifdef DEBUG
		printf("===============\n");
		printf("%s:\n", path);
#endif
		num = scandir(path, &namelist, filter, alphasort);
		if (num < 0) {
			fprintf(stderr, "scandir num = %d\n", num);
			goto exit;
		}
		for (j=0; j < num; j++) {
			if (namelist[j]->d_type & DT_REG) {
				frame = (struct Frame *)malloc(sizeof(struct Frame));
				memset(frame, 0, sizeof(struct Frame));
				snprintf(frame->name, sizeof(frame->name), "%s", namelist[j]->d_name);
#ifdef DEBUG
				printf("%s:%s\n", path, frame->name);
#endif
				if (!frame_cur) {
					frame_cur = frame;
					part_cur->frame = frame;
				} else {
					frame_cur->next = frame;
					frame_cur = frame;
				}
				part_cur->frame_num++;
			}
			free(namelist[j]);
		}
		free(namelist);
		part_cur = part_cur->next;
	}
	part_cur = animation.part;
	for (i=0; i < animation.part_num; i++) {
		for (r=0; (part_cur != NULL) && (!part_cur->count || r < part_cur->count); r++) {
			struct Frame *frame = part_cur->frame;
			if (!frame)
				break;
			if (gExitPending && !part_cur->playUntilComplete)
				break;
			for (j=0; j < part_cur->frame_num && (frame != NULL) && (!gExitPending || part_cur->playUntilComplete); j++) {
				lastFrame = systemTime();
				snprintf(path, sizeof(path), BOOT_PLAY_RES_PATH"%s/%s", part_cur->path, frame->name);
#if BOOT_PLAY_USE_JPEGDECODER_DE
				JpegDecompress(path, &DecoderWrapper);
				if(DecoderWrapper.ImgFrame != NULL) {
					len = DecoderWrapper.ImgFrame->mYuvSize;

					if ((g2d_ret == 0) && logo_rotate) {
						if (g2d_rotate(DecoderWrapper.ImgFrame->mYuvData, len, pVirBuf) < 0)
							break;
						yuv_buf = pVirBuf;
					} else {
						yuv_buf = DecoderWrapper.ImgFrame->mYuvData;
					}

					de_disp_display(&de, yuv_buf, len, VIDEO_PIXEL_FORMAT_YUV_PLANER_420);
				}
#else
				show_image(path, &fb, &frame->image);
#endif

#ifdef DEBUG
				printf("%s\n", path);
#endif
				nowFrame = systemTime();
				delay = frameDuration - (nowFrame - lastFrame);
				if (delay > 0) {
					usleep(delay/1000);
				}
				frame = frame->next;

				done_fp = fopen(WEB_LOAD_DONE, "r");
				if (NULL == done_fp)
				{
					errnum = errno;
					//printf("%s: open %s fail, errno = %s, waiting...\n", __func__, WEB_LOAD_DONE, strerror(errnum));
				} else {
					fclose(done_fp);
					gExitPending = true;
					part_cur->count = 0;
					done_fp = NULL;
					break;
				}
			}
			usleep(part_cur->pause * ns2us(frameDuration));
			if ((gExitPending || part_cur->playUntilComplete)
				&& !part_cur->count)
				break;
#ifndef EACHLOAD_IMAGE
			if (!part_cur->count)
				setpriority(PRIO_PROCESS, 0, 0);
#endif
		}
		part_cur = part_cur->next;
	}

	printf("%s: movie display done!\n", __func__);
    /*
	i = 0;
	while(i++ < 60) {
		done_fp = fopen(WEB_LOAD_DONE, "r");
		if (NULL == done_fp)
		{
			errnum = errno;
			printf("%s: open %s fail, errno = %s, waited %d s...\n", __func__, WEB_LOAD_DONE, strerror(errnum), i);
		} else {
			fclose(done_fp);
			done_fp = NULL;
			break;
		}
		sleep(1);
	}
    */

exit:
	part_cur = animation.part;
	for (i=0; (i < animation.part_num) && (part_cur != NULL); i++) {
		struct Part *part_next = part_cur->next;
		struct Frame *frame = part_cur->frame;
		for (j=0; (j < part_cur->frame_num) && (frame != NULL); j++) {
			struct Frame *frame_next = frame->next;
			if (frame->image.fbbuffer != NULL) {
				free(frame->image.fbbuffer);
				frame->image.fbbuffer = NULL;
			}
			free(frame);
			frame = frame_next;
		}
		free(part_cur);
		part_cur = part_next;
	}
#if BOOT_PLAY_USE_JPEGDECODER_DE
	de_disp_uninit(&de);
	JpegDecoderUnInit(&DecoderWrapper);

	if (logo_rotate) {
		uninit_config_g2d();

		if (pMemops && pVirBuf) {
			SunxiMemPfree(pMemops, pVirBuf);
			SunxiMemClose(pMemops);
		}
	}
#else
	fb_release(&fb);
#endif
	fclose(desc_fp);

	return 0;
}

static int tina()
{
	struct framebuffer fb;
	struct Image image[2];
	int loop,w,h,fps=20;
	nsecs_t duration, now, last, delay;

	if (access(BOOT_PLAY_RES_PATH"mask.png", F_OK) != 0)
		return -1;
	if (access(BOOT_PLAY_RES_PATH"shine.png", F_OK) != 0)
		return -1;

	duration = s2ns(1)/fps;
	memset(&fb, 0, sizeof(fb));
	memset(&image, 0, sizeof(image));
	setpriority(PRIO_PROCESS, 0, BOOT_ANIMATION_PRIORITY);

	if (fb_init(&fb) < 0) {
		fprintf(stderr, "fb_init failed.\n");
		return -1;
	}
	memset(fb.fb_base, 0x00, fb.fix.smem_len);

	if (preload_image(BOOT_PLAY_RES_PATH"mask.png", &fb, &image[0]) != 0)
		goto exit;
	if (preload_image(BOOT_PLAY_RES_PATH"shine.png", &fb, &image[1]) != 0)
		goto exit;

	if (image[1].width < image[0].width)
		goto exit;

	while(true) {
		for (loop=0; loop < image[1].width-image[0].width+1; loop++) {
			last = systemTime();
			for (w=0; w < image[0].width; w++) {
				for (h=0; h < image[0].height; h++) {
					if (image[0].fbbuffer[4*((h*(image[0].width))+w)+3] != 0) {
						image[0].fbbuffer[4*(h*image[0].width+w)] =
							image[1].fbbuffer[4*(h*image[1].width+w+loop)];
						image[0].fbbuffer[4*(h*image[0].width+w)+1] =
							image[1].fbbuffer[4*(h*image[1].width+w+loop)+1];
						image[0].fbbuffer[4*(h*image[0].width+w)+2] =
							image[1].fbbuffer[4*(h*image[1].width+w+loop)+2];
					}
				}
			}

			fb_show(&image[0], &fb);
			loop += 5;
			now = systemTime();
			delay = duration - (now - last);
			if (delay > 0)
				usleep(delay/1000);
		}
	}
exit:
	free(image[0].fbbuffer);
	image[0].fbbuffer = NULL;
	free(image[1].fbbuffer);
	image[1].fbbuffer = NULL;
	fb_release(&fb);
	return 0;
}

static void terminate(int sig_no)
{
    int errnum = 0;

	LOG("bootanimation shutdown sig_no %d\n", sig_no);
	fflush(stderr);
	sync();
#if BOOT_PLAY_USE_JPEGDECODER_DE
	de_disp_uninit(&de);
	JpegDecoderUnInit(&DecoderWrapper);

	if (logo_rotate) {
		uninit_config_g2d();

		if (pMemops && pVirBuf) {
			SunxiMemPfree(pMemops, pVirBuf);
			SunxiMemClose(pMemops);
		}
	}

#else
	fb_release(&fb);
#endif
	if(desc_fp)
		fclose(desc_fp);

	exit(0);
}


static void install_sig_handler(void)
{
	signal(SIGBUS, terminate);
	signal(SIGFPE, terminate);
	signal(SIGHUP, terminate);
	signal(SIGILL, terminate);
	signal(SIGINT, terminate);
	signal(SIGIOT, terminate);
	signal(SIGPIPE, terminate);
	signal(SIGQUIT, terminate);
	signal(SIGSEGV, terminate);
	signal(SIGSYS, terminate);
	signal(SIGTERM, terminate);
	signal(SIGTRAP, terminate);
	signal(SIGKILL, terminate);
	signal(SIGUSR1, terminate);
	signal(SIGUSR2, terminate);
}

int bootanimation(void)
{
	install_sig_handler();
	if (movie() < 0)
		return tina();

	return 0;
}
