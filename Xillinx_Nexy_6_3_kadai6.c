/* ��U�͉ۑ�@�X�g�b�v�E�H�b�`�̊g��                */
/* Copyright(C) 2013 Cobac.Net, All rights reserved. */

#include <stdio.h>
#include "platform.h"
#include "IO_Module.h"
#include "mb_interface.h"

#define LED *((volatile unsigned int *) 0xc2000000)
#define FIT1_int 0x80
#define SW GPI1

#define SW1_MASK 0x01
#define SW2_MASK 0x02

#define RESET 0
#define RUN   1
#define STOP  2
#define SPLIT 3

/* �����֐���` */
void timer_int_handler( void * );
void disp( void );
unsigned int mkpio_data( int time );

static unsigned long system_timer;
unsigned long nowtime;
unsigned char SwPort_25ms;
int mode, dp, split_time;

int main()
{
    unsigned char pre_sw, now_sw;
    init_platform();

    nowtime = system_timer = split_time = dp = 0;
    pre_sw = now_sw = 0;
    mode = RESET;

    microblaze_register_handler(timer_int_handler, (void *)0);
    microblaze_enable_interrupts();
    IRQ_ENABLE = FIT1_int;

    while(1) {
        /* LED�\�� */
        disp();
        /* �X�C�b�`���̓`�F�b�N */
        pre_sw = now_sw;
        now_sw = SwPort_25ms;
        /* BTN0�`�F�b�N */
        if ( (pre_sw & SW1_MASK)==0 && (now_sw & SW1_MASK)!=0 ) {
            switch ( mode ) {
                case RESET: mode = RUN;  break;
                case RUN:   mode = STOP; break;
                case STOP:  mode = RUN;  break;
                case SPLIT: mode = STOP; break;
            }
        }
        /* BTN1�`�F�b�N */
        if ( (pre_sw & SW2_MASK)==0 && (now_sw & SW2_MASK)!=0 ) {
            switch ( mode ) {
                case RESET: break;
                case RUN:   mode = SPLIT; split_time = nowtime; break;
                case STOP:  mode = RESET; nowtime = 0; break;
                case SPLIT: mode = RUN;   break;
            }
        }
    }

    cleanup_platform();
    return 0;
}

/* ���荞�ݏ��� */
void timer_int_handler( void *arg )
{
    /* 1ms���ƂɃJ�E���g�A�b�v */
    system_timer++;
    /* 25ms���ƂɃX�C�b�`�|�[�g����荞�� */
    if ( (system_timer % 25) == 0 )
        SwPort_25ms = SW;
    /* �v���J�E���^�̃C���N�������g�Ə���`�F�b�N */
    if ( (system_timer % 10) == 0 && (mode == RUN || mode == SPLIT) ) {
        if ( nowtime++ >= 6000 )
            nowtime = 0;
    }
    /* dp��4Hz�œ��� */
    if ( (system_timer % 250) == 0 )
        dp ^= 1;
    IRQ_ACK = FIT1_int;
}

/* LED�\���֐� */
void disp( void )
{
    switch ( mode ) {
        case RESET: LED = mkpio_data( nowtime )    | 0xf40000;            break;
        case RUN:   LED = mkpio_data( nowtime )    | 0xf00000 | (dp<<18); break;
        case STOP:  LED = mkpio_data( nowtime )    | 0xf40000;            break;
        case SPLIT: LED = mkpio_data( split_time ) | 0xf00000 | (dp<<18); break;
    }
}

/* 4�����̕\���f�[�^���쐬 */
unsigned int mkpio_data( int time )
{
    return ( time/1000   << 12) | ((time/100)%10 << 8) |
           ((time/10)%10 <<  4) |   time%10;
}
