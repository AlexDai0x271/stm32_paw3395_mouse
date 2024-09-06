#include "PAW3395.h"
#include "Delay.h"
#include "usr_spi.h"
static void Power_Up_Initializaton_Register_Setting(void);


/*
 *�ϸ��տ���˳��ִ�У�
 *	��ȻоƬִ���ڲ��ϵ��Ը�λ�����Խ��齫Power_Up_Reset
 *	ÿ���ϵ�ʱ����д��Ĵ������Ƽ���оƬ�ϵ�˳������:
 *	1. ���κ�˳��ΪVDD��VDDIO���磬ÿ�ι���֮����ӳٲ�����100ms��ȷ�����й�Ӧ�ȶ���
 *	2. �ȴ�����50���롣
 *	3. ��NCS���ߣ�Ȼ������������SPI�˿ڡ�
 *	4. ��0x5Aд��Power_Up_Reset�Ĵ����������л�NRESET���ţ���
 *	5. �ȴ�����5ms��
 *	6. �����ϵ��ʼ���Ĵ������á�
 *	7. �����˶�λ״̬��Σ�����ȡ�Ĵ���0x02��0x03��0x04��0x05��0x06һ�Ρ�
 *
 *
 *
 *
 *
 */
void Power_up_sequence(void)
{
	uint8_t reg_it;
	//Wait for at least 50ms��2.�ȴ�����50���롣��
	delay_ms(50);
	// drive NCS high, then low to reset the SPI port��3.����SPI�˿ڣ�
	CS_Low;
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
	CS_High;
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
	CS_Low;
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
		
	// Write 0x5A to POWER_UP_Reset register��4.��0x5Aд��Power_Up_Reset�Ĵ�����
	writr_register(PAW3395_SPIREGISTER_POWERUPRESET
					,PAW3395_POWERUPRESET_POWERON);
	//Wait for at least 5ms��5.�ȴ�����5ms��
	delay_ms(5);
	//Load Power-up initialization register setting��6.�����ϵ��ʼ���Ĵ������ã�
	Power_Up_Initializaton_Register_Setting();
	CS_High;
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
	// read register from 0x02 to 0x06��7.�����˶�λ״̬��Σ�����ȡ�Ĵ���0x02��0x03��0x04��0x05��0x06һ�Σ�
	for(reg_it=0x02; reg_it<=0x06; reg_it++)
	{	
		read_register(reg_it);
		delay_us(PAW3395_TIMINGS_SRWSRR);	  
	}
	//Ƭѡ���ߣ�����SPIͨѶ����
	CS_High;
	delay_125_ns(PAW3395_TIMINGS_BEXIT);
}


/*
 *�ϸ�ִ������Motion Burst�ĳ���
 *	1. ����NCS��
 *	2. �ȴ�tNCS-SCLK
 *	3. ����Motion_Burst��ַ(0x16)�����ʹ˵�ַ��MOSIӦ���־�̬���ߵ�ƽ��͵�ƽ����ֱ��ͻ��������ɡ�
 *	4. �ȴ�tSRAD
 *	5. ��ʼ������ȡ���12���ֽڵ�SPI���ݡ�����ͨ����NCS���߳���at����ֹ�˶�ͻ������tBEXIT��
 *	6. Ҫ��ȡ�µ��˶�ͻ�����ݣ���Ӳ���1��ʼ�ظ���
 *
 *ע�⣺
 *	��ʹ�����л�ֹģʽ�£�Ҳ���Դ�Burst_Motion_Read�Ĵ�����ȡ�˶�ͻ�����ݡ�
 *
 */
void Motion_Burst(uint8_t *buffer)
{
	//Lower NCS
	CS_Low;
	//Wait for t(NCS-SCLK)
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
	//Send Motion_Brust address(0x16)
	SPI_SendReceive(PAW3395_SPIREGISTER_MotionBurst);	//��
	//Wait for tSRAD
	delay_us(PAW3395_TIMINGS_SRAD);
	//Start reading SPI data continuously up to 12 bytes.
	for(uint8_t i = 0;i < 12;i++)
	{
		buffer[i] = SPI_SendReceive(0x00);
	}
	CS_High;
	delay_125_ns(PAW3395_TIMINGS_BEXIT);
}

/*
 *�ϵ��ʼ���Ĵ�������
 *
 */
static void Power_Up_Initializaton_Register_Setting(void)
{
	uint8_t read_tmp;
	uint8_t i ;
	writr_register(0x7F ,0x07);
	writr_register(0x40 ,0x41);
	writr_register(0x7F ,0x00);
	writr_register(0x40 ,0x80);
	writr_register(0x7F ,0x0E);
	writr_register(0x55 ,0x0D);
	writr_register(0x56 ,0x1B);
	writr_register(0x57 ,0xE8);
	writr_register(0x58 ,0xD5);
	writr_register(0x7F ,0x14);
	writr_register(0x42 ,0xBC);
	writr_register(0x43 ,0x74);
	writr_register(0x4B ,0x20);
	writr_register(0x4D ,0x00);
	writr_register(0x53 ,0x0E);
	writr_register(0x7F ,0x05);
	writr_register(0x44 ,0x04);
	writr_register(0x4D ,0x06);
	writr_register(0x51 ,0x40);
	writr_register(0x53 ,0x40);
	writr_register(0x55 ,0xCA);
	writr_register(0x5A ,0xE8);
	writr_register(0x5B ,0xEA);
	writr_register(0x61 ,0x31);
	writr_register(0x62 ,0x64);
	writr_register(0x6D ,0xB8);
	writr_register(0x6E ,0x0F);

	writr_register(0x70 ,0x02);
	writr_register(0x4A ,0x2A);
	writr_register(0x60 ,0x26);
	writr_register(0x7F ,0x06);
	writr_register(0x6D ,0x70);
	writr_register(0x6E ,0x60);
	writr_register(0x6F ,0x04);
	writr_register(0x53 ,0x02);
	writr_register(0x55 ,0x11);
	writr_register(0x7A ,0x01);
	writr_register(0x7D ,0x51);
	writr_register(0x7F ,0x07);
	writr_register(0x41 ,0x10);
	writr_register(0x42 ,0x32);
	writr_register(0x43 ,0x00);
	writr_register(0x7F ,0x08);
	writr_register(0x71 ,0x4F);
	writr_register(0x7F ,0x09);
	writr_register(0x62 ,0x1F);
	writr_register(0x63 ,0x1F);
	writr_register(0x65 ,0x03);
	writr_register(0x66 ,0x03);
	writr_register(0x67 ,0x1F);
	writr_register(0x68 ,0x1F);
	writr_register(0x69 ,0x03);
	writr_register(0x6A ,0x03);
	writr_register(0x6C ,0x1F);

	writr_register(0x6D ,0x1F);
	writr_register(0x51 ,0x04);
	writr_register(0x53 ,0x20);
	writr_register(0x54 ,0x20);
	writr_register(0x71 ,0x0C);
	writr_register(0x72 ,0x07);
	writr_register(0x73 ,0x07);
	writr_register(0x7F ,0x0A);
	writr_register(0x4A ,0x14);
	writr_register(0x4C ,0x14);
	writr_register(0x55 ,0x19);
	writr_register(0x7F ,0x14);
	writr_register(0x4B ,0x30);
	writr_register(0x4C ,0x03);
	writr_register(0x61 ,0x0B);
	writr_register(0x62 ,0x0A);
	writr_register(0x63 ,0x02);
	writr_register(0x7F ,0x15);
	writr_register(0x4C ,0x02);
	writr_register(0x56 ,0x02);
	writr_register(0x41 ,0x91);
	writr_register(0x4D ,0x0A);
	writr_register(0x7F ,0x0C);
	writr_register(0x4A ,0x10);
	writr_register(0x4B ,0x0C);
	writr_register(0x4C ,0x40);
	writr_register(0x41 ,0x25);
	writr_register(0x55 ,0x18);
	writr_register(0x56 ,0x14);
	writr_register(0x49 ,0x0A);
	writr_register(0x42 ,0x00);
	writr_register(0x43 ,0x2D);
	writr_register(0x44 ,0x0C);
	writr_register(0x54 ,0x1A);
	writr_register(0x5A ,0x0D);
	writr_register(0x5F ,0x1E);
	writr_register(0x5B ,0x05);
	writr_register(0x5E ,0x0F);
	writr_register(0x7F ,0x0D);
	writr_register(0x48 ,0xDD);
	writr_register(0x4F ,0x03);
	writr_register(0x52 ,0x49);
		
	writr_register(0x51 ,0x00);
	writr_register(0x54 ,0x5B);
	writr_register(0x53 ,0x00);
		
	writr_register(0x56 ,0x64);
	writr_register(0x55 ,0x00);
	writr_register(0x58 ,0xA5);
	writr_register(0x57 ,0x02);
	writr_register(0x5A ,0x29);
	writr_register(0x5B ,0x47);
	writr_register(0x5C ,0x81);
	writr_register(0x5D ,0x40);
	writr_register(0x71 ,0xDC);
	writr_register(0x70 ,0x07);
	writr_register(0x73 ,0x00);
	writr_register(0x72 ,0x08);
	writr_register(0x75 ,0xDC);
	writr_register(0x74 ,0x07);
	writr_register(0x77 ,0x00);
	writr_register(0x76 ,0x08);
	writr_register(0x7F ,0x10);
	writr_register(0x4C ,0xD0);
	writr_register(0x7F ,0x00);
	writr_register(0x4F ,0x63);
	writr_register(0x4E ,0x00);
	writr_register(0x52 ,0x63);
	writr_register(0x51 ,0x00);
	writr_register(0x54 ,0x54);
	writr_register(0x5A ,0x10);
	writr_register(0x77 ,0x4F);
	writr_register(0x47 ,0x01);
	writr_register(0x5B ,0x40);
	writr_register(0x64 ,0x60);
	writr_register(0x65 ,0x06);
	writr_register(0x66 ,0x13);
	writr_register(0x67 ,0x0F);
	writr_register(0x78 ,0x01);
	writr_register(0x79 ,0x9C);
	writr_register(0x40 ,0x00);
	writr_register(0x55 ,0x02);
	writr_register(0x23 ,0x70);
	writr_register(0x22 ,0x01);

	//Wait for 1ms
	delay_ms(1);
	
	for( i = 0 ;i < 60 ;i++)
	{
		read_tmp = read_register(0x6C);
		if(read_tmp == 0x80 )
			break;
		delay_ms(1);
	}
	if(i == 60)
	{
		writr_register(0x7F ,0x14);
		writr_register(0x6C ,0x00);
		writr_register(0x7F ,0x00);
	}
	writr_register(0x22 ,0x00);
	writr_register(0x55 ,0x00);
	writr_register(0x7F ,0x07);
	writr_register(0x40 ,0x40);
	writr_register(0x7F ,0x00);
}

/*
 *RawData�������
 *	��ȡ��ǰ�������ͷ���㵽�ĽӴ��������������Ϣ
 *	������Ϣ��������pFrame��
 *
 *ע�⣺
 *	��RawData��������У����뽫������ھ�ֹλ��
 *
 */
void Pixel_Burst_Read(uint8_t* pFrame)
{
	uint8_t reg_tmp;
	//Lower NCS
	CS_Low;
	//Wait for t(NCS-SCLK)
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
	writr_register(0x7F ,0x00);
	writr_register(0x40 ,0x80);
	
	do
	{
		reg_tmp = read_register(PAW3395_SPIREGISTER_MOTION);
		delay_us(PAW3395_TIMINGS_SRWSRR);
	}
	while((reg_tmp & ((1 << PAW3395_OP_MODE0) | (1 << PAW3395_OP_MODE1))) != 0);
		
	writr_register(0x50 ,0x01);
	writr_register(0x55 ,0x04);
	writr_register(0x58 ,0xFF);
	
	do
	{
		reg_tmp = read_register(0x59);
		delay_us(PAW3395_TIMINGS_SRWSRR);
	}
	while((reg_tmp & ((1 << PAW3395_PG_FIRST) | (1 << PAW3395_PG_VALID))) 
		!= ((1 << PAW3395_PG_FIRST) | (1 << PAW3395_PG_VALID)));
	
	pFrame[35*2]=read_register(0x58);//Read the first rawdata from register 0x58
	delay_us(PAW3395_TIMINGS_SRWSRR);
	for(uint8_t width = 0;width < 36;width++)
	{
		for(uint8_t height = 0;height < 36;height++)
		{
			if((width == 0)&&(height == 0))
				continue;
			do
			{
				reg_tmp = read_register(0x59);
				delay_us(PAW3395_TIMINGS_SRWSRR);
			}
			while(!((reg_tmp >> PAW3395_PG_VALID) & 0x01));
			pFrame[(height * 36 + (35-width)) * 2] = read_register(0x58);
			delay_us(PAW3395_TIMINGS_SRWSRR);
		}
	}	
	
	writr_register(0x40 ,0x00);
	writr_register(0x50 ,0x00);
	writr_register(0x55 ,0x00);	
	CS_High;
	delay_125_ns(PAW3395_TIMINGS_BEXIT);
}


/*
 *
 *����DPI
 *
 */
void DPI_Config(uint16_t CPI_Num)
{
	uint8_t temp;
	
	//Lower NCS
	CS_Low;
	//Wait for t(NCS-SCLK)
	delay_125_ns(PAW3395_TIMINGS_NCS_SCLK);
	
	//���÷ֱ���ģʽ��X���Y��ֱ��ʾ���RESOLUTION_X_LOW��RESOLUTION_X_HIGH����
	writr_register(MOTION_CTRL, 0x00);
	
	//����8λ�Ĵ�������X��ֱ���
	temp = (uint8_t)(((CPI_Num/50) << 8) >> 8);
	writr_register(RESOLUTION_X_LOW, temp);
	temp = (uint8_t)((CPI_Num/50) >> 8);
	writr_register(RESOLUTION_X_HIGH, temp);
	
	//���·ֱ���
	writr_register(SET_RESOLUTION, 0x01);
	
	CS_High;
	delay_125_ns(PAW3395_TIMINGS_BEXIT);
}
