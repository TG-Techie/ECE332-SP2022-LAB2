/// No License or Copyright, this code is modified from provided example code

#pragma once

/// ----- peripheral base addresses -----
#define KEY_BASE              0xFF200050
#define VIDEO_IN_BASE         0xFF203060
#define FPGA_ONCHIP_VIDEO_BASE      0xC8000000
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define FPGA_CHAR_START			0xC9000000

/// ----- peripherals as pointers -----
static volatile int     * const KEY_ptr			    = (int *) KEY_BASE; // key peripheral 
static volatile int     * const Video_In_DMA_ptr	= (int *) VIDEO_IN_BASE; // the peripheral
static volatile short   * const Video_Mem_ptr	    = (short *) FPGA_ONCHIP_VIDEO_BASE; // vga output memory location

static volatile int     * const video_char_ctrl0    = (int *) CHAR_BUF_CTRL_BASE; // the FPGA
static volatile char    * const video_char_overlay0 = (char*) FPGA_CHAR_START; // the FPGA

