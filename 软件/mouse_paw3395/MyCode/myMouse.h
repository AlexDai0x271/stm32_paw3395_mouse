#ifndef _MYMOUSE_H_
#define _MYMOUSE_H_

#include "main.h"

//�����Ϣ�ṹ��
__packed struct mouseHID_t
{
	/*��갴����
	 *	bit0: ���	
	 *	bit1: �Ҽ� 
	 *	bit2: �м�
	 *	bit3: �Զ���
	 *	...
	 *	bit7: �Զ���
	 */
//	uint8_t button_left:1;
//	uint8_t button_right:1;
//	uint8_t button_middel:1;
	
//	//x���ƶ�ֵ
//	union{
//		struct{
//			uint8_t x_move_l;
//			uint8_t x_move_h;
//		}t_x_move;
//		uint16_t x_move;
//	}u_x_move;
//	
//	//y���ƶ�ֵ
//	union{
//		struct{
//			uint8_t y_move_l;
//			uint8_t y_move_h;
//		}t_y_move;
//		uint16_t y_move;
//	}u_y_move;
	
	uint8_t button_left:1;      //���
	uint8_t button_right:1;     //�Ҽ�
	uint8_t button_middel:1;    //�н�
	
	int16_t x;                  //x�����λ��
	int16_t y;                  //y�����λ��
	
	int8_t wheel;               //����λ�ƴ�С
};

void myMouse_init(struct mouseHID_t* mouseHID);
void myMouse_update(struct mouseHID_t* mouseHID);

#endif
