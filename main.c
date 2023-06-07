#include <CoreVideo/CVDisplayLink.h>
#include <QuartzCore/CoreAnimation.h>
#include <SDL.h>
#include <stdio.h>
// #define TRUE true // Needed in the demo
#define VK_USE_PLATFORM_METAL_EXT
#include "external/Vulkan-Tools/cube/cube.c"

struct demo demo;

static CVReturn display_link_cb(CVDisplayLinkRef displayLink,
                                const CVTimeStamp *now,
                                const CVTimeStamp *outputTime,
                                CVOptionFlags flagsIn, CVOptionFlags *flagsOut,
                                void *pUserData) {

  demo_draw(&demo);

  return kCVReturnSuccess;
}

int main(int argc_ignored, const char *argv_ignored[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow(
      "Test", 640, 480, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  void *view = SDL_Metal_CreateView(window);
  void *layer = SDL_Metal_GetLayer(view);
  CVDisplayLinkRef display_link;

  // Stolen from DemoViewController.m in the MoltenVK cube demo
  bool useDisplayLink = true;
  VkPresentModeKHR vkPresentMode =
      useDisplayLink ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR;
  char vkPresentModeStr[64];
  sprintf(vkPresentModeStr, "%d", vkPresentMode);

  const char *argv[] = {"cube", "--present_mode", vkPresentModeStr};
  int argc = sizeof(argv) / sizeof(char *);

  demo_main(&demo, layer, argc, argv);

  if (useDisplayLink) {
    CVDisplayLinkCreateWithActiveCGDisplays(&display_link);
    CVDisplayLinkSetOutputCallback(display_link, &display_link_cb, &demo);
    CVDisplayLinkStart(display_link);
  } else {
    dispatch_async(
        dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
          while (true) {
            demo_draw(&demo);
          }
        });
  }

  int running = 1;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = 0;
      }
    }
  }

  demo_cleanup(&demo);
  CVDisplayLinkRelease(display_link);
  SDL_DestroyWindow(window);
  return 0;
}